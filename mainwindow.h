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
#include <QDebug>
class AdBlockInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
public:
    explicit AdBlockInterceptor(QObject *parent = nullptr)
        : QWebEngineUrlRequestInterceptor(parent)
    {
        // ブロックしたいキーワードやドメインのリスト
        m_blackList = {
            "doubleclick.net",
            "googlesyndication.com",
            "adservice.google",
            "pagead",
            "adserver",
            "yads.yahoo.co.jp",
            "adchoices",
            "adchoice",
            "criteo.com",
            "criteo.net",
            "amazon-adsystem.com",
            "adnxs.com",
            "rubiconproject.com",
            "pubmatic.com",
            "microad.jp",
            "fluct.jp"
        };
    }

    void interceptRequest(QWebEngineUrlRequestInfo &info) override {
        QString uriStr = info.requestUrl().toString().toLower(); // 表記揺れ対策で小文字化

        // リスト内のいずれかのキーワードが含まれているかチェック
        for (const QString &keyword : m_blackList) {
            if (uriStr.contains(keyword)) {
                info.block(true);
                qDebug() << "[Blocked Ad]:" << uriStr;
                return; // ブロックしたら即終了
            }
        }
    }

private:
    QStringList m_blackList;
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
    QPushButton *rldBtn;
	QPushButton *goBtn;
	QPushButton *addBmBtn;
	QPushButton *showBmBtn;
	QString bookmarkFilePath;
	AdBlockInterceptor *adBlocker;
    void initBookmarkFile();
};
#endif // MAINWINDOW_H
