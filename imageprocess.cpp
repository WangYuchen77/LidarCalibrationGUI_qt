#include "imageprocess.h"

//******************************************* 图片显示窗口及操作按钮 ************************************

ShowResultWidget::ShowResultWidget(QWidget *parent):QWidget(parent){
    // 显示区域
    mergePicture= new QLabel(tr("雷达数据标定后合并显示"),this);
    mergePicture->setFixedSize(800,800);

    matrix_size = 2200; // opencv图像大小
    haveData1 = false;
    haveData2 = false;
    haveDraw = false;

    BigButton = new QPushButton("放大",this);
    LittleButton = new QPushButton("缩小",this);
    LeftButton = new QPushButton("向左",this);
    RightButton = new QPushButton("向右",this);
    UpButton = new QPushButton("向上",this);
    DownButton = new QPushButton("向下",this);
    DisableButton();

    Paint = QRect(10,10,810,810);
    QPoint Alloffset(0,0);
    size_label = new QLabel("100%",this);

    // 主布局
//    QHBoxLayout *mainlayout = new QHBoxLayout(this);
    QGridLayout *mainlayout = new QGridLayout(this);
    QVBoxLayout *leftlayout = new QVBoxLayout(this);
    leftlayout->addWidget(mergePicture,0,0);
    mergePicture->installEventFilter(this);
    mergePicture->setMouseTracking(true);
//    mainlayout->addWidget(mergePicture,0,0);
    // 右面的按钮布局
    QVBoxLayout *rightlayout = new QVBoxLayout(this);
    rightlayout->addWidget(size_label);
    rightlayout->addWidget(BigButton);
    rightlayout->addWidget(LittleButton);
    rightlayout->addWidget(LeftButton);
    rightlayout->addWidget(RightButton);
    rightlayout->addWidget(UpButton);
    rightlayout->addWidget(DownButton);

    mainlayout->addLayout(leftlayout,0,0);
    mainlayout->addLayout(rightlayout,0,1);

//    mainlayout->setContentsMargins(0, 0, 0, 0);// QLabel填满窗口

    ratio= 1.0;             //初始化图片缩放比例
    action = ShowResultWidget::None;
    pixW = 2200;            //设置图片尺寸为985*740
    pixH = 2200;

    pix = new QPixmap;

    connect(BigButton,SIGNAL(clicked()),this,SLOT(onBigClicked()));
    connect(LittleButton,SIGNAL(clicked()),this,SLOT(onLittleClicked()));
    connect(LeftButton,SIGNAL(clicked()),this,SLOT(OnLeftClicked()));
    connect(RightButton,SIGNAL(clicked()),this,SLOT(OnRightClicked()));
    connect(UpButton,SIGNAL(clicked()),this,SLOT(onUpClicked()));
    connect(DownButton,SIGNAL(clicked()),this,SLOT(onDownClicked()));

// resize窗口？
//    resize(890,850);

}
// 接收雷达1数据
void ShowResultWidget::ReceiveData_lidar1(std::vector<float> range1){
    this->range1 = range1;
    haveData1 = true;
}
// 接收雷达2数据
void ShowResultWidget::ReceiveData_lidar2(std::vector<float> range2){
    this->range2 = range2;
    haveData2 = true;
}
// 画图核心代码
void ShowResultWidget::draw(double show_id, double increment1, double increment2, double angle, double extrin_x, double extrin_y){
//    std::cout<<increment1<<std::endl;
//    std::cout<<increment2<<std::endl;
//    std::cout<<angle<<std::endl;
//    std::cout<<extrin_x<<std::endl;
//    std::cout<<extrin_y<<std::endl;
    int pointCloudSize_lidar1 = range1.size();
//    std::cout<<"pointcloud 1 size:"<<pointCloudSize_lidar1<<std::endl;
    for (int i = 0 ; i < pointCloudSize_lidar1 ; i++){
        if (abs(range1[i]) < 20){
            xList_1.append(range1[i]*cos(double(-135 + increment1 * i)/180*CV_PI));
            yList_1.append(range1[i]*sin(double(-135 + increment1 * i)/180*CV_PI));
        }
    }
    int pointCloudSize_lidar2 = range2.size();
//    std::cout<<"pointcloud 2 size:"<<pointCloudSize_lidar2<<std::endl;
    for (int i = 0 ; i < pointCloudSize_lidar2 ; i++){
        if (abs(range2[i]) < 20){
            xList_2.append(range2[i]*cos((-135 + increment2 * i)/180*CV_PI));
            yList_2.append(range2[i]*sin((-135 + increment2 * i)/180*CV_PI));
        }
    }
    update();

    // 显示雷达点在图像的什么位置
    double u_lidar1_image[1080];
    double v_lidar1_image[1080];
    double u_lidar2_image[1080];
    double v_lidar2_image[1080];
    for (int i = 0 ; i < xList_1.size() ; i++){
        u_lidar1_image[i] = matrix_size/2 + floor(xList_1[i]*100);
        v_lidar1_image[i] = matrix_size/2 - floor(yList_1[i]*100);
    }
    angle = angle / 180 * CV_PI;
    double x_aftercalib;
    double y_aftercalib;
    for (int i = 0 ; i < xList_2.size() ; i++){
        x_aftercalib = xList_2[i] * cos(angle) - yList_2[i] * sin(angle) + extrin_x;
        y_aftercalib = xList_2[i] * sin(angle) + yList_2[i] * cos(angle) + extrin_y;
        u_lidar2_image[i] = matrix_size/2 + floor(x_aftercalib*100);
        v_lidar2_image[i] = matrix_size/2 - floor(y_aftercalib*100);
    }
    xList_1.clear();
    yList_1.clear();
    xList_2.clear();
    yList_2.clear();
    std::vector<cv::Point2d> ptSet1_original;
    std::vector<cv::Point2d> ptSet2_original;
    for (int i = 0 ; i< pointCloudSize_lidar1; i++){
        cv::Point2d pt_1(u_lidar1_image[i], v_lidar1_image[i]);
        ptSet1_original.push_back(pt_1);
    }
    for (int i = 0 ; i< pointCloudSize_lidar2; i++){
        cv::Point2d pt_2(u_lidar2_image[i], v_lidar2_image[i]);
        ptSet2_original.push_back(pt_2);
    }

    // 显示雷达点
    cv::Mat image(matrix_size, matrix_size, CV_8UC3, cv::Scalar(255, 255, 255));
    if (show_id == 0 || show_id == 1){
        for (int i = 0; i < ptSet1_original.size(); i++){
            cv::circle(image, ptSet1_original[i], 1, cv::Scalar(255, 0, 0), -1, 8);
            cv::circle(image, ptSet1_original[i], 5, cv::Scalar(255, 0, 0), 1, 8);
        }
    }
    if (show_id == 0 || show_id == 2){
        for (int i = 0; i < ptSet2_original.size(); i++){
            cv::circle(image, ptSet2_original[i], 1, cv::Scalar(0, 0, 255), -1, 8);
            cv::circle(image, ptSet2_original[i], 5, cv::Scalar(0, 0, 255), 1, 8);
        }
    }

//    image_show = image;
    image_show = image.clone();


//    cv::Rect rect(50,20, 1500, 500);
//    cv::Mat image = image1(rect);


//    cv::Mat image = image(cv::Range(500,1500),cv::Range(500,1500));

//    std::cout<<image.cols<<std::endl;
//    std::cout<<image.rows<<std::endl;
//    cv::namedWindow("11");
//    cv::imshow("11", image_show);


//    cv::Mat dst = cv::Mat::zeros(300, 300, CV_8UC3);
//    cv::resize(image,image, dst.size());


//    std::string imageName( "/Users/admin/Desktop/map.png" ); // 改成你想要的图片
//    image = cv::imread( imageName, cv::IMREAD_COLOR ); // Read the file
//    if( image.empty() )                      // Check for invalid input
//    {
//        std::cout <<  "Could not open or find the image" << std::endl ;
//    }

    cv::Mat imageAddAxes = AddAxes(image);
    QImage img;
    int chana = imageAddAxes.channels();

     //依据通道数不同，改变不同的装换方式
    if(3 == chana ){
        //调整通道次序
        cv::cvtColor(imageAddAxes,imageAddAxes,cv::COLOR_BGR2RGB);
        img = QImage(static_cast<uchar *>(imageAddAxes.data),imageAddAxes.cols,imageAddAxes.rows,QImage::Format_RGB888);
    }
    else if(4== chana ){
         //argb
        img = QImage(static_cast<uchar *>(imageAddAxes.data),imageAddAxes.cols,imageAddAxes.rows,QImage::Format_ARGB32);
    }
    else {
        //单通道，灰度图
        img = QImage( imageAddAxes.cols, imageAddAxes.rows , QImage::Format_Indexed8);
        uchar * matdata = imageAddAxes.data;
        for(int row = 0 ; row <imageAddAxes.rows ; ++row ){
            uchar* rowdata = img.scanLine( row );
            memcpy(rowdata,matdata ,imageAddAxes.cols);
            matdata+=imageAddAxes.cols;
        }
    }
//    cv::cvtColor(image,image,cv::COLOR_RGB2BGR);

//    int with = 2200;
//    int height = 2200;
//    img = img.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充

    // 图片适应QLabel的大小，加了这句话雷达图像变模糊了,KeepAspectRatio改成了KeepAspectRatioByExpanding
    // img = img.scaled(mergePicture->size(), Qt::KeepAspectRatio);
//    img = img.scaled(mergePicture->size(), Qt::KeepAspectRatioByExpanding);
    mergePicture->setScaledContents(true);
    mergePicture->setPixmap(QPixmap::fromImage(img));

    // 暂时不知道，为啥得把正常的BGR换到RGB，放大缩小那些功能才能显示正常颜色
    // 放大缩小的那些函数，是BGR->RGB->BGR
    // 从BGR转到RGB以便画图，再从RGB转回到BGR以备下次继续使用
    // 按照道理，输入就应该是BGR格式，结果输入一张RGB格式的Mat才成功，不知道为啥
    // 下面这句话的图像show的话已经是雷达1红，雷达2蓝了。实际上雷达1蓝雷达2红才对
    // 奇怪的是把COLOR_RGB2BGR换成COLOR_RGB2BGR，依然成功，还是写成COLOR_RGB2BGR吗，这样还顺点眼
    cv::cvtColor(image_show,image_show,cv::COLOR_RGB2BGR);

    haveDraw = true;
}
//void ShowResultWidget::paintEvent(QPaintEvent *){

