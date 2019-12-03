/*
 *  MainWindow.cpp
 *  Main Window
 */

#include <QMessageBox>
#include <MainWindow.hpp>
#include <Dialogs/WelcomeDialog.hpp>
#include <Project/Project.hpp>
#include <IdePreferences.hpp>
#include <Compiler.hpp>

MainWindow::MainWindow(const QStringList& cmdLineArguments, QWidget* parent)
    : QMainWindow(parent),
	  ui(new Ui::MainWindow),
	  gameIsRunning(false)
{
    // Prepare main window UI
    ui->setupUi(this);
    SetupUi();
    SetupConnections();

    // Link up to compiler
    DiscoverCompiler();

	setGeometry(IdePreferences.windowPosition.x() + 6,
				IdePreferences.windowPosition.y() + 28,
				IdePreferences.windowSize.width(),
				IdePreferences.windowSize.height());
	setGeometry(pos().x() + frameGeometry().width() - geometry().width(),
				pos().y() + frameGeometry().height() - geometry().height(),
				geometry().width(),
				geometry().height());

	// Show welcome dialog if we're not loading a project
	if (IdePreferences.showWelcomeDialog && cmdLineArguments.empty())
		(new WelcomeDialog(true, this))->show();
	else if (!cmdLineArguments.empty())
	{
		// Try to open it but we need to wait for the event pump to be running
		QTimer* timer = new QTimer(this);
		timer->setSingleShot(true);

		QMessageBox::information(this, "Count", QString::number(cmdLineArguments.size()));
		for(int i = 0; i < cmdLineArguments.size(); ++i)
			QMessageBox::information(this, "Argv " + QString::number(i),
									 cmdLineArguments[i]);

		connect(timer, &QTimer::timeout,
		[=]()
		{
			if (!Project.Open(cmdLineArguments[0]))
					QMessageBox::critical(this, "Error", "Cannot open the project. It may have been "
														 "created with an older version of Swift.");
			timer->deleteLater();
		});

		timer->start(1);
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::SetupUi()
{
    // Create dock areas
    CreateDockAreas();

    // Create shell and project explorer
    CreateShellDock();
    CreateProjectExplorer();
}

void MainWindow::SetupConnections()
{
    // Link menu bar's actions
    SetupMenuBar();

    // Workspace Tab View Close Tab
    connect(workspaceTabView, &QTabWidget::tabCloseRequested, [=](int index)
    {
        QWidget* widget = workspaceTabView->widget(index);
        if (widget->close())
        {
			AssetForm* assetForm = dynamic_cast<AssetForm*>(widget);
			if (assetForm != nullptr)
				assetForm->TabFocusOut();

			workspaceTabView->removeTab(index);
            delete widget;
        }
    });

	// Tab index change event dispatching
	connect(workspaceTabView, &QTabWidget::currentChanged, [=](int index)
	{
		// Unfocus all tabs
		for(int i = 0; i < workspaceTabView->count(); ++i)
		{
		   AssetForm* assetForm = dynamic_cast<AssetForm*>(workspaceTabView->widget(i));
		   if (assetForm == nullptr)
			   continue;

		   assetForm->TabFocusOut();
		}

		// Focus new tab
		AssetForm* assetForm = dynamic_cast<AssetForm*>(workspaceTabView->widget(index));
		if (assetForm == nullptr)
			return;

		assetForm->TabFocusIn();
	});

    // Shell Tab View Close Tab
    connect(shellTabView, &QTabWidget::tabCloseRequested, shellTabView, &QTabWidget::removeTab);
}

void MainWindow::SyncWithProject()
{
#ifdef QT_NO_DEBUG
    setWindowTitle(Project.project->targetName + " - Swift IDE");
#else
	setWindowTitle(Project.project->targetName + " - Swift IDE [dev build]");
#endif
    workspaceTabView->clear();
    SyncProjectExplorer();
}

void MainWindow::CreateDockAreas()
{
    // We create a window that will act as a container for the shell and workspace docks
    // this is to get around a retarded limitation of Qt.
	workspaceWindow = new QMainWindow(this);
        workspaceWindow->setWindowFlags(Qt::Widget);
	setCentralWidget(workspaceWindow);

    // Create workspace tab view so the docks are laid out properly
    workspaceTabView = new WorkspaceTabWidget(this);
        workspaceTabView->setMovable(true);
		workspaceTabView->setTabsClosable(true);

    workspaceWindow->setCentralWidget(workspaceTabView);
}

void MainWindow::CreateShellDock()
{
    // Shell dock (Error List, Compiler Output, Engine Debug Redirect)
    shellDock = new QDockWidget(tr("Shell"), this);
        shellDock->setAllowedAreas(Qt::BottomDockWidgetArea);
        shellDock->setMinimumSize(480, 184);
        shellDock->hide();
    workspaceWindow->addDockWidget(Qt::BottomDockWidgetArea, shellDock);

    // Tab view for shell dock
    shellTabView = new QTabWidget(shellDock);
        shellTabView->setMovable(true);
        shellTabView->setTabsClosable(true);
    shellDock->setWidget(shellTabView);

    // Create Compiler Output
	QFont font("Consolas", 10, 1, false);
    compileOutput = new QTextEdit(shellTabView);
        compileOutput->setReadOnly(true);
		compileOutput->setFont(font);
		compileOutput->setTabStopWidth(QFontMetrics(font).width(' ') * 4);
        compileOutput->hide();

    // Create Engine Debug Redirect
    engineDebugOutput = new QTextEdit(shellTabView);
        engineDebugOutput->setReadOnly(true);
		engineDebugOutput->setFont(font);
		engineDebugOutput->setTabStopWidth(QFontMetrics(font).width(' ') * 4);
        engineDebugOutput->hide();

	if (IdePreferences.showCompileOutput)
		ShowCompilerOutput();

	if (IdePreferences.showEngineDebug)
		ShowEngineDebug();
}

void MainWindow::DiscoverCompiler()
{
    if (Compiler.Discover())
        return;

    QMessageBox::critical(this, "Error", "Cannot locate Swift compiler for linkage. The IDE cannot finish initializing.");
    close();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	// Ask if we really want to close if a game is still running
	if (gameIsRunning)
	{
		int index = QMessageBox::question(this, "Game is running.", "A game is current running, would you like to exit Swift IDE anyway?",
										  QMessageBox::Ok, QMessageBox::Cancel);
		switch(index)
		{
			case QMessageBox::Ok:
				break;

			case QMessageBox::Cancel:
			default:
				event->ignore();
				return;
		}
	}

	// Try and close each asset and abort close if we have unsaved work we want to save
    for(int i = 0; i < workspaceTabView->count(); ++i)
    {
        if (!workspaceTabView->widget(i)->close())
        {
            event->ignore();
            return;
        }

        workspaceTabView->removeTab(i);
    }

	// Save implicit settings
	IdePreferences.windowMaximized = (windowState() & Qt::WindowMaximized) == Qt::WindowMaximized;
	IdePreferences.windowPosition = pos();
	IdePreferences.windowSize = size();
	IdePreferences.showProjectExplorer = projectTreeDock->isVisible();
	IdePreferences.showCompileOutput = shellTabView->indexOf(compileOutput) != -1;
	IdePreferences.showEngineDebug = shellTabView->indexOf(engineDebugOutput) != -1;
	IdePreferences.projectExplorerDock = dockWidgetArea(projectTreeDock) == Qt::LeftDockWidgetArea;
	IdePreferences.Save();

    event->accept();
}

void MainWindow::ShowCompilerOutput()
{
    int index = shellTabView->addTab(compileOutput, QIcon(":/icons/shell_windows.png"),
                                     "Compiler Output");
    compileOutput->show();
    shellDock->show();
    shellTabView->setCurrentIndex(index);
}

void MainWindow::ShowEngineDebug()
{
    int tindex = shellTabView->addTab(engineDebugOutput, QIcon(":/icons/bug.png"),
                                     "Engine Debug");
    engineDebugOutput->show();
    shellDock->show();
    shellTabView->setCurrentIndex(tindex);
}
