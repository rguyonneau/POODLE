#include "poodle_window.h"
#include "ui_poodle_window.h"

#include <wiringPi.h>
#include <QTime>
#include <QDebug>
#include <QProcess>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LASER_GPIO 5

// for the camera calibration
#define XINFPX 493
#define XSUPPX 878
#define YINFPX 54
#define YSUPPX 603

Poodle_window::Poodle_window(QWidget *parent) : QMainWindow(parent), _logs(new Log_handler()), ui(new Ui::Poodle_window), _driver(_logs), _camera(_logs) {
    ui->setupUi(this);

    wiringPiSetup () ; // from wiringPi library
    pinMode (LASER_GPIO, OUTPUT) ; // from wiringPi library
    connect(_logs, SIGNAL(signal_addLog(QString, int)), this, SLOT(slot_addLog(QString, int)));

    _labelImageNonProcessed = new ClickableLabel(ui->gb_imageNP);
    _labelImageProcessed = new ClickableLabel(ui->gb_imageP);
    connect(_labelImageNonProcessed, SIGNAL(clicked(QMouseEvent*)), this, SLOT(onImageNPClicked(QMouseEvent*)));
    connect(_labelImageNonProcessed, SIGNAL(clicked(QMouseEvent*)), this, SLOT(onImagePClicked(QMouseEvent*)));


    ui->btn_updateimage->setEnabled(false);
    ui->btn_weeding->setEnabled(false);
    ui->tab_manual->setEnabled(false);

    _initonce = false;

    ui->lb_offcet1_val->setText(QString::number(OFFCET_MIRROR_1));
    ui->lb_offcet2_val->setText(QString::number(OFFCET_MIRROR_2));
    ui->lb_offcet1_name->setText(QString("Offcet mirror " + QString::number(ID_MIRROR_1) + ": "));
    ui->lb_offcet2_name->setText(QString("Offcet mirror " + QString::number(ID_MIRROR_2) + ": "));
    ui->lb_mirror1->setText(QString("Mirror " + QString::number(ID_MIRROR_1) + ": "));
    ui->lb_mirror2->setText(QString("Mirror " + QString::number(ID_MIRROR_2) + ": "));

}

Poodle_window::~Poodle_window(){
    delete ui;
}

void Poodle_window::on_pushButton_clicked(){
    if(_driver.connect()){
        _driver.configureMirrors();
        get_images();
        _camera.calibrate(XINFPX, XSUPPX, YINFPX, YSUPPX);
        ui->btn_updateimage->setEnabled(true);
        ui->btn_weeding->setEnabled(true);
        ui->tab_manual->setEnabled(true);

        _initonce = true;
    }
}

void Poodle_window::addLog(QString txt, int type){
    // function that add an item (txt) to the log listwidget
    // type indicates the type of message (INFO, WARN...)
    QTime ctime = QTime::currentTime(); // to add a time stamp to the log
    QString txt_log;
    QString stype;
    QColor color;
    bool hidden=false; //using the check box we want to hide/display logs according to their type

    switch(type){
        case LOG_INFO: // INFO
            color = Qt::green;
            stype = "INFO";
            hidden = ! ui->cb_info->isChecked();
            break;
        case LOG_WARN: // WARNING
            color = Qt::blue;
            stype = "WARN";
            hidden = ! ui->cb_warn->isChecked();
            break;
        case LOG_ERR: // ERROR
            color = Qt::red;
            stype = "ERR";
            hidden = ! ui->cb_err->isChecked();
            break;
        case LOG_CAN: // CAN
            color = Qt::black;
            stype = "CAN";
            hidden = ! ui->cb_can->isChecked();
            break;
        default:
            color = Qt::yellow;
            stype = "DEF";
    }

    txt_log = ctime.toString("[hh:mm:ss] - "); // adding the time stamp
    txt_log += stype + " - ";
    txt_log += txt;

    ui->lw_logs->insertItem(0,txt_log); // the new log is added to the top of the list
    ui->lw_logs->item(0)->setForeground(color);
    ui->lw_logs->item(0)->setHidden(hidden);
}

void Poodle_window::updatelistwidget(){
    // fonction to update the log list widget display
    // hide messages that should be hidden and show messages that should be shown
    for(int i=0; i< ui->lw_logs->count(); i++){
        if(ui->lw_logs->item(i)->text().toUtf8()[13] == 'C'){
            ui->lw_logs->item(i)->setHidden(!ui->cb_can->isChecked());
        }else if(ui->lw_logs->item(i)->text().toUtf8()[13] == 'I'){
            ui->lw_logs->item(i)->setHidden(!ui->cb_info->isChecked());
        }else if(ui->lw_logs->item(i)->text().toUtf8()[13] == 'W'){
            ui->lw_logs->item(i)->setHidden(!ui->cb_warn->isChecked());
        }else if(ui->lw_logs->item(i)->text().toUtf8()[13] == 'E'){
            ui->lw_logs->item(i)->setHidden(!ui->cb_err->isChecked());
        }
    }
}

