/*
 *  MainWindow.hpp
 *  Main Window
 */

#ifndef __MAIN_WINDOW_HPP
#define __MAIN_WINDOW_HPP

#include <QProcess>
#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QCloseEvent>
#include <Forms/AssetForm.hpp>
#include "ui_mainwindow.h"

// TODO Prototype and body should be split
class WorkspaceTabBar : public QTabBar
{
    Q_OBJECT

public:
    WorkspaceTabBar(QWidget* parent = nullptr)
        : QTabBar(parent)
    {

    }

    void mouseReleaseEvent(QMouseEvent* ev)
    {
        if (ev->button() == Qt::MidButton)
            tabCloseRequested(tabAt(ev->pos()));

        QTabBar::mouseReleaseEvent(ev);
    }
};

class WorkspaceTabWidget : public QTabWidget
{
    Q_OBJECT

private:
    WorkspaceTabBar* wTabBar;
    QShortcut* toggleForwardShortcut;
    QShortcut* toggleBackwardsShortcut;

public:
    explicit WorkspaceTabWidget(QWidget* parent = nullptr)
        : QTabWidget(parent),
          wTabBar(new WorkspaceTabBar(this)),
          toggleForwardShortcut(nullptr),
          toggleBackwardsShortcut(nullptr)
    {
        setTabBar(wTabBar);

        toggleForwardShortcut = new QShortcut(QKeySequence("Ctrl+Tab"), this);
        connect(toggleForwardShortcut, &QShortcut::activated, [=]()
        {
            if (count() == 0)
                return;

            int tabIndex = (currentIndex() + 1) % count();
            setCurrentIndex(tabIndex);
        });

        toggleBackwardsShortcut = new QShortcut(QKeySequence("Ctrl+Shift+Tab"), this);
        connect(toggleBackwardsShortcut, &QShortcut::activated, [=]()
        {
            if (count() == 0)
                return;

            int tabIndex = currentIndex() - 1;
            if (tabIndex == -1)
                tabIndex = count() - 1;

            setCurrentIndex(tabIndex);
        });
    }

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	Ui::MainWindow* ui;

private:
	QMainWindow* workspaceWindow;
    QDockWidget* workspaceDock;
    QDockWidget* projectTreeDock;

public:
	QDockWidget* shellDock;
    WorkspaceTabWidget* workspaceTabView;

private:
    QTabWidget* shellTabView;
    QTextEdit* compileOutput;

public:
    QTreeWidgetItem* texturesFolder;
    QTreeWidgetItem* soundsFolder;
    QTreeWidgetItem* fontsFolder;
    QTreeWidgetItem* scenesFolder;
    QTreeWidgetItem* scriptsFolder;

private:
	bool gameIsRunning;

    void SetupUi();
    void SetupConnections();

    void SetupMenuBar();
    void SetupMenuBarFile();
    void SetupMenuBarAssets();
    void SetupMenuBarBuild();
    void SetupMenuBarWindow();
    void SetupMenuBarAnalyze();
    void SetupMenuBarHelp();

    // User Interface
    void CreateDockAreas();
    void CreateProjectExplorer();
    void CreateShellDock();

    // Backend
    void DiscoverCompiler();

    void SyncProjectExplorer();

    void ShowCompilerOutput();
    void ShowEngineDebug();

    QProcess* CompileGame();

    void ProjectExplorerRename(QTreeWidgetItem* item);

    void RunToError();
    void ClearErrors();

private slots:
    void Save(QWidget* widget);
    void SaveAll();
    void CreateNewProject();
    void OpenProject();
    void OpenSwiftInExplorer();
    void OpenProjectInExplorer();

    void CompileAndRunGame();

public:
    void ProjectExplorerItemClicked(QTreeWidgetItem* item, int index);
    void ProjectExplorerAddItem(AssetLink* assetLink, const QIcon& icon);

private:
    void ProjectExplorerDelete();

public:
    QTreeWidget* projectTree;
    QTextEdit* engineDebugOutput;

	MainWindow(const QStringList& cmdLineArguments, QWidget *parent = nullptr);
    ~MainWindow();

    void SyncWithProject();

    void closeEvent(QCloseEvent *event);
};

#endif
