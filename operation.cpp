#include "operation.h"
// InputDataWindow构造函数

std::vector<float> range1_online;
std::vector<float> range2_online;
InputDataWindow::InputDataWindow(QWidget *parent):QWidget(parent){


    mParticipant.Create("calib",101);
    subscriber_lidar1.Create(mParticipant, "scan_1", 1, InputDataWindow::ReceiveMessage_fromlidar1);
    subscriber_lidar2.Create(mParticipant, "scan_2", 1, InputDataWindow::ReceiveMessage_fromlidar2);

    data_online = new QRadioButton(tr("在线"),this);
    data_offline = new QRadioButton(tr("离线"),this);
    inputData_lidar1 = new QPushButton(tr("加载雷达1数据"),this);  
    inputData_lidar2 = new QPushButton(tr("加载雷达2数据"),this);
    initial_extrinsic = new QPushButton(tr("设置默认外参数"),this);
    draw_data = new QPushButton(tr("画图 / 图片复位"),this);
    clear_data = new QPushButton(tr("清除数据及图像"),this);
    write_calibfile = new QPushButton(tr("输出外参标定结果"),this);
    inputData_lidar1path = new QLabel(tr("雷达1文件路径: "),this);
    inputData_lidar1path_now = new QLabel(tr("无文件"),this);
    inputData_lidar1path_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    inputData_lidar2path = new QLabel(tr("雷达2文件路径: "),this);
    inputData_lidar2path_now = new QLabel(tr("无文件"),this);
    inputData_lidar2path_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    inputData_lidar2path_now->setEnabled(false);
    inputData_lidar1path_now->setEnabled(false);
    initial_extrinsic->setEnabled(false);
    draw_data->setEnabled(false);
    clear_data->setEnabled(false);
    write_calibfile->setEnabled(false);

    data_offline->setChecked(true);


    // 是否加载了雷达1和雷达2
    have_lidar1 = false;
    have_lidar2 = false;

    // 主布局
    inputDatalayout = new QVBoxLayout(this);
    sourcelayout = new QHBoxLayout(this);
    data_source = new QButtonGroup(this);
    data_source->addButton(data_online,0);
    data_source->addButton(data_offline,1);
    sourcelayout->addWidget(data_online);
    sourcelayout->addWidget(data_offline);
    inputDatalayout->addLayout(sourcelayout);
    inputDatalayout->addWidget(inputData_lidar1);
    inputDatalayout->addWidget(inputData_lidar2);
    inputDatalayout->addWidget(initial_extrinsic);
    inputDatalayout->addWidget(draw_data);
    inputDatalayout->addWidget(clear_data);
    inputDatalayout->addWidget(write_calibfile);
    pathlayout = new QGridLayout(this);
    pathlayout->addWidget(inputData_lidar1path,0,0);
    pathlayout->addWidget(inputData_lidar1path_now,0,1);
    pathlayout->addWidget(inputData_lidar2path,1,0);
    pathlayout->addWidget(inputData_lidar2path_now,1,1);
    inputDatalayout->addLayout(pathlayout);


    connect(inputData_lidar1 , SIGNAL(clicked()) , this , SLOT(InputDataLidar1()) );
    connect(inputData_lidar2 , SIGNAL(clicked()) , this , SLOT(InputDataLidar2()) );
    connect(initial_extrinsic , SIGNAL(clicked()), this , SLOT(InitialExtrinsic()) );
    connect(draw_data, SIGNAL(clicked()) , this , SLOT(DrawDataByButton()) );
    connect(clear_data, SIGNAL(clicked()) , this , SLOT(ClearData()) );
    connect(write_calibfile, SIGNAL(clicked()) , this , SLOT(WriteCalibFile()) );
    connect(this, SIGNAL(command_enablebutton()) , this , SLOT(EnableButton()) );


    tmr1 = new QTimer(this);
    connect(tmr1, SIGNAL(timeout()), this, SLOT(UpdateLidar1()));
    connect(tmr1, SIGNAL(timeout()), this, SLOT(DrawDataByTimer()));
    tmr2 = new QTimer(this);
    connect(tmr2, SIGNAL(timeout()), this, SLOT(UpdateLidar2()));
    connect(tmr2, SIGNAL(timeout()), this, SLOT(DrawDataByTimer()));
}
void InputDataWindow::InputDataLidar1(){
    // 按钮加载
//    QString path1 = QFileDialog::getOpenFileName(this,"open","../","TXT(*.txt)");
//    std::string file1 = path1.toStdString();
//    std::ifstream myfile_1(file1);
//    inputData_lidar1path_now->setEnabled(true);
//    inputData_lidar1path_now->setText(path1.right(20));

    int data_id = data_source->checkedId();
    data_online->setEnabled(false);
    data_offline->setEnabled(false);
    // 离线数据
    if (data_id == 1){

        // 固定路径
        std::ifstream myfile_1((getenv("HOME") +path_lidar1).c_str());
        inputData_lidar1path_now->setEnabled(true);
        inputData_lidar1path_now->setText("lidar1.txt");

        for(int i=0;i<1080;i++)
        {
            myfile_1>>lidar1_angle[i]>>lidar1_range[i];
            range1.push_back(lidar1_range[i]);
        }
        myfile_1.close();

        inputData_lidar1->setEnabled(false);

        emit SendData_lidar1(false, range1);
        emit SendStatus_lidar1(false);

        // 发送使能信号，会将整个窗口的按钮全部使能
        have_lidar1 = true;
        if (have_lidar1 == true && have_lidar2 == true){
            emit command_enablebutton();
        }
    }
    // 在线数据
    if (data_id == 0){
        range2.clear();
        SendData_lidar2(true, range2);

        tmr1->start(10);
        emit SendStatus_lidar1(true);

        inputData_lidar1->setEnabled(false);
        have_lidar1 = true;
        if (have_lidar1 == true && have_lidar2 == true){
            emit command_enablebutton();
        }
    }
}
void InputDataWindow::InputDataLidar2(){
//    QString path2 = QFileDialog::getOpenFileName(this,"open","../","TXT(*.txt)");
//    std::string file2 = path2.toStdString();
//    std::ifstream myfile_2(file2);
//    inputData_lidar2path_now->setEnabled(true);
//    inputData_lidar2path_now->setText(path2.right(20));

    int data_id = data_source->checkedId();
    data_online->setEnabled(false);
    data_offline->setEnabled(false);
    // 离线数据
    if (data_id == 1){     
        std::ifstream myfile_2((getenv("HOME") +path_lidar2).c_str());
        inputData_lidar2path_now->setEnabled(true);
        inputData_lidar2path_now->setText("lidar2.txt");

        for(int i=0;i<1080;i++)
        {
            myfile_2>>lidar2_angle[i]>>lidar2_range[i];
            range2.push_back(lidar2_range[i]);
        }
        myfile_2.close();

        inputData_lidar2->setEnabled(false);

        emit SendData_lidar2(false, range2);
        emit SendStatus_lidar2(false);

        have_lidar2 = true;
        if (have_lidar1 == true && have_lidar2 == true){
            emit command_enablebutton();
        }
    }
    // 在线数据
    if (data_id == 0){
        range1.clear();
        SendData_lidar2(true, range1);

        tmr2->start(10);
        emit SendStatus_lidar2(true);

        inputData_lidar2->setEnabled(false);
        have_lidar2 = true;
        if (have_lidar1 == true && have_lidar2 == true){
            emit command_enablebutton();
        }
    }
}
void InputDataWindow::ReceiveMessage_fromlidar1(commander_robot_msg::LaserScan *message){
    range1_online= message->ranges();
}
void InputDataWindow::ReceiveMessage_fromlidar2(commander_robot_msg::LaserScan *message){
    range2_online= message->ranges();
}
void InputDataWindow::UpdateLidar1(){

    emit SendData_lidar1(true, range1_online);

}
void InputDataWindow::UpdateLidar2(){
    emit SendData_lidar2(true, range2_online);

}

