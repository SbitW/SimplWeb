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
        "--disable-gpu-vsync "              // V-Syncを切る
        "--disable-frame-rate-limit "        // FPSリミッターを解除
        "--max-gum-fps=120 "                 // FPS上限を引き上げ
        "--enable-gpu-benchmarking";

    // Qt アプリケーションが起動する前に環境変数へセット
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", chromiumFlags);
    QApplication a(argc,argv);
    MainWindow w;
    w.show();
    return a.exec();
}
