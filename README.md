## SimplWeb
SimplWebはQtで作られたシンプルなウェブブラウザです。これはQtWebEngineを使用しており、C++で作られています。<br>
SimplWeb is a simple web browser based on Qt. It uses QtWebEngine, and is made in C++.<br>
## 特徴(Features)
* 組み込み広告ブロッカー(Built-in Ad Blocker)
	* 広告ドメインとの通信を遮断する広告ブロッカーを内蔵しています。
	* Intercepts and blocks advertising/tracking domains.
* 全画面表示・ポインターロック対応(Full-screen and pointerlock support)
	* 全画面表示の要求やポインターロックを自動的に承認し、SimplWebでのゲーミングを可能にします。
	* Seamlessly handles full-screen requests and auto-grants pointerlock permissions, enabling gaming with SimplWeb.
* WebGPU・GPUアクセラレーション有効(WebGPU and GPU Acceleration enabled)
	* SimplWebは実験的なWebGPUをサポートし、またGPUラスタライズを強制します。最新のWebアプリやゲームに最適化されています。 
	* SimplWeb supports experimental WebGPU and forces GPU rasterization, and is optimised for modern Web apps and games.
## ビルド(Build)
### 前提条件(Prerequisites)
* CMake (バージョン3.16以上 | version 3.16 or higher)
* C++17をサポートするC++コンパイラ (C++ Compiler with C++17 support: GCC 8+, Clang 5+, MSVC 2017+)
* Qt6 (必要なコンポーネント | Required Components: `Widgets`, `WebEngineWidgets`)
	* *Qtインストール時の注意: **QtWebEngine** はデフォルトで未選択の場合があります。インストールの際に必ずチェックを入れてください。*
	* *Note for Qt installation: **QtWebEngine** is often not selected by default. Please ensure it is checked during the Qt installation process.*
### 手順(Steps)
1. CMakeLists.txt、main.cpp、mainwindow.cpp、mainwindow.hをダウンロードし、全て同じフォルダに配置します。<br>
Download CMakeLists.txt, main.cpp, mainwindow.cpp, and mainwindow.h. Place them in the same folder.
2. ビルド用のフォルダを作成し、その中に移動します。<br>
Create a build directory and navigate into it.
```bash
mkdir build
cd build
```
3. CMakeを実行してビルド設定を生成します。<br>
Configure the project with CMake.
```bash
cmake ..
```
*(補足:Qtが見つからない場合は、`-DCMAKE_PREFIX_PATH="path/to/Qt/6.x.x/your_compiler/"`を用いてインストールパスを指定してください。)*<br>
*(Note: If Qt is not found, specify the path as shown above.)*<br>
4. アプリケーションをビルドします。<br>
Build the application.
```bash
cmake --build .
```
## ライセンス(License)
このソフトウェアは[GPLv3](LICENSE)の元で公開されています。<br>
This software is published under [GPLv3](LICENSE).

