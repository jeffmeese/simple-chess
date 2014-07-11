/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStart_New_Game;
    QAction *actionExit;
    QAction *actionUndo_Last_Move;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuProgram;
    QMenu *menuGame;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        actionStart_New_Game = new QAction(MainWindow);
        actionStart_New_Game->setObjectName(QStringLiteral("actionStart_New_Game"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionUndo_Last_Move = new QAction(MainWindow);
        actionUndo_Last_Move->setObjectName(QStringLiteral("actionUndo_Last_Move"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        menuProgram = new QMenu(menuBar);
        menuProgram->setObjectName(QStringLiteral("menuProgram"));
        menuGame = new QMenu(menuBar);
        menuGame->setObjectName(QStringLiteral("menuGame"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuProgram->menuAction());
        menuBar->addAction(menuGame->menuAction());
        menuProgram->addAction(actionStart_New_Game);
        menuProgram->addAction(actionExit);
        menuGame->addAction(actionUndo_Last_Move);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionStart_New_Game->setText(QApplication::translate("MainWindow", "Start New Game", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionUndo_Last_Move->setText(QApplication::translate("MainWindow", "Undo Last Move", 0));
        menuProgram->setTitle(QApplication::translate("MainWindow", "Program", 0));
        menuGame->setTitle(QApplication::translate("MainWindow", "Game", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
