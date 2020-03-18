#include "ui_mainwindow.h"

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


MainwindowWidget::MainwindowWidget(QWidget *parent):QWidget(parent){

    InputDataWindow *inputDataW = new InputDataWindow(this);
    OperationWindow *operationW = new OperationWindow(this);
    ShowResultWidget *showResultW = new ShowResultWidget(this);

    QGridLayout *mainlayout = new QGridLayout(this);
    mainlayout->setColumnStretch(0, 5);
    mainlayout->setColumnStretch(1, 1);
    mainlayout->setMargin(15);
    mainlayout->setSpacing(10);
    // 左布局
    QVBoxLayout *leftlayout = new QVBoxLayout(this);
    leftlayout->addWidget(showResultW);
//    showResultW->resize(0,0);

    // 右布局
    QVBoxLayout *rightlayout = new QVBoxLayout(this);
    rightlayout->setStretchFactor(inputDataW, 5);
    rightlayout->setStretchFactor(operationW, 1);
    rightlayout->addWidget(inputDataW,0,0);
    rightlayout->addWidget(operationW,1,0);

    // 主布局

    mainlayout->addLayout(leftlayout,0,0);
    mainlayout->addLayout(rightlayout,0,1);
    setLayout(mainlayout);


    // InputDataWindow to ShowResultWidget
    // 传雷达点给图像窗口
    qRegisterMetaType<std::vector<float>>("inputdata");
    connect(inputDataW, SIGNAL(SendData_lidar1(std::vector<float>)),
            showResultW, SLOT(ReceiveData_lidar1(std::vector<float>)));
    connect(inputDataW, SIGNAL(SendData_lidar2(std::vector<float>)),
            showResultW, SLOT(ReceiveData_lidar2(std::vector<float>)));
    // 清除图像
    connect(inputDataW, SIGNAL(command_clear()), showResultW , SLOT(ClearImage()) );
    // 数据清除命令，画图按钮全部失能
    connect(inputDataW, SIGNAL(command_clear()), showResultW , SLOT(DisableButton()) );
    connect(inputDataW, SIGNAL(command_enablebutton()), showResultW , SLOT(EnableButton()) );


    // InputDataWindow to OperationWindow
    // 接受雷达数据
    connect(inputDataW, SIGNAL(SendData_lidar1(std::vector<float>)),
                operationW , SLOT(ReceiveInput_lidar1()) );
    connect(inputDataW, SIGNAL(SendData_lidar2(std::vector<float>)),
                operationW , SLOT(ReceiveInput_lidar2()) );

    // 外参初始化
    connect(inputDataW, SIGNAL(command_initialExtrinsic()), operationW , SLOT(InitialExtrinsic()) );
    // 画图命令
    connect(inputDataW, SIGNAL(command_draw()), operationW , SLOT(DrawData()) );
    // 清除数据后，把外参按钮失能
    connect(inputDataW, SIGNAL(command_clear()), operationW , SLOT(DisableButton()) );
    // 使能按钮
    connect(inputDataW, SIGNAL(command_enablebutton()), operationW , SLOT(EnableButton()) );


    // OperationWindow to ShowResultWindow
    // 画图是由operationWindow向ShowResultWindow发出的命令
    connect(operationW, SIGNAL(command_draw(double, double, double, double, double, double)),
            showResultW , SLOT(draw(double, double, double, double, double, double)) );

}
void MainwindowWidget::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    std::cout<<numDegrees<<std::endl;
    std::cout<<numSteps<<std::endl;
}
