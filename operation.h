#ifndef OPERATION_H
#define OPERATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QTextBrowser>
#include <QLineEdit>
#include <QInputDialog>
#include <QButtonGroup>
#include <QRadioButton>


#include <math.h>
#include <iostream>
#include <fstream>

class InputDataWindow: public QWidget{
    Q_OBJECT
public:
    InputDataWindow(QWidget *parent= 0);

    double lidar1_angle[1080];
    double lidar1_range[1080];
    double lidar2_angle[1080];
    double lidar2_range[1080];

    std::vector<float> range1;
    std::vector<float> range2;

    bool have_lidar1;
    bool have_lidar2;
    std::string path_lidar1 = "/Desktop/CalibrationLidarofFar/data/lidar1_data.txt";
    std::string path_lidar2 = "/Desktop/CalibrationLidarofFar/data/lidar2_data.txt";

    QVBoxLayout *inputDatalayout;

    QPushButton *inputData_lidar1;
    QPushButton *inputData_lidar2;
    QPushButton *initial_extrinsic;
    QPushButton *draw_data;
    QPushButton *clear_data;

signals:
    void SendData_lidar1(std::vector<float>);
    void SendData_lidar2(std::vector<float>);
    void command_initialExtrinsic();
    void command_draw();
    void command_clear();
    void command_enablebutton();

private slots:
    void InputDataLidar1();
    void InputDataLidar2();
    void InitialExtrinsic();
    void DrawData();
    void ClearData();
    void EnableButton();

};

class OperationWindow: public QWidget{
    Q_OBJECT
public:
    OperationWindow(QWidget *parent= 0);

    QVBoxLayout *operationlayout;

    QTextBrowser *command_record;
    int command_row;

    QButtonGroup *lidar_show;
    QRadioButton *lidarAll_show;
    QRadioButton *lidar1_show;
    QRadioButton *lidar2_show;

    QLabel *lidar1_increment;
    QLabel *lidar1_increment_now;
    QPushButton *lidar1_increment_set;
    QLabel *lidar2_increment;
    QLabel *lidar2_increment_now;
    QPushButton *lidar2_increment_set;


    QLabel *extrinsic_x;
    QLabel *extrinsic_x_now;
    QPushButton *x_set;
    QPushButton *x_subtract;
    QPushButton *x_add;

    QLabel *extrinsic_y;
    QLabel *extrinsic_y_now;
    QPushButton *y_set;
    QPushButton *y_subtract;
    QPushButton *y_add;

    QLabel *extrinsic_theta;
    QLabel *extrinsic_theta_now;
    QPushButton *theta_set;
    QPushButton *theta_subtract;
    QPushButton *theta_add;

    QLabel *dx;
    QLabel *dx_now;
    QPushButton *dx_set;
    QPushButton *dx_subtract;
    QPushButton *dx_add;

    QLabel *dy;
    QLabel *dy_now;
    QPushButton *dy_set;
    QPushButton *dy_subtract;
    QPushButton *dy_add;

    QLabel *dtheta;
    QLabel *dtheta_now;
    QPushButton *dtheta_set;
    QPushButton *dtheta_subtract;
    QPushButton *dtheta_add;
signals:
    void command_draw(double, double, double, double, double, double);

private slots:

    void ReceiveInput_lidar1();
    void ReceiveInput_lidar2();
    void InitialExtrinsic();
    void DrawData();

    void DrawWhichLidar();

    void EnableButton();
    void DisableButton();

    void SetIncrement_lidar1();
    void SetIncrement_lidar2();
    void SetX();
    void SubtractX();
    void AddX();

    void SetY();
    void SubtractY();
    void AddY();

    void SetTheta();
    void SubtractTheta();
    void AddTheta();

    void SetDX();
    void SubtractDX();
    void AddDX();

    void SetDY();
    void SubtractDY();
    void AddDY();

    void SetDTheta();
    void SubtractDTheta();
    void AddDTheta();
};

#endif // OPERATIONWINDOW_H