#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    int newArgc = argc + 2;
    char** newArgv = new char*[newArgc];
    for(int i = 0; i < argc; ++i){
        newArgv[i] = argv[i];
    }
    newArgv[argc] = const_cast<char*>("--enable-unsafe-webgpu");
    newArgv[argc + 1] = const_cast<char*>("--enable-features=WebGPU");
    QApplication a(newArgc, newArgv);
    MainWindow w;
    w.show();
    int result = a.exec();
    delete[] newArgv;
    return result;
}