//    QPainter painter(this);
//    QPen pen;
//    pen.setColor(Qt::blue);
//    pen.setStyle(Qt::SolidLine);
//    pen.setWidthF(0.01);
//    painter.setPen(pen);
//    painter.setViewport(50, 50, width()-100, height()-100);
//    /*painter.setWindow(-10, 2, 20, -4); */// (-10, 2)    (10, -2)


//    painter.setWindow(-10, 2, 20, -4);
//    painter.fillRect(-10, 2, 20, -4, Qt::white);

////    painter.translate(20000, 20000);
////    painter.setWindow(-20000, -20000, 60000, 60000);

//    painter.drawLine(QPointF(-10, 0), QPointF(10, 0));   // x
////    painter.drawLine(QPointF(0, 2), QPointF(0, -2));     // y
//    for(int i = 0; i < yList_1.count(); i++)
//    {
//        painter.drawPoint(QPointF(xList_1[i], yList_1[i]));
////        std::cout<<xList_1[i]<<std::endl;
//    }
//    pen.setColor(Qt::red);
//    painter.setPen(pen);
//    for(int i = 0; i < yList_2.count(); i++)
//    {
//        painter.drawPoint(QPointF(xList_2[i], yList_2[i]));
//    }
//    std::cout<<xList_1.count()<<std::endl;
//    std::cout<<xList_2.count()<<std::endl;
//    xList_1.clear();
//    yList_1.clear();
//    xList_2.clear();
//    yList_2.clear();
//}
cv::Mat ShowResultWidget::AddAxes(cv::Mat inputMat){
    cv::Mat imageAddAxes = cv::Mat(inputMat.rows+100, inputMat.cols+100, CV_8UC3, cv::Scalar(255,255,255));
    drawAxes.InputFigure(inputMat, imageAddAxes);
    std::string ylabel = "y";
    std::string xlabel = "x";
    std::string title_name = "Calibration";
    drawAxes.DrawLabel_Y(ylabel, -matrix_size/200, matrix_size/200, matrix_size/100, cv::Scalar(0, 0, 0));
    drawAxes.DrawLabel_X(xlabel, -matrix_size/200, matrix_size/200, matrix_size/100, cv::Scalar(0, 0, 0));
    drawAxes.DrawTitle(title_name);
    return imageAddAxes;
}