// Functions associated to the checkbox to hide/show log messages
void Poodle_window::on_cb_can_clicked(){
    updatelistwidget();
}

void Poodle_window::on_cb_info_clicked(){
    updatelistwidget();
}

void Poodle_window::on_cb_warn_clicked(){
    updatelistwidget();
}

void Poodle_window::on_cb_err_clicked(){
    updatelistwidget();
}

void Poodle_window::slot_addLog(QString txt, int type){
    addLog(txt, type);
}


void Poodle_window::get_images(){
    if(!_camera.getDataFromCamera()){
        addLog("Impossible to get an Image from the camera", LOG_ERR);
        return;
    }

    _imageNonProcessed = QImage(_camera.getNonProcessedData(), _camera.getWidth(), _camera.getHeight(), QImage::Format_RGB888);
    _labelImageNonProcessed->setScaledContents(true);
    _labelImageNonProcessed->setGeometry(0,
                        0,
                        ui->gb_imageNP->geometry().height()*1280.0/960.0,
                        ui->gb_imageNP->geometry().height());
    _labelImageNonProcessed->setPixmap(QPixmap::fromImage(_imageNonProcessed));

    _imageProcessed = QImage(_camera.getProcessedData(), _camera.getWidth(), _camera.getHeight(), QImage::Format_RGB888);
    _labelImageProcessed->setScaledContents(true);
    _labelImageProcessed->setGeometry(0,
                        0,
                        ui->gb_imageP->geometry().height()*1280.0/960.0,
                        ui->gb_imageP->geometry().height());
    _labelImageProcessed->setPixmap(QPixmap::fromImage(_imageProcessed));

}

void Poodle_window::on_btn_cleanlogs_clicked(){
    ui->lw_logs->clear();
}

void Poodle_window::on_btn_updateimage_clicked(){
    get_images();
}

void Poodle_window::resizeEvent( QResizeEvent *e ){
    if(_initonce) get_images();
    QWidget::resizeEvent(e);
}

void Poodle_window::transfert_fct(double x , double y, double z, double &phi1, double &phi2, double &f)
{

    //double d = 50;   //Il s'agit en fait de la variable x
    double t = 2.45;
    double d_L = 11;   //diametre du lentille

    //double lambda_nm = 587;       //longueur d'onde, inclu dans n_L
    double n_L = 1.559;             //indice du lentille
    double o1_y = 28;                 //coordonnées du 1ere miroir (mm) cf dessin Franck.
    double lv2_x = -30;               //coordonnées du lentille suivant x
    double lv2_y = 30;                //coordonnées du lentille suivant x

    phi1 = -0.5 * atan(x/(o1_y + sqrt((pow(z,2))+(pow(y,2))) ) );
    phi2 = 0.5  * atan(y/z);

    phi1 = (phi1 * 180/M_PI)*1000;
    phi2 = (phi2 * 180/M_PI)*1000;

    double r_square = pow(y,2) + pow(x,2);
    //double r_xy = sqrt(r_square);
    double inter = pow(o1_y +  sqrt(r_square),2);

    double s_prime = -lv2_x +
                     sqrt(r_square + r_square*pow(z,2) / inter ) +
                     sqrt(pow(o1_y,2) + (pow(lv2_y,2) * pow(z,2)) / inter );

    double D_f = 4*pow(n_L,4)*pow(s_prime,2) -
                 8*pow(n_L,3)*s_prime*(s_prime-t) +
                 (pow(n_L,2))*(pow(d_L,2)+4*(pow(s_prime,2)+pow(t,2))-16*s_prime*t) +
                 n_L*(8*t*(s_prime-t)-2*pow(d_L,2)) +
                 4*pow(t,2);

    f= (- 1 / (n_L*(n_L - 2))) * (t + n_L*s_prime - 0.5*sqrt(D_f));
}

