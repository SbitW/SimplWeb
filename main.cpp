#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QByteArray chromiumFlags =
        "--enable-unsafe-webgpu "
        "--enable-features=WebGPU "
        "--ignore-gpu-blocklist "
        "--enable-gpu-rasterization "
        "--all-egl-on-gpu "
        "--disable-software-rasterizer "
        "--disable-gpu-vsync "              // V-Sync(60Hz同期)を切る
        "--disable-frame-rate-limit "        // リミッターを解除
        "--max-gum-fps=120 "                 // 上限を引き上げ
        "--enable-gpu-benchmarking";

    // Qt アプリケーションが起動する前に環境変数へセット（これが必須です）
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", chromiumFlags);
    QApplication a(argc,argv);
    MainWindow w;
    w.show();
    return a.exec();
}
