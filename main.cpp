#include <QApplication>
#include <iostream>

#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;

    MainwindowWidget w;

    std::cout<<"Hello world!"<<std::endl;

    //w.resize(1400,1000); // 窗口大小可以调整
    w.setFixedSize(1400,800); // 窗口大小固定
    w.show();

    return a.exec();
}
