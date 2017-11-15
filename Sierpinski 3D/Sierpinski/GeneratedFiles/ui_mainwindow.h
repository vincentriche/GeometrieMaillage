/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gldisplay.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSubmenu;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QPushButton *button1;
    QPushButton *button10;
    QPushButton *button7;
    QPushButton *button8;
    QPushButton *button6;
    QPushButton *button9;
    QVBoxLayout *verticalLayout;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QGridLayout *gridLayout_3;
    GLDisplay *openGLWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(895, 870);
        actionSubmenu = new QAction(MainWindow);
        actionSubmenu->setObjectName(QStringLiteral("actionSubmenu"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        button1 = new QPushButton(centralWidget);
        button1->setObjectName(QStringLiteral("button1"));

        gridLayout->addWidget(button1, 0, 1, 1, 1);

        button10 = new QPushButton(centralWidget);
        button10->setObjectName(QStringLiteral("button10"));

        gridLayout->addWidget(button10, 2, 1, 1, 1);

        button7 = new QPushButton(centralWidget);
        button7->setObjectName(QStringLiteral("button7"));

        gridLayout->addWidget(button7, 1, 0, 1, 1);

        button8 = new QPushButton(centralWidget);
        button8->setObjectName(QStringLiteral("button8"));

        gridLayout->addWidget(button8, 2, 0, 1, 1);

        button6 = new QPushButton(centralWidget);
        button6->setObjectName(QStringLiteral("button6"));

        gridLayout->addWidget(button6, 0, 0, 1, 1);

        button9 = new QPushButton(centralWidget);
        button9->setObjectName(QStringLiteral("button9"));

        gridLayout->addWidget(button9, 1, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        button2 = new QPushButton(centralWidget);
        button2->setObjectName(QStringLiteral("button2"));

        verticalLayout->addWidget(button2);

        button3 = new QPushButton(centralWidget);
        button3->setObjectName(QStringLiteral("button3"));

        verticalLayout->addWidget(button3);

        button4 = new QPushButton(centralWidget);
        button4->setObjectName(QStringLiteral("button4"));

        verticalLayout->addWidget(button4);

        button5 = new QPushButton(centralWidget);
        button5->setObjectName(QStringLiteral("button5"));

        verticalLayout->addWidget(button5);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout_2->addLayout(horizontalLayout, 2, 1, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        openGLWidget = new GLDisplay(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setMinimumSize(QSize(400, 650));

        gridLayout_3->addWidget(openGLWidget, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_3, 0, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_2);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        label->setFont(font1);
        label->setAutoFillBackground(false);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label);


        gridLayout_2->addLayout(horizontalLayout_3, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 895, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionSubmenu->setText(QApplication::translate("MainWindow", "submenu", Q_NULLPTR));
        button1->setText(QApplication::translate("MainWindow", "Toggle Wireframe", Q_NULLPTR));
        button10->setText(QApplication::translate("MainWindow", "Reset All", Q_NULLPTR));
        button7->setText(QApplication::translate("MainWindow", "Load Points File", Q_NULLPTR));
        button8->setText(QApplication::translate("MainWindow", "Load Off File", Q_NULLPTR));
        button6->setText(QApplication::translate("MainWindow", "Create Triangulation", Q_NULLPTR));
        button9->setText(QApplication::translate("MainWindow", "Save Triangulation (.OFF)", Q_NULLPTR));
        button2->setText(QApplication::translate("MainWindow", "Delaunay -  Lawson", Q_NULLPTR));
        button3->setText(QApplication::translate("MainWindow", "Toggle Voronoi", Q_NULLPTR));
        button4->setText(QApplication::translate("MainWindow", "Add Voronoi to Triangulation", Q_NULLPTR));
        button5->setText(QApplication::translate("MainWindow", "Crust", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Utilities Functions", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Triangulation Functions", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