void InputDataWindow::InitialExtrinsic(){
    emit(command_initialExtrinsic());
}
void InputDataWindow::DrawDataByButton(){
    emit command_draw_byButton();
}
void InputDataWindow::DrawDataByTimer(){
    emit command_draw_byTimer();
}
void InputDataWindow::ClearData(){
    range1.clear();
    range2.clear();

    tmr1->stop();
    tmr2->stop();

    data_online->setEnabled(true);
    data_offline->setEnabled(true);
    inputData_lidar1->setEnabled(true);
    inputData_lidar2->setEnabled(true);


    have_lidar1 = false;
    have_lidar2 = false;
    initial_extrinsic->setEnabled(false);
    draw_data->setEnabled(false);
    clear_data->setEnabled(false);
    inputData_lidar1path_now->setEnabled(false);
    inputData_lidar2path_now->setEnabled(false);

    emit(command_clear());
}
void InputDataWindow::WriteCalibFile(){

}

void InputDataWindow::EnableButton(){
    initial_extrinsic->setEnabled(true);
    draw_data->setEnabled(true);
    clear_data->setEnabled(true);
}

OperationWindow::OperationWindow(QWidget *parent):QWidget(parent){
    QGridLayout *operationlayout = new QGridLayout(this);

    command_record = new QTextBrowser(this);
    command_row = 0;


    lidar_show = new QButtonGroup(this);
    lidarAll_show = new QRadioButton(tr("显示全部雷达数据"),this);
    lidar1_show = new QRadioButton(tr("只显示雷达1"),this);
    lidar2_show = new QRadioButton(tr("只显示雷达2"),this);

    lidar_show->addButton(lidarAll_show,0);
    lidar_show->addButton(lidar1_show,1);
//    powerOnBtn->setIcon(QIcon(":/images/power_on.png"));
    lidar_show->addButton(lidar2_show,2);
    lidarAll_show->setChecked(true);


    lidar1_increment = new QLabel(tr("雷达1角度分辨率:"),this);
    lidar1_increment_now = new QLabel(tr("/"),this);
    lidar1_increment_set = new QPushButton(tr("set"),this);
    lidar1_increment_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    lidar2_increment = new QLabel(tr("雷达2角度分辨率:"),this);
    lidar2_increment_now = new QLabel(tr("/"),this);
    lidar2_increment_set = new QPushButton(tr("set"),this);
    lidar2_increment_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    extrinsic_x = new QLabel(tr("x:"),this);
    extrinsic_x_now = new QLabel(tr("/"),this);
    x_set = new QPushButton(tr("set"),this);
    x_subtract = new QPushButton(tr("-"),this);
    x_add = new QPushButton(tr("+"),this);
    extrinsic_x->setAlignment(Qt::AlignCenter);
    extrinsic_x_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    extrinsic_y = new QLabel(tr("y:"),this);
    extrinsic_y_now = new QLabel(tr("/"),this);
    y_set = new QPushButton(tr("set"),this);
    y_subtract = new QPushButton(tr("-"),this);
    y_add = new QPushButton(tr("+"),this);
    extrinsic_y->setAlignment(Qt::AlignCenter);
    extrinsic_y_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    extrinsic_theta = new QLabel(tr("theta:"),this);
    extrinsic_theta_now = new QLabel(tr("/"),this);
    theta_set = new QPushButton(tr("set"),this);
    theta_subtract = new QPushButton(tr("-"),this);
    theta_add = new QPushButton(tr("+"),this);
    extrinsic_theta->setAlignment(Qt::AlignCenter);
    extrinsic_theta_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    dx = new QLabel(tr("dx:"),this);
    dx_now = new QLabel(tr("/"),this);
    dx_set = new QPushButton(tr("set"),this);
    dx_subtract = new QPushButton(tr("-"),this);
    dx_add = new QPushButton(tr("+"),this);
    dx->setAlignment(Qt::AlignCenter);
    dx_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    dy = new QLabel(tr("dy:"),this);
    dy_now = new QLabel(tr("/"),this);
    dy_set = new QPushButton(tr("set"),this);
    dy_subtract = new QPushButton(tr("-"),this);
    dy_add = new QPushButton(tr("+"),this);
    dy->setAlignment(Qt::AlignCenter);
    dy_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    dtheta = new QLabel(tr("dtheta:"),this);
    dtheta_now = new QLabel(tr("/"),this);
    dtheta_set = new QPushButton(tr("set"),this);
    dtheta_subtract = new QPushButton(tr("-"),this);
    dtheta_add = new QPushButton(tr("+"),this);
    dtheta->setAlignment(Qt::AlignCenter);
    dtheta_now->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    DisableButton();

    // 文字居中
//    extrinsic_x->setAlignment(Qt::AlignCenter);

//    extrinsic_x->setFixedWidth(2);
//    extrinsic_x->adjustSize();

//    extrinsic_y->setFixedWidth(4);
//    extrinsic_theta->setFixedWidth(4);
//    dx->setFixedWidth(4);
//    dy->setFixedWidth(4);
//    dtheta->setFixedWidth(4);

//     extrinsic_x->adjustSize();
//    extrinsic_x->setFixedSize(0,0);


//    x_subtract->setFixedSize(60,20);
//    y_subtract->setFixedSize(60,20);
//    theta_subtract->setFixedSize(60,20);
//    x_add->setFixedSize(60,20);
//    y_add->setFixedSize(60,20);
//    theta_add->setFixedSize(60,20);
//    dx_subtract->setFixedSize(60,20);
//    dy_subtract->setFixedSize(60,20);
//    dtheta_subtract->setFixedSize(60,20);
//    dx_add->setFixedSize(60,20);
//    dy_add->setFixedSize(60,20);
//    dtheta_add->setFixedSize(60,20);


//    x_add->setGeometry(0,0,400,200);

//    QHBoxLayout *h = new QHBoxLayout;

//    h->addWidget(lidarAll_show);
//    h->addWidget(lidar1_show);
//    h->addWidget(lidar2_show);

//    operationlayout->addLayout(h,0,0);

    operationlayout->addWidget(lidarAll_show,0,0);
    operationlayout->addWidget(lidar1_show,2,3,1,2);
    operationlayout->addWidget(lidar2_show,3,3,1,2);

    operationlayout->addWidget(lidar1_increment,2,0);
    operationlayout->addWidget(lidar1_increment_now,2,1);
    operationlayout->addWidget(lidar1_increment_set,2,2);
    operationlayout->addWidget(lidar2_increment,3,0);
    operationlayout->addWidget(lidar2_increment_now,3,1);
    operationlayout->addWidget(lidar2_increment_set,3,2);

    operationlayout->addWidget(extrinsic_x,4,0);
    operationlayout->addWidget(extrinsic_x_now,4,1);
    operationlayout->addWidget(x_set,4,2);
    operationlayout->addWidget(x_subtract,4,3);
    operationlayout->addWidget(x_add,4,4);

    operationlayout->addWidget(extrinsic_y,5,0);
    operationlayout->addWidget(extrinsic_y_now,5,1);
    operationlayout->addWidget(y_set,5,2);
    operationlayout->addWidget(y_subtract,5,3);
    operationlayout->addWidget(y_add,5,4);

    operationlayout->addWidget(extrinsic_theta,6,0);
    operationlayout->addWidget(extrinsic_theta_now,6,1);
    operationlayout->addWidget(theta_set,6,2);
    operationlayout->addWidget(theta_subtract,6,3);
    operationlayout->addWidget(theta_add,6,4);

    operationlayout->addWidget(dx,7,0);
    operationlayout->addWidget(dx_now,7,1);
    operationlayout->addWidget(dx_set,7,2);
    operationlayout->addWidget(dx_subtract,7,3);
    operationlayout->addWidget(dx_add,7,4);

    operationlayout->addWidget(dy,8,0);
    operationlayout->addWidget(dy_now,8,1);
    operationlayout->addWidget(dy_set,8,2);
    operationlayout->addWidget(dy_subtract,8,3);
    operationlayout->addWidget(dy_add,8,4);

    operationlayout->addWidget(dtheta,9,0);
    operationlayout->addWidget(dtheta_now,9,1);
    operationlayout->addWidget(dtheta_set,9,2);
    operationlayout->addWidget(dtheta_subtract,9,3);
    operationlayout->addWidget(dtheta_add,9,4);

    operationlayout->addWidget(command_record,10,0,1,0);

//    extrinsic_x->setFixedWidth(1);
//    extrinsic_y->setFixedWidth(1);
//    extrinsic_theta->setFixedWidth(1);
//    dx->setFixedWidth(1);
//    dy->setFixedWidth(1);
//    dtheta->setFixedWidth(1);

//    x_set->setFixedSize(60,20);



//    x_subtract->setStyleSheet("background-color: rgb(255, 255, 255);");
//    y_subtract->setStyleSheet("background-color: rgb(255, 255, 255);");
//    theta_subtract->setStyleSheet("background-color: rgb(255, 255, 255);");
//    x_add->setStyleSheet("background-color: rgb(255, 255, 255);");
//    y_add->setStyleSheet("background-color: rgb(255, 255, 255);");
//    theta_add->setStyleSheet("background-color: rgb(255, 255, 255);");
//    dx_subtract->setStyleSheet("background-color: rgb(255, 255, 255);");
//    dy_subtract->setStyleSheet("background-color: rgb(255, 255, 255);");
//    dtheta_subtract->setStyleSheet("background-color: rgb(255, 255, 255);");
//    dx_add->setStyleSheet("background-color: rgb(255, 255, 255);");
//    dy_add->setStyleSheet("background-color: rgb(255, 255, 255);");
//    dtheta_add->setStyleSheet("background-color: rgb(255, 255, 255);");

//    x_subtract->setStyleSheet( "QPushButton:pressed{""background-color:rgb(255,255,0);");

//    lidarAll_show->setFixedWidth(100);
//    lidar1_show->setFixedWidth(100);
//    lidar2_show->setFixedWidth(100);

    lidar1_increment_now->setFixedWidth(70);
    lidar2_increment_now->setFixedWidth(70);
    extrinsic_x_now->setFixedWidth(70);
    extrinsic_y_now->setFixedWidth(70);
    extrinsic_theta_now->setFixedWidth(70);
    dx_now->setFixedWidth(70);
    dy_now->setFixedWidth(70);
    dtheta_now->setFixedWidth(70);

    lidar1_increment_set->setFixedWidth(90);
    lidar2_increment_set->setFixedWidth(90);
    x_set->setFixedWidth(90);
    y_set->setFixedWidth(90);
    theta_set->setFixedWidth(90);
    dx_set->setFixedWidth(90);
    dy_set->setFixedWidth(90);
    dtheta_set->setFixedWidth(90);

    connect(lidar_show, SIGNAL(buttonClicked(int)), this, SLOT(DrawWhichLidar()));
    connect(lidar1_increment_set , SIGNAL(clicked()) , this , SLOT(SetIncrement_lidar1()) );
    connect(lidar2_increment_set , SIGNAL(clicked()) , this , SLOT(SetIncrement_lidar2()) );
    connect(x_set , SIGNAL(clicked()) , this , SLOT(SetX()) );
    connect(x_subtract , SIGNAL(clicked()) , this , SLOT(SubtractX()) );
    connect(x_add , SIGNAL(clicked()) , this , SLOT(AddX()) );
    connect(x_subtract , SIGNAL(clicked()) , this , SLOT(DrawData()) );
    connect(x_add , SIGNAL(clicked()) , this , SLOT(DrawData()) );
    connect(y_set , SIGNAL(clicked()) , this , SLOT(SetY()) );
    connect(y_subtract , SIGNAL(clicked()) , this , SLOT(SubtractY()) );
    connect(y_add , SIGNAL(clicked()) , this , SLOT(AddY()) );
    connect(y_subtract , SIGNAL(clicked()) , this , SLOT(DrawData()) );
    connect(y_add , SIGNAL(clicked()) , this , SLOT(DrawData()) );
    connect(theta_set , SIGNAL(clicked()) , this , SLOT(SetTheta()) );
    connect(theta_subtract , SIGNAL(clicked()) , this , SLOT(SubtractTheta()) );
    connect(theta_add , SIGNAL(clicked()) , this , SLOT(AddTheta()) );
    connect(theta_subtract , SIGNAL(clicked()) , this , SLOT(DrawData()) );
    connect(theta_add , SIGNAL(clicked()) , this , SLOT(DrawData()) );
    connect(dx_set , SIGNAL(clicked()) , this , SLOT(SetDX()) );
    connect(dx_subtract , SIGNAL(clicked()) , this , SLOT(SubtractDX()) );
    connect(dx_add , SIGNAL(clicked()) , this , SLOT(AddDX()) );
    connect(dy_set , SIGNAL(clicked()) , this , SLOT(SetDY()) );
    connect(dy_subtract , SIGNAL(clicked()) , this , SLOT(SubtractDY()) );
    connect(dy_add , SIGNAL(clicked()) , this , SLOT(AddDY()) );
    connect(dtheta_set , SIGNAL(clicked()) , this , SLOT(SetDTheta()) );
    connect(dtheta_subtract , SIGNAL(clicked()) , this , SLOT(SubtractDTheta()) );
    connect(dtheta_add , SIGNAL(clicked()) , this , SLOT(AddDTheta()) );
}

