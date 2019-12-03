/*
 *  MainWindow.MenuBar.cpp
 *  Main Window's Menu Bar
 */

#include <QDesktopServices>
#include <MainWindow.hpp>
#include <Dialogs/ConfigurationDialog.hpp>
#include <Dialogs/NewProjectDialog.hpp>
#include <Dialogs/NewAssetDialog.hpp>
#include <Dialogs/AboutDialog.hpp>
#include <Dialogs/IdePreferencesDialog.hpp>
#include <Dialogs/WelcomeDialog.hpp>
#include <Forms/AssetForm.hpp>
#include <Forms/ScriptEditor.hpp>
#include <Project/Project.hpp>
#include <IdePreferences.hpp>
#include <Compiler.hpp>
#include <Util.hpp>

void MainWindow::SetupMenuBar()
{
    SetupMenuBarFile();
    SetupMenuBarAssets();
    SetupMenuBarBuild();
    SetupMenuBarWindow();
    SetupMenuBarAnalyze();
    SetupMenuBarHelp();
}

void MainWindow::SetupMenuBarFile()
{
    connect(ui->actionNew_Project, &QAction::triggered, this, &MainWindow::CreateNewProject);
    connect(ui->actionOpen_Project, &QAction::triggered, this, &MainWindow::OpenProject);
    connect(ui->actionSave, &QAction::triggered, [=]() { Save(workspaceTabView->currentWidget()); });
    connect(ui->actionSave_All, &QAction::triggered, this, &MainWindow::SaveAll);
	connect(ui->actionPreferences, &QAction::triggered, [=]()
	{
		(new IdePreferencesDialog(this))->show();
	});

    connect(ui->actionOpen_Swift_in_Explorer, &QAction::triggered, this, &MainWindow::OpenSwiftInExplorer);
    connect(ui->actionExit, &QAction::triggered, &QApplication::exit);
}

void MainWindow::SetupMenuBarAssets()
{
    connect(ui->actionAdd_Texture, &QAction::triggered, [=]()
        { (new NewAssetDialog(ProjectAssetType::Texture, this))->show(); });
    connect(ui->actionAdd_Sound, &QAction::triggered, [=]()
        { (new NewAssetDialog(ProjectAssetType::Sound, this))->show(); });
    connect(ui->actionAdd_Scene, &QAction::triggered, [=]()
        { (new NewAssetDialog(ProjectAssetType::Scene, this))->show(); });
    connect(ui->actionAdd_Script, &QAction::triggered, [=]()
        { (new NewAssetDialog(ProjectAssetType::Script, this))->show(); });
    connect(ui->actionConfiguration, &QAction::triggered, [=]()
    {
        (new ConfigurationDialog(Project.configuration, this))->show();
    });

    connect(ui->actionOpen_Project_in_Explorer, &QAction::triggered, this, &MainWindow::OpenProjectInExplorer);
}

void MainWindow::SetupMenuBarBuild()
{
    connect(ui->actionBuild_and_Run, &QAction::triggered, this, &MainWindow::CompileAndRunGame);
    connect(ui->actionBuild, &QAction::triggered, this, &MainWindow::CompileGame);
}

void MainWindow::SetupMenuBarWindow()
{
    connect(ui->actionProject_Explorer, &QAction::triggered, projectTreeDock, &QTreeWidget::show);
    connect(ui->actionCompiler_Output, &QAction::triggered, this, &MainWindow::ShowCompilerOutput);
    connect(ui->actionEngine_Debug_Redirect, &QAction::triggered, this, &MainWindow::ShowEngineDebug);
	connect(ui->actionWelcome_Dialog, &QAction::triggered, [=]()
	{
		(new WelcomeDialog(false, this))->show();
	});
}

void MainWindow::SetupMenuBarAnalyze()
{

}

void MainWindow::SetupMenuBarHelp()
{
	connect(ui->actionSwift_Docs, &QAction::triggered, []()
    {
		QDesktopServices::openUrl(QUrl("http://www.x2048.com/swift/docs/index.php"));
    });

    connect(ui->actionRelease_Notes, &QAction::triggered, [=]()
    {
		QDesktopServices::openUrl(QUrl("http://www.x2048.com/swift/docs/index.php?path=ReleaseNotes.md"));
    });

    connect(ui->actionWebsite, &QAction::triggered, [=]()
    {
        QDesktopServices::openUrl(QUrl("http://www.x2048.com/swift/"));
    });

    connect(ui->actionDeveloper_Forum, &QAction::triggered, [=]()
    {
        QDesktopServices::openUrl(QUrl("http://www.x2048.com/swift/forums/"));
    });

    connect(ui->actionReport_bug, &QAction::triggered, [=]()
    {
        QDesktopServices::openUrl(QUrl("http://www.x2048.com/swift/bugs/"));
    });

    connect(ui->actionAbout_Swift_IDE, &QAction::triggered, [=]()
    {
        (new AboutDialog(this))->show();
    });
}

void MainWindow::Save(QWidget* widget)
{
    // Get tab widget
    AssetForm* assetForm = dynamic_cast<AssetForm*>(widget);
    if (assetForm == nullptr)
        return;

    // Call asset specific save
    assetForm->Save();
}

void MainWindow::SaveAll()
{
    // Go through all tabs and save them
    for(int i = 0, j = workspaceTabView->count(); j > 0; ++i)
    {
        QWidget* widget = workspaceTabView->widget(i);
        if (widget == nullptr)
            continue;

        Save(widget);
        --j;
    }
}

void MainWindow::CreateNewProject()
{
    (new NewProjectDialog((QWidget*)parent()))->show();
}