bool ShowResultWidget::event(QEvent * event){
    static bool press=false;
    static QPoint PreDot;

    if (haveData1 == true && haveData2 == true && haveDraw == true){
        if(event->type() == QEvent::MouseButtonPress ){
            QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);
            //判断鼠标是否是左键按下,且鼠标位置是否在绘画区域
            if(mouse->button()==Qt::LeftButton && Paint.contains(mouse->pos())){
                press=true;
                QApplication::setOverrideCursor(Qt::OpenHandCursor); //设置鼠标样式
                PreDot = mouse->pos();
            }

        }
        else if(event->type() == QEvent::MouseButtonRelease){
            QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);
            //判断鼠标是否是左键释放,且之前是在绘画区域
            if(mouse->button()==Qt::LeftButton && press ){
                QApplication::setOverrideCursor(Qt::ArrowCursor); //改回鼠标样式
                press=false;
            }
        }
        if(event->type() == QEvent::MouseMove){              //移动图片
            if(press){
                QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);
                int speed = 3;
                offset.setX( (PreDot.x() - mouse->x())*speed);
                offset.setY( (PreDot.y() - mouse->y())*speed);
                PreDot = mouse->pos();
                action = ShowResultWidget::Move;
                this->update();
            }
        }
    }
    return QWidget::event(event);
}

