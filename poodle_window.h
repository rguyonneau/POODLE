#ifndef POODLE_WINDOW_H
#define POODLE_WINDOW_H

#include <QMainWindow>
#include <QString>
#include <QResizeEvent>

#include "motor_canopen_driver.h"
#include "seriallens.h"
#include "poodlecamera.h"
#include "log_handler.h"
#include "clickablelabel.h"


namespace Ui {
class Poodle_window;
}

class Poodle_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Poodle_window(QWidget *parent = 0);
    ~Poodle_window();

    void addLog(QString txt, int type);
    void updatelistwidget();
    void get_images();
    void transfert_fct(double x , double y, double z, double &phi1, double &phi2, double &f);

private slots:
    void on_pushButton_clicked();
    void slot_addLog(QString txt, int type);


    // Functions associated to the checkbox to hide/show log messages
    void on_cb_can_clicked();
    void on_cb_info_clicked();
    void on_cb_warn_clicked();
    void on_cb_err_clicked();

    void on_btn_cleanlogs_clicked();
    void on_btn_updateimage_clicked();

    void on_btn_weeding_clicked();

    void on_btn_laserON_clicked();

    void on_btn_laserOFF_clicked();

    void on_btn_initlens_clicked();

    void on_btn_setFocale_clicked();

    void on_btn_getpositions_clicked();

    void on_btn_setpositions_clicked();

    void on_btn_compute_tf_clicked();

    void onImageNPClicked(QMouseEvent*);
    void onImagePClicked(QMouseEvent*);

private:

    void resizeEvent( QResizeEvent *e );

    Log_handler* _logs;
    Ui::Poodle_window *ui;
    Motor_CANOpen_Driver _driver;
    SerialLens _lens;
    PoodleCamera _camera;

    ClickableLabel *_labelImageProcessed;
    ClickableLabel *_labelImageNonProcessed;

    QImage _imageProcessed;
    QImage _imageNonProcessed;

    bool _initonce;
};

#endif // POODLE_WINDOW_H