void MainWindow::OpenProject()
{
    // Get project path
    QString path = QFileDialog::getOpenFileName(this, "Open Project",
												QDir::currentPath(), "Swift Project (*.sproj)");
    if (path == "")
        return;

    // Try and load it
    if (!Project.Open(path))
        QMessageBox::critical(this, "Error", "Failed to load the project.");
}

void MainWindow::OpenSwiftInExplorer()
{
    Util::OpenPathInShell(QDir().absolutePath());
}

void MainWindow::OpenProjectInExplorer()
{
    // Don't open project if there is none
    if (!Project.IsOpen() || Project.projectRootDirectory == "")
        return;

    // Open the project's directory in the native shell
    Util::OpenPathInShell(Project.projectRootDirectory);
}

QProcess* MainWindow::CompileGame()
{
	// We can't compile something that isn't opened...
	if (!Project.IsOpen())
		return nullptr;

	// Set status message
	statusBar()->showMessage("Compiling...");

    // Save all files before we compile the game
    SaveAll();
    ClearErrors();

    // Show the compiler window
    compileOutput->clear();
    ShowCompilerOutput();

    // Create arguments list
    QStringList arguments;
    arguments << "compile" << Project.projectRootDirectory;
	if (IdePreferences.compileDisassemble)
		arguments << "--disassemble";

    // Compile the game
    QProcess* process = Compiler.ExecuteThreaded(arguments);
    connect(process, &QProcess::readyRead, [=]()
    {
        // Read line from process and write it to console
        QString line = Util::RStrip(process->readAll());
        compileOutput->append(line);

        // Make sure cursor's at the end of the log
        QTextCursor cursor = compileOutput->textCursor();
        cursor.movePosition(QTextCursor::End);
        compileOutput->setTextCursor(cursor);
    });

    connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished),
    [=](int returnCode)
    {
        if (returnCode != 0)
            return;

        RunToError();

		statusBar()->clearMessage();

		process->deleteLater();
    });

    // Also return process
    return process;
}

void MainWindow::CompileAndRunGame()
{
    // Compile game
    QProcess* compileProcess = CompileGame();
	if (compileProcess == nullptr)
		return;

    connect(compileProcess, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=](int returnCode)
    {
        if (returnCode != 0)
        {
            compileOutput->append("Compiler returned error code " + QString::number(returnCode) + ".");
            compileOutput->append("Aborting Compile & Run.");
            return;
        }

        // Do not run if there's no game data file (it failed to compile)
        QFile gameData(Project.GetTargetPath());
        if (!gameData.exists())
            return;

        // Show Engine Debug Output
        engineDebugOutput->clear();
        ShowEngineDebug();

        // Create arguments list
        QStringList arguments;
        arguments << "run" << Project.projectRootDirectory;

        if (ui->actionMute->isChecked())
            arguments << "--mute";

        // Start process
        QProcess* process = Compiler.ExecuteThreaded(arguments);
		gameIsRunning = true;

		// Let the developer know what's going on
		statusBar()->showMessage("Running '" + Project.project->targetName + "'...");

		// On engine debug console output
		connect(process, &QProcess::readyRead, [=]()
        {
            // Read line from process and write it to console
            QString line = Util::RStrip(process->readAll());
            engineDebugOutput->append(line);

            // Make sure cursor's at the end of the log
            QTextCursor cursor = engineDebugOutput->textCursor();
            cursor.movePosition(QTextCursor::End);
            engineDebugOutput->setTextCursor(cursor);
        });

		// On game finished
		connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=](int)
		{
			gameIsRunning = false;

			// Hide shell dock if it's desired
			if (IdePreferences.autoToggleShellOnRun)
				shellDock->hide();

			// Clear status message
			statusBar()->clearMessage();

			process->deleteLater();
		});
    });
}

void MainWindow::RunToError()
{
    QStringList lines = compileOutput->toPlainText().split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    QString filename = "";
    int lineNumber = -1;

    for(QString line : lines)
    {
        if (!line.startsWith("Error"))
            continue;

		QRegExp filenameExp("\'.*\'");
        filenameExp.indexIn(line);
		filename = filenameExp.capturedTexts().first().replace('\'', "");

        QRegExp lineExp("[0-9]*:$");
        lineExp.indexIn(line);
        QString lineValue = lineExp.capturedTexts().first().replace(':', "");
        lineNumber = lineValue.toInt();
        break;
    }

	if (filename == "" || lineNumber == -1)
		return;

    for(int i = 0; i < scriptsFolder->childCount(); ++i)
    {
        QTreeWidgetItem* item = scriptsFolder->child(i);
        AssetLink* assetLink = reinterpret_cast<AssetLink*>(item->data(0, Qt::UserRole).value<void*>());
        if (assetLink == nullptr || assetLink->GetAssetName() != filename)
            continue;

        ProjectExplorerItemClicked(assetLink->treeItem, 0);
        dynamic_cast<ScriptEditor*>(assetLink->tabItem)->RunToLine(lineNumber);
    }
}

void MainWindow::ClearErrors()
{
	if (scriptsFolder == nullptr)
		return;

    for(int i = 0; i < scriptsFolder->childCount(); ++i)
    {
        QTreeWidgetItem* item = scriptsFolder->child(i);
        AssetLink* assetLink = reinterpret_cast<AssetLink*>(item->data(0, Qt::UserRole).value<void*>());
        if (assetLink == nullptr)
            continue;

        ScriptEditor* scriptEditor = dynamic_cast<ScriptEditor*>(assetLink->tabItem);
        if (scriptEditor == nullptr)
            continue;

        scriptEditor->ClearErrors();
    }
}
