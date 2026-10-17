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
        //"--disable-frame-rate-limit "        // FPSリミッターを解除
        "--max-gum-fps=120";                 // FPS上限を引き上げ

    // Qt アプリケーションが起動する前に環境変数へセット
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", chromiumFlags);
    QApplication a(argc,argv);
    MainWindow w;
    w.show();
    return a.exec();
}
