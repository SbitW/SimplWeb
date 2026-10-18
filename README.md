## SimplWeb
SimplWeb is a simple web browser based on Qt. It uses QtWebEngine, and is made in C++.<br>
SimplWebはQtで作られたシンプルなウェブブラウザです。これはQtWebEngineを使用しており、C++で作られています。<br>
## ビルド(Build)
### 前提条件(Prerequisites)
* CMake (version 3.16 or higher)
* C++ Compiler with C++17 support (GCC 8+, Clang 5+, MSVC 2017+)
* Qt6 (Required components: `Widgets`, `WebEngineWidgets`)
	* *Note for Qt installation: **QtWebEngine** is often not selected by default. Please ensure it is checked during the Qt installation process.*
	* *Qtインストール時の注意: **QtWebEngine** はデフォルトで未選択の場合があります。インストールの際に必ずチェックを入れてください。*
### 手順(Steps)
1. Download CMakeLists.txt, main.cpp, mainwindow.cpp, and mainwindow.h. Place them in the same folder.<br>
CMakeLists.txt、main.cpp、mainwindow.cpp、mainwindow.hをダウンロードし、全て同じフォルダに配置します。
2. Create a build directory and navigate into it.<br>
ビルド用のフォルダを作成し、その中に移動します。
```bash
mkdir build
cd build
```
3. Configure the project with CMake.<br>
CMakeを実行してビルド設定を生成します。
```bash
cmake ..
```
*(Note: If Qt is not found, specify the path using `-DCMAKE_PREFIX_PATH="path/to/Qt/6.x.x/your_compiler/"`)*<br>
*(補足:Qtが見つからない場合は、上記の通りにインストールパスを指定してください。)*
4. Build the application.<br>
アプリケーションをビルドします。
```bash
cmake --build .
```
## ライセンス(License)
This software is published under [GPLv3](LICENSE).<br>
このソフトウェアは[GPLv3](LICENSE)の元で公開されています。