void ShowResultWidget::wheelEvent(QWheelEvent* event)     //鼠标滑轮事件
{
    if (haveData1 == true && haveData2 == true && haveDraw == true){
        if (event->delta()>0){      //上滑,缩小
            action=ShowResultWidget::Shrink;
            this->update();
        }
        else{                    //下滑,放大
            action=ShowResultWidget::Amplification;
            this->update();
        }
        event->accept();
    }
}

// 对图像进行放大缩小移动操作
void ShowResultWidget::paintEvent(QPaintEvent *event)
{
    if (haveData1 == true && haveData2 == true){
        QPainter painter(this);
        int NowW = ratio *pixW;
        int NowH = ratio *pixH;

        if(action==ShowResultWidget::Amplification){           //放大图像
            ratio-=0.1*ratio;
            if(ratio<0.018)
                ratio = 0.01;
            /*显示比例*/
            QString str;
            str.sprintf("%.0f%",ratio*100);
            size_label->setText(str) ;
        }
        else if(action==ShowResultWidget::Shrink){          //缩小图像
            ratio+=0.1*ratio;
            if(ratio>4.5)
                ratio = 5.000;
            /*显示比例*/
            QString str;
            str.sprintf("%.0f%",ratio*100);
            size_label->setText(str);
        }
        if(action==ShowResultWidget::Amplification || action==ShowResultWidget::Shrink){      //更新图片
            NowW = ratio *pixW;
            NowH = ratio *pixH;
            //pix->load("/Users/admin/Desktop/x.jpg");                 //重新装载,因为之前的图片已经被缩放过

            // 加载准备显示的雷达图片，并把Mat格式转为QImage格式
            cv::Mat imageAddAxes = AddAxes(image_show);
            QImage _image_show;
            cv::cvtColor(imageAddAxes,imageAddAxes,cv::COLOR_BGR2RGB);
            _image_show = QImage(static_cast<uchar *>(imageAddAxes.data),imageAddAxes.cols,imageAddAxes.rows, QImage::Format_RGB888);
    //        cv::imshow("out", this->image_show);
            cv::cvtColor(imageAddAxes,imageAddAxes,cv::COLOR_RGB2BGR);

            // 向左和向右移动的偏移量
            int offsetx=Alloffset.x()+offset.x();
            Alloffset.setX(offsetx);
            int offsety=Alloffset.y()+offset.y();
            Alloffset.setY(offsety);

            // 截取QImage的一部分进行显示，四个参数（左上角col，左上角row，图片col宽，图片row高）
            // 下面代码截取的部分是，永远以原图像中心为放大缩小点，并添加偏移
            _image_show = _image_show.copy( (matrix_size-NowW)/2 + offsetx,
                                            (matrix_size-NowW)/2 + offsety,
                                            NowW, NowH);

            // QLabel显示这张Pixmap
            *pix = QPixmap::fromImage(_image_show);
            //*pix = pix->scaled(NowW, NowH,Qt::KeepAspectRatio);
            action=ShowResultWidget::None;
            mergePicture->setScaledContents(true);
            mergePicture->setPixmap(*pix);
        }

        if(action==ShowResultWidget::Move)                    //移动
        {
            int offsetx=Alloffset.x()+offset.x();
            Alloffset.setX(offsetx);

            int offsety=Alloffset.y()+offset.y();
            Alloffset.setY(offsety);
            action=ShowResultWidget::None;

            NowW = ratio *pixW;
            NowH = ratio *pixH;

            cv::Mat imageAddAxes = AddAxes(image_show);
            QImage _image_show;
            cv::cvtColor(imageAddAxes,imageAddAxes,cv::COLOR_BGR2RGB);
            _image_show = QImage(static_cast<uchar *>(imageAddAxes.data),imageAddAxes.cols,imageAddAxes.rows,QImage::Format_RGB888);
            cv::cvtColor(imageAddAxes,imageAddAxes,cv::COLOR_RGB2BGR);

            _image_show = _image_show.copy( (matrix_size-NowW)/2 + offsetx,
                                            (matrix_size-NowW)/2 + offsety,
                                            NowW, NowH);

            *pix = QPixmap::fromImage(_image_show);
            mergePicture->setScaledContents(true);
            mergePicture->setPixmap(*pix);
        }

        if(abs(Alloffset.x())>=(Paint.width()/2 + NowW/2 -10)){    //限制X偏移值
            if(Alloffset.x()>0)
                Alloffset.setX(Paint.width()/2 + NowW/2 -10);
            else
                Alloffset.setX(-Paint.width()/2 + -NowW/2 +10);
        }
        if(abs(Alloffset.y())>=(Paint.height()/2 + NowH/2 -10)){    //限制Y偏移值
            if(Alloffset.y()>0)
                Alloffset.setY(Paint.height()/2 + NowH/2 -10);
            else
                Alloffset.setY(-Paint.height()/2 + -NowH/2 +10);
        }

        int x = Paint.width()/2 + Alloffset.x() -NowW/2;
        if(x<0)
            x=0;

        int y = Paint.height()/2 + Alloffset.y() -NowH/2;
        if(y<0)
            y=0;

        int  sx = NowW/2 - Paint.width()/2 - Alloffset.x();
        if(sx<0)
            sx=0;

        int  sy = NowH/2 - Paint.height()/2 - Alloffset.y();
        if(sy<0)
            sy=0;

        int w =(NowW - sx)>Paint.width()? Paint.width() : (NowW - sx);
        if(w>(Paint.width()-x))
            w = Paint.width()-x;

        int h =(NowH - sy)>Paint.height()? Paint.height() : (NowH - sy);
        if(h>(Paint.height()-y))
            h = Paint.height()-y;
    }

//    painter.drawRect(Paint.x()-1,Paint.y()-1,Paint.width()+1,Paint.height()+1); //画框
//    painter.drawTiledPixmap(x+Paint.x(),y+Paint.y(),w,h,*pix,sx,sy);             //绘画图形
}

