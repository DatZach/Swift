/*
 *  WelcomeDialog.cpp
 *  Welcome Dialog
 */

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <Dialogs/WelcomeDialog.hpp>
#include <Dialogs/NewProjectDialog.hpp>
#include <Project/Project.hpp>
#include <IdePreferences.hpp>
#include <Util.hpp>
#include "ui_welcomedialog.h"

WelcomeDialog::WelcomeDialog(bool canCloseIde, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::WelcomeDialog),
	  canCloseIde(canCloseIde),
      shouldCloseParent(true)
{
    ui->setupUi(this);
    InitializeDefaultDialog();

    SetupUi();
    SetupConnections();
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::SetupUi()
{
    QList<QTreeWidgetItem*> items;

    // TODO This should probably be abstracted behind another class
	// TODO Use QSettings for recent.txt instead

    // Open recent projects file
	QDir recentProjectsPath(QCoreApplication::applicationDirPath());
	QFile recentProjects(recentProjectsPath.absoluteFilePath("recent.txt"));
    if (!recentProjects.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // Populate recent projects list
    while(!recentProjects.atEnd())
    {
        // Read recent project path
        QString projectPath = QFileInfo(recentProjects.readLine()).filePath();

        // Evaluate path and check if it exists
        QDir projectDir(projectPath);
        projectDir.cdUp();
        if (!projectDir.exists())
            continue;

        // Parse out project name
        // TODO Use project's target to determine project name
        QString projectName = projectDir.absolutePath().split('/').last();

        // Create tree item
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeRecentProjects);
            item->setText(0, projectName);
            item->setText(1, QDir::toNativeSeparators(projectDir.absolutePath() + "/"));
        items.push_back(item);
    }

    // Add all items
    ui->treeRecentProjects->insertTopLevelItems(0, items);
}

void WelcomeDialog::SetupConnections()
{
    connect(ui->buttonNewProject, &QPushButton::clicked, this, &WelcomeDialog::CreateNewProject);
    connect(ui->buttonOpenProject, &QPushButton::clicked, this, &WelcomeDialog::OpenProject);
    connect(ui->treeRecentProjects, &QTreeWidget::itemDoubleClicked, this, &WelcomeDialog::OpenRecentProject);
}

void WelcomeDialog::CreateNewProject()
{
    // Show "Create New Project" Dialog
    NewProjectDialog* newProjectDialog = new NewProjectDialog(reinterpret_cast<QWidget*>(parent()));
    newProjectDialog->show();

    // Close this dialog without closing IDE
    shouldCloseParent = false;
    close();
}

void WelcomeDialog::OpenProject()
{
    // Get path of the project to open
    QString path = QFileDialog::getOpenFileName(this, "Open Project",
												QDir::currentPath(), "Swift Project (*.sproj)");
    if (path == "")
        return;

    // Try to open it
    if (!Project.Open(path))
    {
		QMessageBox::critical(this, "Error", "Cannot open the project. It may have been created with an older version of Swift.");
        return;
    }

    // Close this dialog without closing IDE
    shouldCloseParent = false;
    close();
}

void WelcomeDialog::OpenRecentProject(QTreeWidgetItem* item, int)
{
    // Try and load recent project item
    QDir path(item->text(1));
	if (!Project.Open(path.absoluteFilePath("project.sproj")))
    {
		QMessageBox::critical(this, "Error", "Cannot open the project. This project may have been created "
											 "with an older version of Swift.");
        return;
    }

    // Close this dialog without closing IDE
    shouldCloseParent = false;
    close();
}

void WelcomeDialog::closeEvent(QCloseEvent* event)
{
    // Close IDE if we need to
	if (canCloseIde && shouldCloseParent && IdePreferences.welcomeDialogClosesMainWindow)
        reinterpret_cast<QWidget*>(parent())->close();

    // Accept this closing event
    event->accept();
}