void Poodle_window::on_btn_weeding_clicked(){
    on_btn_laserOFF_clicked();

    if(!_camera.get_isCalibrated()){
        _camera.calibrate(XINFPX, XSUPPX, YINFPX, YSUPPX); // calibration should be done at least once
        if(!_camera.get_isCalibrated()) return;
    }

    std::vector<double> adv_pos(0);
    if(!_camera.process(adv_pos)){
        return;
    }

    _imageNonProcessed = QImage(_camera.getNonProcessedData(), _camera.getWidth(), _camera.getHeight(), QImage::Format_RGB888);
    _labelImageNonProcessed->setScaledContents(true);
    _labelImageNonProcessed->setGeometry(0,
                        0,
                        ui->gb_imageNP->geometry().height()*1280.0/960.0,
                        ui->gb_imageNP->geometry().height());
    _labelImageNonProcessed->setPixmap(QPixmap::fromImage(_imageNonProcessed));

    _imageProcessed = QImage(_camera.getProcessedData(), _camera.getWidth(), _camera.getHeight(), QImage::Format_RGB888);
    _labelImageProcessed->setScaledContents(true);
    _labelImageProcessed->setGeometry(0,
                        0,
                        ui->gb_imageP->geometry().height()*1280.0/960.0,
                        ui->gb_imageP->geometry().height());
    _labelImageProcessed->setPixmap(QPixmap::fromImage(_imageProcessed));

    QString log;
    log += QString::number(adv_pos.size()/2) + " adventice(s) detected: \n";


    for(unsigned int i=0; i<adv_pos.size(); i+=2){
        log += "(" + QString::number(adv_pos[i]) + "," + QString::number(adv_pos[i+1]) +")\n";
        double phi1;
        double phi2;
        double f;
        transfert_fct(110 - adv_pos[i] , 248 - adv_pos[i+1], 820, phi1, phi2, f);

        _driver.go2position_angle(phi1, phi2);

        usleep(300000);
        for(int j=0; j<3; j++){
            digitalWrite (LASER_GPIO, HIGH) ; // from wiringPi library
            usleep(50000);
            digitalWrite (LASER_GPIO, LOW) ; // from wiringPi library
            usleep(50000);
        }

    }
    addLog(log, LOG_INFO);
}

void Poodle_window::on_btn_laserON_clicked(){
    digitalWrite (LASER_GPIO, HIGH) ; // from wiringPi library
}

void Poodle_window::on_btn_laserOFF_clicked(){
    digitalWrite (LASER_GPIO, LOW) ; // from wiringPi library
}

void Poodle_window::on_btn_initlens_clicked()
{// function to init the lens (and the serial connexion)

    //ui->btn_setfocale->setEnabled(false);

    QProcess process; // to handle bash commands

    //getting the ttyACM* number of the lens, using the "ls" bash command
    process.start("sh", QStringList()<<"-c"<<"ls /dev/ | grep ttyACM*");
    process.waitForFinished();
    usleep(1000);
    QString output(process.readAllStandardOutput()); // getting the value of the bash command
    output.remove(output.length()-1,1); //removing \n at the end
    output = QString("/dev/")+output; // adding the /dev/ to the serial port

    //giving access to be able to read and write the serial port
    process.start("sh",  QStringList() << "-c" << (QString("echo laris | sudo -S chmod 666 ") + output ));
    process.waitForFinished();
    usleep(1000);
    QString msg;
    msg = process.readAllStandardError();
    if(msg.length()>0){ // if the chmod command went wrong
        addLog(msg, LOG_ERR);
        return;
    }

    // init the serial socket to communcate with the lens
    if(_lens.init(output.toStdString().c_str(),msg) <0){
        addLog(msg, LOG_ERR);
        return;
    }

    // send the start command
    if(_lens.cmd_start(msg)<0){
        addLog(msg, LOG_ERR);
        return;
    }
    addLog(msg, LOG_INFO);

    // configure the command mode of the lens
    if(_lens.cmd_CommandMode(msg)<0){
        addLog(msg, LOG_ERR);
        return;
    }
    addLog(msg, LOG_INFO);

    ui->btn_setFocale->setEnabled(true);
    ui->txb_focale->setEnabled(true);
}

void Poodle_window::on_btn_setFocale_clicked()
{
    bool ok;
    QString msg;
    int f = ui->txb_focale->text().toDouble(&ok);
    if(!ok){
        addLog("the focale value should be a double", LOG_ERR);
        return;
    }

    if(_lens.cmd_changeFocale(f, msg)<0){
        addLog(msg, LOG_ERR);
        return;
    }
    addLog(msg, LOG_INFO);

}