void ShowResultWidget::ClearImage(){
    // 设置一张白图作为无数据时的显示
    cv::Mat image(matrix_size, matrix_size, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    QImage img = QImage(static_cast<uchar *>(image.data),image.cols,image.rows,QImage::Format_RGB888);
    mergePicture->setScaledContents(true);
    mergePicture->setPixmap(QPixmap::fromImage(img));

    image_show.release();
    haveData1 = false;
    haveData2 = false;
    haveDraw = false;
}
// 激活所有按钮
void ShowResultWidget::EnableButton(){
    BigButton->setEnabled(true);
    LittleButton->setEnabled(true);
    LeftButton->setEnabled(true);
    RightButton->setEnabled(true);
    UpButton->setEnabled(true);
    DownButton->setEnabled(true);
}
// 失能所有按钮
void ShowResultWidget::DisableButton(){
    BigButton->setEnabled(false);
    LittleButton->setEnabled(false);
    LeftButton->setEnabled(false);
    RightButton->setEnabled(false);
    UpButton->setEnabled(false);
    DownButton->setEnabled(false);
}

// 以下是每个操作按钮触发的动作，并进入update()函数
void  ShowResultWidget::onBigClicked()
{
    action=ShowResultWidget::Amplification;
    this->update();
}
void  ShowResultWidget::onLittleClicked()
{
    action=ShowResultWidget::Shrink;
    this->update();
}
void ShowResultWidget::onUpClicked()
{
    action=ShowResultWidget::Move;
    offset.setX(0);
    offset.setY(20);
    this->update();
}
void ShowResultWidget::onDownClicked()
{
    action=ShowResultWidget::Move;
    offset.setX(0);
    offset.setY(-20);
    this->update();
}
void ShowResultWidget::OnLeftClicked()
{
    action=ShowResultWidget::Move;
    offset.setX(20);
    offset.setY(0);
    this->update();
}
void ShowResultWidget::OnRightClicked()
{
    action=ShowResultWidget::Move;
    offset.setX(-20) ;
    offset.setY(0) ;
    this->update();
}
