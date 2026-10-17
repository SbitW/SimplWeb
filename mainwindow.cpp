/*
* Copyright (C) 2026 @SbitW
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) ant later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://gnu.org>.
*/
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QUrl>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>
#include <QSizePolicy>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
QWebEngineView* CustomWebView::createWindow(QWebEnginePage::WebWindowType type){
    Q_UNUSED(type);

    QMainWindow *popupWindow = new QMainWindow();
    CustomWebView *popupView = new CustomWebView(popupWindow, popupWindow);

    // メインウィンドウと同じプロファイルを引き継ぐことで、ログインセッションを共有させる
    if (this->page() && this->page()->profile()) {
        QWebEnginePage *popupPage = new QWebEnginePage(this->page()->profile(), popupView);
        popupView->setPage(popupPage);
    }

    // JavaScriptから「失せろ」と要求されたら、外枠ごと消去する
    connect(popupView->page(), &QWebEnginePage::windowCloseRequested, popupWindow, &QMainWindow::close);

    popupWindow->setCentralWidget(popupView);
    popupWindow->resize(600, 700);
    popupWindow->setWindowTitle("SimplWeb - ポップアップ");

    popupWindow->setAttribute(Qt::WA_DeleteOnClose);
    popupWindow->show();

    return popupView;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    bookmarkFilePath = configDir + "/bookmarks.json";
    initBookmarkFile();
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *MainLayout = new QVBoxLayout(centralWidget);
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
    topBarWidget = new QWidget(centralWidget);
    topBarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *topLayout = new QHBoxLayout(topBarWidget);
    topLayout->setContentsMargins(5,5,5,5);
    backBtn = new QPushButton("←", this);
    backBtn->setFixedSize(40,24);
    topLayout->addWidget(backBtn);
    fwdBtn = new QPushButton("→", this);
    fwdBtn->setFixedSize(40,24);
    topLayout->addWidget(fwdBtn);
    rldBtn = new QPushButton("⟳", this);
    rldBtn->setFixedSize(40,24);
    topLayout->addWidget(rldBtn);
    uriLineEdit = new QLineEdit(this);
    uriLineEdit->setPlaceholderText("URLを入力してEnter");
    topLayout->addWidget(uriLineEdit);
    goBtn = new QPushButton("Go", this);
    goBtn->setFixedSize(40,24);
    topLayout->addWidget(goBtn);
    addBmBtn = new QPushButton("★", this);
    addBmBtn->setFixedSize(40,24);
    topLayout->addWidget(addBmBtn);
    showBmBtn = new QPushButton("📖", this);
    showBmBtn->setFixedSize(40,24);
    topLayout->addWidget(showBmBtn);

    MainLayout->addWidget(topBarWidget);

    QWebEngineProfile *profile = new QWebEngineProfile("SimplWebProfile", this);
    profile->setPersistentStoragePath(configDir + "/browser_storage");
    profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    adBlocker = new AdBlockInterceptor(this);
    profile->setUrlRequestInterceptor(adBlocker);

    QString cssSelectors = ".adchoices, #adchoices, .ad-choices, .ad_choices, [title='AdChoices'], [class*='adchoices'], .criteo-privacy, .cto-privacy, [id*='criteo']";
    QString jsCode = QString(
        "(function() {"
        "  var style = document.getElementById('simplweb-adblock-style');"
        "  if (!style) {"
        "    style = document.createElement('style');"
        "    style.id = 'simplweb-adblock-style';"
        "    document.head.appendChild(style);"
        "  }"
        "  style.innerHTML = '%1 { display: none !important; }';"
        "})();"
    ).arg(cssSelectors);

    // 2. QWebEngineScriptオブジェクトを作成して設定
    QWebEngineScript script;
    script.setSourceCode(jsCode);
    script.setName("AdBlockCosmetic");

    // ページ読み込み完了時
    script.setInjectionPoint(QWebEngineScript::DocumentReady);

    script.setWorldId(QWebEngineScript::MainWorld);

    script.setRunsOnSubFrames(true); // iframe内の三角マークも逃さない

    // 3. プロファイルに直接登録
    profile->scripts()->insert(script);

    QWebEngineSettings *settings = profile->settings();
    settings->setFontFamily(QWebEngineSettings::StandardFont, "sans-serif");
    settings->setFontFamily(QWebEngineSettings::SerifFont, "sans-serif"); // 明朝体要求もゴシックにする
    settings->setFontFamily(QWebEngineSettings::SansSerifFont, "sans-serif");
    // 全画面表示機能をプロファイルレベルで有効化
    settings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

    view = new CustomWebView(this);
    QWebEnginePage *mainPage = new QWebEnginePage(profile, view);
    view->setPage(mainPage);
    MainLayout->addWidget(view);

    setCentralWidget(centralWidget);

    connect(uriLineEdit, &QLineEdit::returnPressed, this, &MainWindow::navigateToUri);
    connect(goBtn, &QPushButton::clicked, this, &MainWindow::navigateToUri);
    connect(backBtn, &QPushButton::clicked, view, &QWebEngineView::back);
    connect(fwdBtn, &QPushButton::clicked, view, &QWebEngineView::forward);
    connect(rldBtn, &QPushButton::clicked, view, &QWebEngineView::reload);
    connect(view, &QWebEngineView::urlChanged, this, &MainWindow::updateAddressBar);
    connect(addBmBtn, &QPushButton::clicked, this, &MainWindow::addCurrentBookmark);
    connect(showBmBtn, &QPushButton::clicked, this, &MainWindow::showBookmarksMenu);
    connect(view->page(), &QWebEnginePage::permissionRequested, this, &MainWindow::handlePermissionRequested);
    connect(view->page(), &QWebEnginePage::fullScreenRequested, this, &MainWindow::handleFullScreenRequested);

    uriLineEdit->setText("https://google.com");
    navigateToUri();

    resize(1024, 768);
}

