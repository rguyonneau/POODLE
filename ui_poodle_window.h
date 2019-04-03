/********************************************************************************
** Form generated from reading UI file 'poodle_window.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POODLE_WINDOW_H
#define UI_POODLE_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Poodle_window
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QListWidget *lw_logs;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *cb_can;
    QCheckBox *cb_info;
    QCheckBox *cb_warn;
    QCheckBox *cb_err;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_cleanlogs;
    QTabWidget *tabWidget;
    QWidget *tab_demo;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout;
    QGroupBox *gb_imageP;
    QSpacerItem *verticalSpacer;
    QGroupBox *gb_imageNP;
    QPushButton *btn_updateimage;
    QPushButton *btn_weeding;
    QWidget *tab_manual;
    QGroupBox *gl_laser;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btn_laserON;
    QPushButton *btn_laserOFF;
    QGroupBox *gl_lens;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout_3;
    QPushButton *btn_initlens;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *txb_focale;
    QPushButton *btn_setFocale;
    QGroupBox *gb_mirrors;
    QGridLayout *gridLayout_8;
    QVBoxLayout *verticalLayout_4;
    QPushButton *btn_getpositions;
    QGridLayout *gridLayout_6;
    QLabel *lb_mirror1;
    QLineEdit *txb_pos2;
    QLabel *lb_pos1;
    QLabel *lb_mirror2;
    QLineEdit *txb_pos1;
    QLabel *lb_pos2;
    QPushButton *btn_setpositions;
    QGridLayout *gridLayout_7;
    QLabel *lb_offcet1_name;
    QLabel *lb_offcet1_val;
    QLabel *lb_offcet2_name;
    QLabel *lb_offcet2_val;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_9;
    QLineEdit *txb_x_mm;
    QLabel *label_13;
    QLineEdit *txb_z_mm;
    QLabel *label_12;
    QLabel *label_11;
    QLineEdit *txb_y_mm;
    QPushButton *btn_compute_tf;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Poodle_window)
    {
        if (Poodle_window->objectName().isEmpty())
            Poodle_window->setObjectName(QStringLiteral("Poodle_window"));
        Poodle_window->resize(679, 672);
        centralWidget = new QWidget(Poodle_window);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setMaximumSize(QSize(16777215, 16777215));
        groupBox_4->setFlat(false);
        groupBox_4->setCheckable(false);
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lw_logs = new QListWidget(groupBox_4);
        lw_logs->setObjectName(QStringLiteral("lw_logs"));
        lw_logs->setMaximumSize(QSize(16777215, 100));

        gridLayout_2->addWidget(lw_logs, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        cb_can = new QCheckBox(groupBox_4);
        cb_can->setObjectName(QStringLiteral("cb_can"));
        cb_can->setChecked(true);

        horizontalLayout_2->addWidget(cb_can);

        cb_info = new QCheckBox(groupBox_4);
        cb_info->setObjectName(QStringLiteral("cb_info"));
        cb_info->setChecked(true);

        horizontalLayout_2->addWidget(cb_info);

        cb_warn = new QCheckBox(groupBox_4);
        cb_warn->setObjectName(QStringLiteral("cb_warn"));
        cb_warn->setChecked(true);

        horizontalLayout_2->addWidget(cb_warn);

        cb_err = new QCheckBox(groupBox_4);
        cb_err->setObjectName(QStringLiteral("cb_err"));
        cb_err->setChecked(true);

        horizontalLayout_2->addWidget(cb_err);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        btn_cleanlogs = new QPushButton(groupBox_4);
        btn_cleanlogs->setObjectName(QStringLiteral("btn_cleanlogs"));

        gridLayout_2->addWidget(btn_cleanlogs, 3, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_4, 1, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_demo = new QWidget();
        tab_demo->setObjectName(QStringLiteral("tab_demo"));
        gridLayout = new QGridLayout(tab_demo);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButton = new QPushButton(tab_demo);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gb_imageP = new QGroupBox(tab_demo);
        gb_imageP->setObjectName(QStringLiteral("gb_imageP"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gb_imageP->sizePolicy().hasHeightForWidth());
        gb_imageP->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(gb_imageP);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout->addItem(verticalSpacer);

        gb_imageNP = new QGroupBox(tab_demo);
        gb_imageNP->setObjectName(QStringLiteral("gb_imageNP"));
        sizePolicy1.setHeightForWidth(gb_imageNP->sizePolicy().hasHeightForWidth());
        gb_imageNP->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(gb_imageNP);


        verticalLayout->addLayout(horizontalLayout);

        btn_updateimage = new QPushButton(tab_demo);
        btn_updateimage->setObjectName(QStringLiteral("btn_updateimage"));

        verticalLayout->addWidget(btn_updateimage);

        btn_weeding = new QPushButton(tab_demo);
        btn_weeding->setObjectName(QStringLiteral("btn_weeding"));

        verticalLayout->addWidget(btn_weeding);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        tabWidget->addTab(tab_demo, QString());
        tab_manual = new QWidget();
        tab_manual->setObjectName(QStringLiteral("tab_manual"));
        gl_laser = new QGroupBox(tab_manual);
        gl_laser->setObjectName(QStringLiteral("gl_laser"));
        gl_laser->setGeometry(QRect(10, 10, 106, 93));
        gridLayout_4 = new QGridLayout(gl_laser);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        btn_laserON = new QPushButton(gl_laser);
        btn_laserON->setObjectName(QStringLiteral("btn_laserON"));

        verticalLayout_2->addWidget(btn_laserON);

        btn_laserOFF = new QPushButton(gl_laser);
        btn_laserOFF->setObjectName(QStringLiteral("btn_laserOFF"));

        verticalLayout_2->addWidget(btn_laserOFF);


        gridLayout_4->addLayout(verticalLayout_2, 0, 0, 1, 1);

        gl_lens = new QGroupBox(tab_manual);
        gl_lens->setObjectName(QStringLiteral("gl_lens"));
        gl_lens->setGeometry(QRect(230, 20, 288, 95));
        gridLayout_5 = new QGridLayout(gl_lens);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        btn_initlens = new QPushButton(gl_lens);
        btn_initlens->setObjectName(QStringLiteral("btn_initlens"));

        verticalLayout_3->addWidget(btn_initlens);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(gl_lens);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        txb_focale = new QLineEdit(gl_lens);
        txb_focale->setObjectName(QStringLiteral("txb_focale"));
        txb_focale->setEnabled(false);

        horizontalLayout_3->addWidget(txb_focale);

        btn_setFocale = new QPushButton(gl_lens);
        btn_setFocale->setObjectName(QStringLiteral("btn_setFocale"));
        btn_setFocale->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btn_setFocale->sizePolicy().hasHeightForWidth());
        btn_setFocale->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(btn_setFocale);


        verticalLayout_3->addLayout(horizontalLayout_3);


        gridLayout_5->addLayout(verticalLayout_3, 0, 0, 1, 1);

        gb_mirrors = new QGroupBox(tab_manual);
        gb_mirrors->setObjectName(QStringLiteral("gb_mirrors"));
        gb_mirrors->setGeometry(QRect(40, 130, 266, 193));
        gridLayout_8 = new QGridLayout(gb_mirrors);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        btn_getpositions = new QPushButton(gb_mirrors);
        btn_getpositions->setObjectName(QStringLiteral("btn_getpositions"));

        verticalLayout_4->addWidget(btn_getpositions);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        lb_mirror1 = new QLabel(gb_mirrors);
        lb_mirror1->setObjectName(QStringLiteral("lb_mirror1"));

        gridLayout_6->addWidget(lb_mirror1, 0, 0, 1, 1);

        txb_pos2 = new QLineEdit(gb_mirrors);
        txb_pos2->setObjectName(QStringLiteral("txb_pos2"));

        gridLayout_6->addWidget(txb_pos2, 1, 2, 1, 1);

        lb_pos1 = new QLabel(gb_mirrors);
        lb_pos1->setObjectName(QStringLiteral("lb_pos1"));
        lb_pos1->setEnabled(false);

        gridLayout_6->addWidget(lb_pos1, 0, 1, 1, 1);

        lb_mirror2 = new QLabel(gb_mirrors);
        lb_mirror2->setObjectName(QStringLiteral("lb_mirror2"));

        gridLayout_6->addWidget(lb_mirror2, 1, 0, 1, 1);

        txb_pos1 = new QLineEdit(gb_mirrors);
        txb_pos1->setObjectName(QStringLiteral("txb_pos1"));

        gridLayout_6->addWidget(txb_pos1, 0, 2, 1, 1);

        lb_pos2 = new QLabel(gb_mirrors);
        lb_pos2->setObjectName(QStringLiteral("lb_pos2"));
        lb_pos2->setEnabled(false);

        gridLayout_6->addWidget(lb_pos2, 1, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_6);

        btn_setpositions = new QPushButton(gb_mirrors);
        btn_setpositions->setObjectName(QStringLiteral("btn_setpositions"));

        verticalLayout_4->addWidget(btn_setpositions);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        lb_offcet1_name = new QLabel(gb_mirrors);
        lb_offcet1_name->setObjectName(QStringLiteral("lb_offcet1_name"));
        lb_offcet1_name->setEnabled(false);

        gridLayout_7->addWidget(lb_offcet1_name, 0, 0, 1, 1);

        lb_offcet1_val = new QLabel(gb_mirrors);
        lb_offcet1_val->setObjectName(QStringLiteral("lb_offcet1_val"));
        lb_offcet1_val->setEnabled(false);

        gridLayout_7->addWidget(lb_offcet1_val, 0, 1, 1, 1);

        lb_offcet2_name = new QLabel(gb_mirrors);
        lb_offcet2_name->setObjectName(QStringLiteral("lb_offcet2_name"));
        lb_offcet2_name->setEnabled(false);

        gridLayout_7->addWidget(lb_offcet2_name, 1, 0, 1, 1);

        lb_offcet2_val = new QLabel(gb_mirrors);
        lb_offcet2_val->setObjectName(QStringLiteral("lb_offcet2_val"));
        lb_offcet2_val->setEnabled(false);

        gridLayout_7->addWidget(lb_offcet2_val, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer, 0, 2, 1, 1);


        verticalLayout_4->addLayout(gridLayout_7);


        gridLayout_8->addLayout(verticalLayout_4, 0, 0, 1, 1);

        groupBox = new QGroupBox(tab_manual);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(350, 150, 201, 149));
        gridLayout_10 = new QGridLayout(groupBox);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setSpacing(6);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        txb_x_mm = new QLineEdit(groupBox);
        txb_x_mm->setObjectName(QStringLiteral("txb_x_mm"));

        gridLayout_9->addWidget(txb_x_mm, 0, 1, 1, 1);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_9->addWidget(label_13, 2, 0, 1, 1);

        txb_z_mm = new QLineEdit(groupBox);
        txb_z_mm->setObjectName(QStringLiteral("txb_z_mm"));

        gridLayout_9->addWidget(txb_z_mm, 2, 1, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_9->addWidget(label_12, 1, 0, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_9->addWidget(label_11, 0, 0, 1, 1);

        txb_y_mm = new QLineEdit(groupBox);
        txb_y_mm->setObjectName(QStringLiteral("txb_y_mm"));

        gridLayout_9->addWidget(txb_y_mm, 1, 1, 1, 1);


        gridLayout_10->addLayout(gridLayout_9, 0, 0, 1, 1);

        btn_compute_tf = new QPushButton(groupBox);
        btn_compute_tf->setObjectName(QStringLiteral("btn_compute_tf"));

        gridLayout_10->addWidget(btn_compute_tf, 1, 0, 1, 1);

        tabWidget->addTab(tab_manual, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 1);

        Poodle_window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Poodle_window);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 679, 19));
        Poodle_window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Poodle_window);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Poodle_window->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Poodle_window);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Poodle_window->setStatusBar(statusBar);

        retranslateUi(Poodle_window);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Poodle_window);
    } // setupUi

    void retranslateUi(QMainWindow *Poodle_window)
    {
        Poodle_window->setWindowTitle(QApplication::translate("Poodle_window", "Poodle_window", 0));
        groupBox_4->setTitle(QApplication::translate("Poodle_window", "Logs", 0));
        cb_can->setText(QApplication::translate("Poodle_window", "CAN", 0));
        cb_info->setText(QApplication::translate("Poodle_window", "INFO", 0));
        cb_warn->setText(QApplication::translate("Poodle_window", "WARN", 0));
        cb_err->setText(QApplication::translate("Poodle_window", "ERR", 0));
        btn_cleanlogs->setText(QApplication::translate("Poodle_window", "Clean", 0));
        pushButton->setText(QApplication::translate("Poodle_window", "Init", 0));
        gb_imageP->setTitle(QApplication::translate("Poodle_window", "Image processed", 0));
        gb_imageNP->setTitle(QApplication::translate("Poodle_window", "Image not processed", 0));
        btn_updateimage->setText(QApplication::translate("Poodle_window", "Update Image", 0));
        btn_weeding->setText(QApplication::translate("Poodle_window", "Weeding", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_demo), QApplication::translate("Poodle_window", "Demo", 0));
        gl_laser->setTitle(QApplication::translate("Poodle_window", "Laser", 0));
        btn_laserON->setText(QApplication::translate("Poodle_window", "ON", 0));
        btn_laserOFF->setText(QApplication::translate("Poodle_window", "OFF", 0));
        gl_lens->setTitle(QApplication::translate("Poodle_window", "Lens", 0));
        btn_initlens->setText(QApplication::translate("Poodle_window", "Init", 0));
        label_2->setText(QApplication::translate("Poodle_window", "Focale:", 0));
        btn_setFocale->setText(QApplication::translate("Poodle_window", "Set", 0));
        gb_mirrors->setTitle(QApplication::translate("Poodle_window", "Mirrors", 0));
        btn_getpositions->setText(QApplication::translate("Poodle_window", "Get Positions", 0));
        lb_mirror1->setText(QApplication::translate("Poodle_window", "Mirror 1", 0));
        lb_pos1->setText(QApplication::translate("Poodle_window", "<value>", 0));
        lb_mirror2->setText(QApplication::translate("Poodle_window", "Mirror 2", 0));
        lb_pos2->setText(QApplication::translate("Poodle_window", "<value>", 0));
        btn_setpositions->setText(QApplication::translate("Poodle_window", "Set Positions", 0));
        lb_offcet1_name->setText(QApplication::translate("Poodle_window", "Offcet 1", 0));
        lb_offcet1_val->setText(QApplication::translate("Poodle_window", "<value>", 0));
        lb_offcet2_name->setText(QApplication::translate("Poodle_window", "Offcet 2", 0));
        lb_offcet2_val->setText(QApplication::translate("Poodle_window", "<value>", 0));
        groupBox->setTitle(QApplication::translate("Poodle_window", "Transfert function", 0));
        label_13->setText(QApplication::translate("Poodle_window", "z (mm)", 0));
        label_12->setText(QApplication::translate("Poodle_window", "y (mm)", 0));
        label_11->setText(QApplication::translate("Poodle_window", "x (mm)", 0));
        btn_compute_tf->setText(QApplication::translate("Poodle_window", "Compute", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_manual), QApplication::translate("Poodle_window", "Manual Control", 0));
    } // retranslateUi

};

namespace Ui {
    class Poodle_window: public Ui_Poodle_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POODLE_WINDOW_H
