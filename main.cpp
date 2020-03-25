#include <QApplication>
#include <iostream>

#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;

    MainwindowWidget w;

    std::cout<<"Hello world!"<<std::endl;

    //w.setFixedSize(1400,800); // Mac 窗口大小固定
    w.setFixedSize(1800,1000); // Ubuntu 窗口大小固定
    w.show();

    return a.exec();
}
