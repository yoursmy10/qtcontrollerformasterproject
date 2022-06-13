/********************************************************************************
** Form generated from reading UI file 'multiwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIWINDOW_H
#define UI_MULTIWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_multiwindowClass
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTableView *tableView;
    QWidget *tab_2;
    QCustomPlot *widget_2dgraph;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pushButton_start;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_end;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLabel *label;
    QTextEdit *textEdit_commu;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLineEdit *lineEdit_lat_bottom;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_14;
    QLineEdit *lineEdit_lat_top;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *lineEdit_serial;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_17;
    QLineEdit *lineEdit_hgt;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_10;
    QLineEdit *lineEdit_lon_init;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_11;
    QLineEdit *lineEdit_lat_init;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QLineEdit *lineEdit_ref_lon;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_9;
    QLineEdit *lineEdit_ref_lat;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_12;
    QLineEdit *lineEdit_heli_lat;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_13;
    QLineEdit *lineEdit_heli_lon;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_18;
    QLineEdit *lineEdit_LAD;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_19;
    QLineEdit *lineEdit_heli_velocity;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_15;
    QLineEdit *lineEdit_lon_left;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_16;
    QLineEdit *lineEdit_lon_right;
    QTextEdit *textEdit_detail;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_20;
    QLineEdit *lineEdit_heli_yaw;
    QLabel *LED;
    QTextEdit *textEdit_velocity;
    QCustomPlot *widget_velocity_LAD;
    QGroupBox *groupBox;
    QLineEdit *lineEdit_alert;
    QMenuBar *menuBar;
    QMenu *menuSTART;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *multiwindowClass)
    {
        if (multiwindowClass->objectName().isEmpty())
            multiwindowClass->setObjectName(QString::fromUtf8("multiwindowClass"));
        multiwindowClass->resize(1330, 868);
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        multiwindowClass->setFont(font);
        centralWidget = new QWidget(multiwindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(680, 40, 631, 271));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        tabWidget->setFont(font1);
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(236, 236, 236);"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tableView = new QTableView(tab);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(0, 0, 631, 231));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        widget_2dgraph = new QCustomPlot(centralWidget);
        widget_2dgraph->setObjectName(QString::fromUtf8("widget_2dgraph"));
        widget_2dgraph->setGeometry(QRect(10, 90, 661, 401));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 40, 661, 41));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        pushButton_start = new QPushButton(layoutWidget);
        pushButton_start->setObjectName(QString::fromUtf8("pushButton_start"));
        pushButton_start->setMaximumSize(QSize(16777215, 30));
        QFont font2;
        font2.setFamily(QString::fromUtf8("JetBrains Mono ExtraBold"));
        font2.setPointSize(9);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(10);
        pushButton_start->setFont(font2);
        pushButton_start->setStyleSheet(QString::fromUtf8("font: 81 9pt \"JetBrains Mono ExtraBold\";\n"
""));

        horizontalLayout_7->addWidget(pushButton_start);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        pushButton_end = new QPushButton(layoutWidget);
        pushButton_end->setObjectName(QString::fromUtf8("pushButton_end"));
        pushButton_end->setMaximumSize(QSize(16777215, 30));
        pushButton_end->setFont(font2);
        pushButton_end->setStyleSheet(QString::fromUtf8("font: 81 9pt \"JetBrains Mono ExtraBold\";"));

        horizontalLayout_7->addWidget(pushButton_end);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(12, 498, 657, 311));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("JetBrains Mono"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setUnderline(false);
        font3.setWeight(75);
        label->setFont(font3);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        textEdit_commu = new QTextEdit(layoutWidget1);
        textEdit_commu->setObjectName(QString::fromUtf8("textEdit_commu"));
        textEdit_commu->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";\n"
"color: rgb(0, 198, 0);"));

        gridLayout->addWidget(textEdit_commu, 1, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));

        gridLayout->addLayout(gridLayout_2, 6, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        QFont font4;
        font4.setFamily(QString::fromUtf8("JetBrains Mono"));
        font4.setPointSize(9);
        font4.setBold(true);
        font4.setItalic(true);
        font4.setWeight(75);
        label_8->setFont(font4);

        horizontalLayout_9->addWidget(label_8);

        lineEdit_lat_bottom = new QLineEdit(layoutWidget1);
        lineEdit_lat_bottom->setObjectName(QString::fromUtf8("lineEdit_lat_bottom"));
        lineEdit_lat_bottom->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_9->addWidget(lineEdit_lat_bottom);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_14 = new QLabel(layoutWidget1);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font4);

        horizontalLayout_10->addWidget(label_14);

        lineEdit_lat_top = new QLineEdit(layoutWidget1);
        lineEdit_lat_top->setObjectName(QString::fromUtf8("lineEdit_lat_top"));
        lineEdit_lat_top->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_10->addWidget(lineEdit_lat_top);


        horizontalLayout_9->addLayout(horizontalLayout_10);


        gridLayout->addLayout(horizontalLayout_9, 4, 1, 1, 1);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font3);
        label_2->setTextFormat(Qt::PlainText);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font4);

        horizontalLayout_8->addWidget(label_7);

        lineEdit_serial = new QLineEdit(layoutWidget1);
        lineEdit_serial->setObjectName(QString::fromUtf8("lineEdit_serial"));
        lineEdit_serial->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_8->addWidget(lineEdit_serial);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_17 = new QLabel(layoutWidget1);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font4);

        horizontalLayout_13->addWidget(label_17);

        lineEdit_hgt = new QLineEdit(layoutWidget1);
        lineEdit_hgt->setObjectName(QString::fromUtf8("lineEdit_hgt"));
        lineEdit_hgt->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_13->addWidget(lineEdit_hgt);


        horizontalLayout_8->addLayout(horizontalLayout_13);


        gridLayout->addLayout(horizontalLayout_8, 4, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_10 = new QLabel(layoutWidget1);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font4);

        horizontalLayout_3->addWidget(label_10);

        lineEdit_lon_init = new QLineEdit(layoutWidget1);
        lineEdit_lon_init->setObjectName(QString::fromUtf8("lineEdit_lon_init"));
        lineEdit_lon_init->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_3->addWidget(lineEdit_lon_init);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_11 = new QLabel(layoutWidget1);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font4);

        horizontalLayout_4->addWidget(label_11);

        lineEdit_lat_init = new QLineEdit(layoutWidget1);
        lineEdit_lat_init->setObjectName(QString::fromUtf8("lineEdit_lat_init"));
        lineEdit_lat_init->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_4->addWidget(lineEdit_lat_init);


        horizontalLayout_3->addLayout(horizontalLayout_4);


        gridLayout->addLayout(horizontalLayout_3, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font4);

        horizontalLayout->addWidget(label_6);

        lineEdit_ref_lon = new QLineEdit(layoutWidget1);
        lineEdit_ref_lon->setObjectName(QString::fromUtf8("lineEdit_ref_lon"));
        lineEdit_ref_lon->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout->addWidget(lineEdit_ref_lon);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font4);

        horizontalLayout_2->addWidget(label_9);

        lineEdit_ref_lat = new QLineEdit(layoutWidget1);
        lineEdit_ref_lat->setObjectName(QString::fromUtf8("lineEdit_ref_lat"));
        lineEdit_ref_lat->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_2->addWidget(lineEdit_ref_lat);


        horizontalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_12 = new QLabel(layoutWidget1);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font4);

        horizontalLayout_5->addWidget(label_12);

        lineEdit_heli_lat = new QLineEdit(layoutWidget1);
        lineEdit_heli_lat->setObjectName(QString::fromUtf8("lineEdit_heli_lat"));
        lineEdit_heli_lat->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_5->addWidget(lineEdit_heli_lat);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_13 = new QLabel(layoutWidget1);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font4);

        horizontalLayout_14->addWidget(label_13);

        lineEdit_heli_lon = new QLineEdit(layoutWidget1);
        lineEdit_heli_lon->setObjectName(QString::fromUtf8("lineEdit_heli_lon"));
        lineEdit_heli_lon->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_14->addWidget(lineEdit_heli_lon);


        horizontalLayout_5->addLayout(horizontalLayout_14);


        gridLayout->addLayout(horizontalLayout_5, 3, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_18 = new QLabel(layoutWidget1);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font4);

        horizontalLayout_15->addWidget(label_18);

        lineEdit_LAD = new QLineEdit(layoutWidget1);
        lineEdit_LAD->setObjectName(QString::fromUtf8("lineEdit_LAD"));
        lineEdit_LAD->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_15->addWidget(lineEdit_LAD);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_19 = new QLabel(layoutWidget1);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setFont(font4);

        horizontalLayout_16->addWidget(label_19);

        lineEdit_heli_velocity = new QLineEdit(layoutWidget1);
        lineEdit_heli_velocity->setObjectName(QString::fromUtf8("lineEdit_heli_velocity"));
        lineEdit_heli_velocity->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_16->addWidget(lineEdit_heli_velocity);


        horizontalLayout_15->addLayout(horizontalLayout_16);


        horizontalLayout_6->addLayout(horizontalLayout_15);


        gridLayout->addLayout(horizontalLayout_6, 3, 1, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_15 = new QLabel(layoutWidget1);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font4);

        horizontalLayout_11->addWidget(label_15);

        lineEdit_lon_left = new QLineEdit(layoutWidget1);
        lineEdit_lon_left->setObjectName(QString::fromUtf8("lineEdit_lon_left"));
        lineEdit_lon_left->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_11->addWidget(lineEdit_lon_left);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_16 = new QLabel(layoutWidget1);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font4);

        horizontalLayout_12->addWidget(label_16);

        lineEdit_lon_right = new QLineEdit(layoutWidget1);
        lineEdit_lon_right->setObjectName(QString::fromUtf8("lineEdit_lon_right"));
        lineEdit_lon_right->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_12->addWidget(lineEdit_lon_right);


        horizontalLayout_11->addLayout(horizontalLayout_12);


        gridLayout->addLayout(horizontalLayout_11, 5, 0, 1, 1);

        textEdit_detail = new QTextEdit(layoutWidget1);
        textEdit_detail->setObjectName(QString::fromUtf8("textEdit_detail"));
        textEdit_detail->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";\n"
"color: rgb(0, 198, 0);"));

        gridLayout->addWidget(textEdit_detail, 1, 1, 1, 1);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        label_20 = new QLabel(layoutWidget1);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font4);

        horizontalLayout_17->addWidget(label_20);

        lineEdit_heli_yaw = new QLineEdit(layoutWidget1);
        lineEdit_heli_yaw->setObjectName(QString::fromUtf8("lineEdit_heli_yaw"));
        lineEdit_heli_yaw->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";"));

        horizontalLayout_17->addWidget(lineEdit_heli_yaw);


        gridLayout->addLayout(horizontalLayout_17, 5, 1, 1, 1);

        LED = new QLabel(centralWidget);
        LED->setObjectName(QString::fromUtf8("LED"));
        LED->setGeometry(QRect(720, 370, 54, 12));
        textEdit_velocity = new QTextEdit(centralWidget);
        textEdit_velocity->setObjectName(QString::fromUtf8("textEdit_velocity"));
        textEdit_velocity->setGeometry(QRect(880, 350, 171, 61));
        textEdit_velocity->setStyleSheet(QString::fromUtf8("font: 9pt \"JetBrains Mono\";\n"
"color: rgb(0, 198, 0);"));
        widget_velocity_LAD = new QCustomPlot(centralWidget);
        widget_velocity_LAD->setObjectName(QString::fromUtf8("widget_velocity_LAD"));
        widget_velocity_LAD->setGeometry(QRect(680, 300, 631, 511));
        groupBox = new QGroupBox(widget_velocity_LAD);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 250, 631, 551));
        QFont font5;
        font5.setFamily(QString::fromUtf8("JetBrains Mono"));
        font5.setPointSize(10);
        font5.setBold(true);
        font5.setWeight(75);
        groupBox->setFont(font5);
        lineEdit_alert = new QLineEdit(widget_velocity_LAD);
        lineEdit_alert->setObjectName(QString::fromUtf8("lineEdit_alert"));
        lineEdit_alert->setGeometry(QRect(30, 130, 95, 22));
        lineEdit_alert->setStyleSheet(QString::fromUtf8("font: 12pt \"JetBrains Mono ExtraBold\";"));
        multiwindowClass->setCentralWidget(centralWidget);
        widget_velocity_LAD->raise();
        tabWidget->raise();
        widget_2dgraph->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        LED->raise();
        textEdit_velocity->raise();
        menuBar = new QMenuBar(multiwindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1330, 23));
        menuSTART = new QMenu(menuBar);
        menuSTART->setObjectName(QString::fromUtf8("menuSTART"));
        multiwindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(multiwindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        multiwindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(multiwindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        multiwindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuSTART->menuAction());

        retranslateUi(multiwindowClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(multiwindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *multiwindowClass)
    {
        multiwindowClass->setWindowTitle(QApplication::translate("multiwindowClass", "multiwindow", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("multiwindowClass", "\345\234\260\345\275\242\346\225\260\346\215\256\345\272\223\346\233\264\346\226\260\350\256\260\345\275\225\350\241\250", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QString());
        pushButton_start->setText(QApplication::translate("multiwindowClass", "START", nullptr));
        pushButton_end->setText(QApplication::translate("multiwindowClass", "END", nullptr));
        label->setText(QApplication::translate("multiwindowClass", "DEM_Update", nullptr));
        label_8->setText(QApplication::translate("multiwindowClass", "lat_bottom:", nullptr));
        label_14->setText(QApplication::translate("multiwindowClass", "lat_top:", nullptr));
        label_2->setText(QApplication::translate("multiwindowClass", "Communication", nullptr));
        label_7->setText(QApplication::translate("multiwindowClass", "serial:", nullptr));
        label_17->setText(QApplication::translate("multiwindowClass", "hgt:", nullptr));
        label_10->setText(QApplication::translate("multiwindowClass", "lon_init:", nullptr));
        label_11->setText(QApplication::translate("multiwindowClass", "lat_init:", nullptr));
        label_6->setText(QApplication::translate("multiwindowClass", "ref_lon:", nullptr));
        label_9->setText(QApplication::translate("multiwindowClass", "ref_lat:", nullptr));
        label_12->setText(QApplication::translate("multiwindowClass", "heli_lat", nullptr));
        label_13->setText(QApplication::translate("multiwindowClass", "heli_lon:", nullptr));
        label_18->setText(QApplication::translate("multiwindowClass", "LAD:", nullptr));
        label_19->setText(QApplication::translate("multiwindowClass", "velocity:", nullptr));
        label_15->setText(QApplication::translate("multiwindowClass", "lon_left:", nullptr));
        label_16->setText(QApplication::translate("multiwindowClass", "lon_right:", nullptr));
        label_20->setText(QApplication::translate("multiwindowClass", "heli_yaw:", nullptr));
        LED->setText(QApplication::translate("multiwindowClass", "TextLabel", nullptr));
        groupBox->setTitle(QApplication::translate("multiwindowClass", "Velocity&LAD Monitor", nullptr));
        menuSTART->setTitle(QApplication::translate("multiwindowClass", "START", nullptr));
    } // retranslateUi

};

namespace Ui {
    class multiwindowClass: public Ui_multiwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIWINDOW_H