MainWindow::~MainWindow() {}

void MainWindow::navigateToUri(){
    QString inputStr = uriLineEdit->text();
    if (!inputStr.startsWith("http://") && !inputStr.startsWith("https://")) {
        inputStr = "https://" + inputStr;
    }
    view->load(QUrl(inputStr));
}

void MainWindow::updateAddressBar(const QUrl &uri){
    uriLineEdit->setText(uri.toString());
}
void MainWindow::showBookmarksMenu(){
    QFile file(bookmarkFilePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray bmArray = doc.array();
    if (bmArray.isEmpty()) {
        QMenu emptyMenu(this);
        emptyMenu.addAction("ブックマークがありません")->setEnabled(false);
        emptyMenu.exec(showBmBtn->mapToGlobal(QPoint(0, showBmBtn->height())));
        return;
    }

    QMenu menu(this);
    for (int i = 0; i < bmArray.size(); ++i) {
        QJsonObject obj = bmArray[i].toObject();
        QString title = obj["title"].toString();
        QString url = obj["url"].toString();

        // メニューアイテム（Action）を作成
        QAction *action = menu.addAction(title);

        // ラムダ式を使って、クリックされたらそのURLを開くように紐付け
        connect(action, &QAction::triggered, this, [this, url]() {
            uriLineEdit->setText(url);
            navigateToUri();
        });
    }

    // ボタンの真下にメニューを表示
    menu.exec(showBmBtn->mapToGlobal(QPoint(0, showBmBtn->height())));
}

void MainWindow::addCurrentBookmark(){
    QString currentUrl = view->url().toString();
    QString currentTitle = view->title();

    // タイトルが空の場合はURLを代わりに使う
    if (currentTitle.isEmpty()) currentTitle = currentUrl;

    // 現在のファイルを読み込む
    QFile file(bookmarkFilePath);
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray bmArray = doc.array();

    // 重複チェック（既に同じURLがあれば追加しない）
    for (int i = 0; i < bmArray.size(); ++i) {
        if (bmArray[i].toObject()["url"].toString() == currentUrl) {
            qDebug() << "Already bookmarked:" << currentUrl;
            return;
        }
    }

    // 新しいブックマークオブジェクトを作成して追加
    QJsonObject newBm;
    newBm["title"] = currentTitle;
    newBm["url"] = currentUrl;
    bmArray.append(newBm);

    // ファイルに書き戻し保存
    doc.setArray(bmArray);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "[Bookmarked]:" << currentTitle;
    }
}

void MainWindow::initBookmarkFile(){
    QFile file(bookmarkFilePath);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            QJsonArray emptyArray;
            QJsonDocument doc(emptyArray);
            file.write(doc.toJson());
            file.close();
        }
    }
}

void MainWindow::handlePermissionRequested(QWebEnginePermission permission){
    // 要求された機能が「ポインターロック（MouseLock）」の場合
    if (permission.permissionType() == QWebEnginePermission::PermissionType::MouseLock) {
        // オブジェクトに対して直接 grant() を呼んで許可する
        permission.grant();
        qDebug() << "[Permission]: MouseLock Granted for" << permission.origin().host();
    }
}


void MainWindow::handleFullScreenRequested(QWebEngineFullScreenRequest request){
    if (request.toggleOn()) {
        // ゲームが全画面になりてぇ時
        // 上部のアドレスバーボタン等のレイアウトを非表示にする
        topBarWidget->hide();
        this->showFullScreen(); // ブラウザウィンドウ自体を全画面化
        request.accept();       // 要求を承認
    } else {
        // 全画面を解除する時
        topBarWidget->show();
        this->showNormal();     // 元のウィンドウサイズに戻す
        request.accept();
    }
}