void Poodle_window::on_btn_getpositions_clicked(){

    // timer to update the position value of the motors
    int32_t actpos;

    // Node 1
    if(!_driver.readRegister(REG_PPOS_ACPO2, ID_MIRROR_1, &actpos, sizeof(actpos),0, false)){
        addLog("Fail to read Max profile velocity", LOG_ERR);
        return;
    }
    ui->lb_pos1->setText(QString::number(actpos,10));
    ui->txb_pos1->setText(QString::number(actpos-OFFCET_MIRROR_1,10));

    // Node 2
    if(!_driver.readRegister(REG_PPOS_ACPO2, ID_MIRROR_2, &actpos, sizeof(actpos),0, false)){
        addLog("Fail to read Max profile velocity", LOG_ERR);
        return;
    }
    ui->lb_pos2->setText(QString::number(actpos,10));
    ui->txb_pos2->setText(QString::number(actpos-OFFCET_MIRROR_2,10));
}

void Poodle_window::on_btn_setpositions_clicked(){
    // function to set the motors into the wanted position (according to the textbox values)
    double phi1, phi2;
    bool ok;

    // first we check that the position wanted is a correct number
    phi1 = ui->txb_pos1->text().toInt(&ok,10);
    if(!ok){
        addLog("Target position should be an int", LOG_ERR);
        return;
    }

    phi2 = ui->txb_pos2->text().toInt(&ok,10);
    if(!ok){
        addLog("Target position should be an int", LOG_ERR);
        return;
    }

    _driver.go2position_angle(phi1, phi2);

}

void Poodle_window::on_btn_compute_tf_clicked(){
    // code de matthieu
    //NB : A modifier suivant nos coordonnées

    //arguments
    bool ok;
    double x = ui->txb_x_mm->text().toDouble(&ok);
    if(!ok){
        addLog("x value should be double", LOG_ERR);
    }
    double y=ui->txb_y_mm->text().toDouble(&ok);
    if(!ok){
        addLog("x value should be double", LOG_ERR);
    }
    double z=ui->txb_z_mm->text().toDouble(&ok);
    if(!ok){
        addLog("x value should be double", LOG_ERR);
    }

    double phi1;
    double phi2;
    double f;

    transfert_fct(x, y, z, phi1, phi2, f);

    addLog(QString("phi1 (mdeg):")+QString::number(phi1), LOG_INFO);
    addLog(QString("phi2 (mdeg):")+QString::number(phi2), LOG_INFO);

    ui->txb_pos1->setText(QString::number((int)phi1));
    ui->txb_pos2->setText(QString::number((int)phi2));

    ui->txb_focale->setText(QString::number(f));

    /*NB :

        def fct_transfert(x, y, z): # x <-> d dans l'article

        phi1 = - np.arctan(z/(o1_y + np.sqrt((y**2)+(x**2)) ) )
        phi2 = 0.5  * np.arctan(y/x)

        r_square = y**2 + x**2
        r_xy = np.sqrt(r_square)
        inter = (o1_y +  np.sqrt(y**2 + x**2))**2

        s_prime = -lv2_x + np.sqrt(r_square + r_square*z**2 / inter ) + np.sqrt(o1_y**2 + (lv2_y**2 * z**2) / inter )
        #print(s_prime)

        D_f = 4 * n_L**4 * s_prime**2 - 8 * n_L**3 * s_prime * (s_prime - t) + (n_L**2) * (d_L**2 + 4*(s_prime**2 + t**2) - 16 *s_prime*t) + n_L * (8*t*(s_prime - t) - 2*d_L**2) + 4*t**2
        #print(D_f)

        f_prime= (- 1 / (n_L*(n_L - 2))) * (t + n_L*s_prime - 0.5*np.sqrt(D_f))

        return (phi1*180/np.pi, phi2*180/np.pi, f_prime)
        #en radians :
        #return (phi1, phi2, f)
    */

}


void Poodle_window::onImageNPClicked(QMouseEvent* event){
    if(_camera.get_isCalibrated()){
        qDebug()<< event->x() << " " << event->y();

        unsigned int x_pixel = event->x()*_camera.getWidth()/_labelImageProcessed->width();
        unsigned int y_pixel = event->y()*_camera.getHeight()/_labelImageProcessed->height();

        qDebug() << "x:" << QString::number(x_pixel) << "px, y:" << QString::number(y_pixel) << "px";
        qDebug() << "x:" << QString::number(_camera.x_px2mm(x_pixel)) << "mm, y:" << QString::number(_camera.y_px2mm(y_pixel)) << "mm";

        double xmm = _camera.x_px2mm(x_pixel);
        double ymm = _camera.y_px2mm(y_pixel);

        double phi1;
        double phi2;
        double f;
        transfert_fct(110 - xmm , 248 - ymm, 820, phi1, phi2, f);

        _driver.go2position_angle(phi1, phi2);

        get_images();
    }

}

void Poodle_window::onImagePClicked(QMouseEvent* event){

}
