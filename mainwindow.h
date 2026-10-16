#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QWebEngineView>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>
#include <QWebEnginePage>
#include <QWebEnginePermission>
#include <QWebEngineFullScreenRequest>
class AdBlockInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
public:
    explicit AdBlockInterceptor(QObject *parent = nullptr) : QWebEngineUrlRequestInterceptor(parent) {}

    // すべての通信リクエストがここを通過します
    void interceptRequest(QWebEngineUrlRequestInfo &info) override {
        QString url = info.requestUrl().toString();

        // 簡易的なブロックリスト（ここにドメインやキーワードを指定）
        if (url.contains("doubleclick.net") ||
            url.contains("://googlesyndication.com") ||
            url.contains("adserver") ||
            url.contains("pagead") ||
            url.contains("yads.yahoo.co.jp") ||            // Yahooの広告など
            url.contains("adservice.google")){
            // リクエストをブロック（拒否）する
            info.block(true);
            // ターミナルにブロックログを出力
            qDebug() << "[Blocked Ad]:" << url;
        }
    }
};

class CustomWebView : public QWebEngineView{
    Q_OBJECT
public:
    explicit CustomWebView(QWidget *parent = nullptr, QMainWindow *parentWindow = nullptr)
        : QWebEngineView(parent), m_parentWindow(parentWindow) {}

protected:
    // Webサイトが「新しいウィンドウを開こうとした時」に自動で呼ばれる関数
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type) override;
private:
    QMainWindow *m_parentWindow;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private slots:
	void navigateToUri();
	void updateAddressBar(const QUrl &uri);
	void addCurrentBookmark();
	void showBookmarksMenu();
    void handlePermissionRequested(QWebEnginePermission permission);
    void handleFullScreenRequested(QWebEngineFullScreenRequest request);
private:
    QWidget *centralWidget;
    QWidget *topBarWidget;
	QLineEdit *uriLineEdit;
	CustomWebView *view;
	QPushButton *backBtn;
	QPushButton *fwdBtn;
	QPushButton *goBtn;
	QPushButton *addBmBtn;
	QPushButton *showBmBtn;
	QString bookmarkFilePath;
	AdBlockInterceptor *adBlocker;
    void initBookmarkFile();
};
#endif // MAINWINDOW_H
