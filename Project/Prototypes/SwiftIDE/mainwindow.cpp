/*
 *  mainwindow.cpp
 *  Main Window
 */

#include <QDockWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QPushButton>
#include <Qsci/qsciscintilla.h>
#include "mainwindow.hpp"
#include "sceneeditorview.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    textEdit = new QsciScintilla;
    SceneEditorView* sceneEditorView = new SceneEditorView(this, QPoint(0, 0), QSize(640, 480));
    sceneEditorView->show();

    QMainWindow* window = new QMainWindow();
    QTabWidget* tabView = new QTabWidget(this);
    tabView->addTab(textEdit, QIcon("tab.png"), "Player  ");
    tabView->addTab(new QWidget(this), QIcon("tab.png"), "Player  ");
    tabView->addTab(new QWidget(this), QIcon("tab.png"), "Game  ");
    tabView->addTab(sceneEditorView, QIcon("tab.png"), "Main  ");
    tabView->setMovable(true);
    tabView->setTabsClosable(true);

    tabView->tabBar()->tabButton(1, QTabBar::RightSide)->resize(0, 0);
    tabView->tabBar()->tabButton(2, QTabBar::RightSide)->resize(0, 0);
    tabView->tabBar()->tabButton(3, QTabBar::RightSide)->resize(0, 0);

    window->setCentralWidget(tabView);
    window->setWindowFlags(Qt::Widget);
    setCentralWidget(window);

    QDockWidget* shellDock = new QDockWidget(tr("Error List"), this);
    shellDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    shellDock->setMinimumSize(480, 240);

    QListWidget* errorList = new QListWidget(shellDock);
    shellDock->setWidget(errorList);

    errorList->addItem("You talk like a fag, and your shit's all retarded.");

    window->addDockWidget(Qt::BottomDockWidgetArea, shellDock);

    // Example
    //QDockWidget *dockMain = new QDockWidget(this);
    //dockMain->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    //addDockWidget(Qt::RightDockWidgetArea, dockMain);
    //dockMain->setWidget(new QLabel("The left dock area", dockMain));
    // -------

    /*** Project Explorer ***/
    QDockWidget* dock = new QDockWidget(tr("Project Explorer"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setMinimumSize(240, 480);

    QTreeWidget* projectTreeView = new QTreeWidget(dock);
    dock->setWidget(projectTreeView);

    projectTreeView->setColumnCount(1);
    projectTreeView->setHeaderHidden(true);

    QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Sandbox Project")));
    item->setIcon(0, QIcon(tr(":/resources/icon/application.png")));
    projectTreeView->insertTopLevelItem(0, item);

    addDockWidget(Qt::RightDockWidgetArea, dock);
    /*** -------------- ***/

    /*
    QDockWidget* shellDock = new QDockWidget(tr("Error List"), this);
    shellDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    shellDock->setMinimumSize(480, 240);

    QListWidget* errorList = new QListWidget(shellDock);
    shellDock->setWidget(errorList);

    errorList->addItem("Your shit's all fucked and you talk like a fag.");

    window->addDockWidget(Qt::BottomDockWidgetArea, shellDock);

    QDockWidget* dock = new QDockWidget(tr("Project Explorer"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setMinimumSize(240, 480);

    QTreeWidget* projectTreeView = new QTreeWidget(dock);
    dock->setWidget(projectTreeView);

    projectTreeView->setColumnCount(1);
    projectTreeView->setHeaderHidden(true);

    QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Sandbox Project")));
    item->setIcon(0, QIcon(tr(":/resources/icon/application.png")));
    projectTreeView->insertTopLevelItem(0, item);

    window->addDockWidget(Qt::RightDockWidgetArea, dock);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
