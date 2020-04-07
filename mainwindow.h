#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWheelEvent>

#include "imageprocess.h"
#include "operation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
};

class MainwindowWidget : public QWidget
{
    Q_OBJECT
public:
    MainwindowWidget(QWidget *parent=0);
//    void wheelEvent(QWheelEvent *event);

    QGridLayout *mainlayout;

    InputDataWindow *inputDataW ;
    OperationWindow *operationW ;
    ShowResultWidget *showResultW;

};

#endif // MAINWINDOW_H