void OperationWindow::DrawWhichLidar(){
    int id = lidar_show->checkedId();
    command_row++;
    if (id == 0){
        command_record->insertPlainText(tr("显示两个雷达图像，"));
    }
    else if (id ==1){
        command_record->insertPlainText(tr("只显示雷达1图像，"));
    }
    else if (id ==2){
        command_record->insertPlainText(tr("只显示雷达2图像，"));
    }
    command_record->moveCursor(QTextCursor::NextRow);

    DrawData("Button");
}
// 画图按钮被触发
void OperationWindow::DrawDataByButton(){
    DrawData("Button");
}
void OperationWindow::DrawDataByTimer(){
    DrawData("Timer");
}
void OperationWindow::DrawData(std::string way){
     if (way == "Button"){
        command_row++;
        command_record->insertPlainText(tr("画图\n"));
        command_record->moveCursor(QTextCursor::NextRow);
     }

    int draw_id = lidar_show->checkedId();
    double draw_increment1 = this->lidar1_increment_now->text().toDouble();
    double draw_increment2 = this->lidar2_increment_now->text().toDouble();
    double draw_x = this->extrinsic_x_now->text().toDouble();
    double draw_y = this->extrinsic_y_now->text().toDouble();
    double draw_theta = this->extrinsic_theta_now->text().toDouble();

    if (draw_x!=0 && draw_y!= 0 && draw_theta!=0 && draw_increment1!= 0 && draw_increment2!=0 ){
        emit command_draw(way, draw_id, draw_increment1, draw_increment2, draw_theta, draw_x, draw_y);
    }
    else{
        if (way == "Button"){
            command_row++;
            command_record->insertPlainText(tr("无法画图！输入外参不合法\n"));
            command_record->moveCursor(QTextCursor::NextRow);
        }
    }

}
void OperationWindow::InitialExtrinsic(){
    command_row++;
    command_record->insertPlainText(tr("设置默认外参数\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    lidar1_increment_now->setText("0.25");
    lidar2_increment_now->setText("0.25");
    extrinsic_x_now->setText("-0.90");
    extrinsic_y_now->setText("-0.18");
    extrinsic_theta_now->setText("180.0");
    dx_now->setText("0.01");
    dy_now->setText("0.01");
    dtheta_now->setText("0.1");
}

void OperationWindow::ReceiveStatus_lidar1(bool online){
    command_row++;
    if (online){
        command_record->insertPlainText(tr("雷达1已连接\n"));
    }
    else{
        command_record->insertPlainText(tr("已加载雷达1的数据\n"));
    }
    command_record->moveCursor(QTextCursor::NextRow);
}
void OperationWindow::ReceiveStatus_lidar2(bool online){
    command_row++;
    if (online){
        command_record->insertPlainText(tr("雷达2已连接\n"));
    }
    else{
        command_record->insertPlainText(tr("已加载雷达1的数据\n"));
    }
    command_record->moveCursor(QTextCursor::NextRow);
}

void OperationWindow::EnableButton(){
    lidarAll_show->setEnabled(true);
    lidar1_show->setEnabled(true);
    lidar2_show->setEnabled(true);
    lidar1_increment_set->setEnabled(true);
    lidar2_increment_set->setEnabled(true);
    x_set->setEnabled(true);
    x_subtract->setEnabled(true);
    x_add->setEnabled(true);
    y_set->setEnabled(true);
    y_subtract->setEnabled(true);
    y_add->setEnabled(true);
    theta_set->setEnabled(true);
    theta_subtract->setEnabled(true);
    theta_add->setEnabled(true);
    dx_set->setEnabled(true);
    dx_subtract->setEnabled(true);
    dx_add->setEnabled(true);
    dy_set->setEnabled(true);
    dy_subtract->setEnabled(true);
    dy_add->setEnabled(true);
    dtheta_set->setEnabled(true);
    dtheta_subtract->setEnabled(true);
    dtheta_add->setEnabled(true);
}
void OperationWindow::DisableButton(){
    if (command_row == 0){
        command_record->insertPlainText(tr("当前没有任何雷达数据，加载数据后才能操作\n"));
    }
    else{
        command_record->insertPlainText(tr("已清除全部数据\n"));
    }
    command_row++;
    command_record->moveCursor(QTextCursor::NextRow);

    lidarAll_show->setEnabled(false);
    lidar1_show->setEnabled(false);
    lidar2_show->setEnabled(false);
    lidar1_increment_set->setEnabled(false);
    lidar2_increment_set->setEnabled(false);
    x_set->setEnabled(false);
    x_subtract->setEnabled(false);
    x_add->setEnabled(false);
    y_set->setEnabled(false);
    y_subtract->setEnabled(false);
    y_add->setEnabled(false);
    theta_set->setEnabled(false);
    theta_subtract->setEnabled(false);
    theta_add->setEnabled(false);
    dx_set->setEnabled(false);
    dx_subtract->setEnabled(false);
    dx_add->setEnabled(false);
    dy_set->setEnabled(false);
    dy_subtract->setEnabled(false);
    dy_add->setEnabled(false);
    dtheta_set->setEnabled(false);
    dtheta_subtract->setEnabled(false);
    dtheta_add->setEnabled(false);
}

// 以下就是具体每个按钮的回调函数
// x、y、theta的大小和它们的步进值设定

void OperationWindow::SetIncrement_lidar1(){
    command_row++;
    command_record->insertPlainText(tr("雷达 1 角度分辨率设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_increment = QInputDialog::getText(this,tr("雷达1角度分辨率"),tr("请输入雷达 1 角度分辨率："),QLineEdit::Normal,lidar1_increment_now->text(),&ok);
    if (ok && !new_increment.isEmpty())
        lidar1_increment_now->setText(new_increment);
}
void OperationWindow::SetIncrement_lidar2(){
    command_row++;
    command_record->insertPlainText(tr("雷达 2 角度分辨率设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_increment = QInputDialog::getText(this,tr("雷达 2 角度分辨率"),tr("请输入雷达 2 角度分辨率："),QLineEdit::Normal,lidar2_increment_now->text(),&ok);
    if (ok && !new_increment.isEmpty())
        lidar2_increment_now->setText(new_increment);
}
void OperationWindow::SetX(){
    command_row++;
    command_record->insertPlainText(tr("X 方向设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_x = QInputDialog::getText(this,tr("外参：X"),tr("请输入 X 方向外参："),QLineEdit::Normal,extrinsic_x_now->text(),&ok);
    if (ok && !new_x.isEmpty())
        extrinsic_x_now->setText(new_x);
}
void OperationWindow::SubtractX(){
    command_row++;
    command_record->insertPlainText(tr("X 方向减\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = extrinsic_x_now->text().toDouble();
    double dtmp = dx_now->text().toDouble();
    QString afterchange = QString::number( tmp - dtmp,'f',3);
    extrinsic_x_now->setText(afterchange);
}
void OperationWindow::AddX(){
    command_row++;
    command_record->insertPlainText(tr("X 方向加\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = extrinsic_x_now->text().toDouble();
    double dtmp = dx_now->text().toDouble();
    QString afterchange = QString::number( tmp + dtmp,'f',3);
    extrinsic_x_now->setText(afterchange);
}

void OperationWindow::SetY(){
    command_row++;
    command_record->insertPlainText(tr("Y 方向设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_y = QInputDialog::getText(this,tr("外参：Y"),tr("请输入 Y 方向外参："),QLineEdit::Normal,extrinsic_y_now->text(),&ok);
    if (ok && !new_y.isEmpty())
        extrinsic_y_now->setText(new_y);
}
void OperationWindow::SubtractY(){
    command_row++;
    command_record->insertPlainText(tr("Y 方向减\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = extrinsic_y_now->text().toDouble();
    double dtmp = dy_now->text().toDouble();
    QString afterchange = QString::number( tmp - dtmp,'f',3);
    extrinsic_y_now->setText(afterchange);
}
void OperationWindow::AddY(){
    command_row++;
    command_record->insertPlainText(tr("Y 方向加\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = extrinsic_y_now->text().toDouble();
    double dtmp = dy_now->text().toDouble();
    QString afterchange = QString::number( tmp + dtmp,'f',3);
    extrinsic_y_now->setText(afterchange);
}

void OperationWindow::SetTheta(){
    command_row++;
    command_record->insertPlainText(tr("Theta 角度设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_theta = QInputDialog::getText(this,tr("外参：Theta"),tr("请输入 Theta 角度外参："),QLineEdit::Normal,extrinsic_theta_now->text(),&ok);
    if (ok && !new_theta.isEmpty())
        extrinsic_theta_now->setText(new_theta);

}
void OperationWindow::SubtractTheta(){
    command_row++;
    command_record->insertPlainText(tr("Theta 角度减\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = extrinsic_theta_now->text().toDouble();
    double dtmp = dtheta_now->text().toDouble();
    QString afterchange = QString::number( tmp - dtmp,'f',3);
    extrinsic_theta_now->setText(afterchange);
}
void OperationWindow::AddTheta(){
    command_row++;
    command_record->insertPlainText(tr("Theta 角度加\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = extrinsic_theta_now->text().toDouble();
    double dtmp = dtheta_now->text().toDouble();
    QString afterchange = QString::number( tmp + dtmp,'f',3);
    extrinsic_theta_now->setText(afterchange);
}

void OperationWindow::SetDX(){
    command_row++;
    command_record->insertPlainText(tr("dx 大小设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_dx = QInputDialog::getText(this,tr("增量：dx"),tr("请输入 X 方向外参增量："),QLineEdit::Normal,dx_now->text(),&ok);
    if (ok && !new_dx.isEmpty())
        dx_now->setText(new_dx);
}
void OperationWindow::SubtractDX(){
    command_row++;
    command_record->insertPlainText(tr("dx 减\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = dx_now->text().toDouble();
    double dtmp = 0.001;
    QString afterchange = QString::number( tmp - dtmp,'f',3);
    dx_now->setText(afterchange);
}
void OperationWindow::AddDX(){
    command_row++;
    command_record->insertPlainText(tr("dx 加\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = dx_now->text().toDouble();
    double dtmp = 0.001;
    QString afterchange = QString::number( tmp + dtmp,'f',3);
    dx_now->setText(afterchange);
}

void OperationWindow::SetDY(){
    command_row++;
    command_record->insertPlainText(tr("dy 大小设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_dy = QInputDialog::getText(this,tr("增量：dy"),tr("请输入 Y 方向外参增量："),QLineEdit::Normal,dy_now->text(),&ok);
    if (ok && !new_dy.isEmpty())
        dy_now->setText(new_dy);
}
void OperationWindow::SubtractDY(){
    command_row++;
    command_record->insertPlainText(tr("dy 减\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = dy_now->text().toDouble();
    double dtmp = 0.001;
    QString afterchange = QString::number( tmp - dtmp,'f',3);
    dy_now->setText(afterchange);
}
void OperationWindow::AddDY(){
    command_row++;
    command_record->insertPlainText(tr("dy 加\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = dy_now->text().toDouble();
    double dtmp = 0.001;
    QString afterchange = QString::number( tmp + dtmp,'f',3);
    dy_now->setText(afterchange);
}

void OperationWindow::SetDTheta(){
    command_row++;
    command_record->insertPlainText(tr("dtheta 大小设置\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    bool ok ;
    QString new_dtheta = QInputDialog::getText(this,tr("增量：dtheta"),tr("请输入 theta 角度外参增量："),QLineEdit::Normal,dtheta_now->text(),&ok);
    if (ok && !new_dtheta.isEmpty())
        dtheta_now->setText(new_dtheta);
}
void OperationWindow::SubtractDTheta(){
    command_row++;
    command_record->insertPlainText(tr("dtheta 减\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = dtheta_now->text().toDouble();
    double dtmp = 0.001;
    QString afterchange = QString::number( tmp - dtmp,'f',3);
    dtheta_now->setText(afterchange);
}
void OperationWindow::AddDTheta(){
    command_row++;
    command_record->insertPlainText(tr("dtheta 加\n"));
    command_record->moveCursor(QTextCursor::NextRow);

    double tmp = dtheta_now->text().toDouble();
    double dtmp = 0.001;
    QString afterchange = QString::number( tmp + dtmp,'f',3);
    dtheta_now->setText(afterchange);
}


