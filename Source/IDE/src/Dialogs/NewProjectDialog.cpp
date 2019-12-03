/*
 *  NewProjectDialog.hpp
 *  New Project Dialog
 */

#include <QFileDialog>
#include <QMessageBox>
#include <Dialogs/NewProjectDialog.hpp>
#include <Project/Project.hpp>
#include <Util.hpp>
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    InitializeDefaultDialog();

    SetupUi();
    SetupConnections();
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::SetupUi()
{
    ui->lineEditName->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z0-9_]*$")));
    ui->lineEditLocation->setText(GetDesktopPath());
    ui->lineEditName->setFocus();
}

void NewProjectDialog::SetupConnections()
{
    connect(ui->buttonBrowse, &QPushButton::clicked, this, &NewProjectDialog::BrowseLocation);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NewProjectDialog::Accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
}

void NewProjectDialog::BrowseLocation()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory", ui->lineEditLocation->text(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir != "")
        ui->lineEditLocation->setText(dir);
}

void NewProjectDialog::Accept()
{
    // Don't attempt to create a project is the directory or name is not specified
    if (ui->lineEditLocation->text() == "" ||
            ui->lineEditName->text() == "")
    {
        QMessageBox::critical(this, "Error",
                              "You need to enter a project name and select a destination folder "
                              "for the project to be created in.");
        return;
    }

    // Create the project
    QString result = Project.New(ui->lineEditName->text(), ui->lineEditLocation->text());
    if (result != "")
    {
        QMessageBox::critical(this, "Error", result);
        return;
    }

    // Successfully created the project, close this dialog
    close();
}

QString NewProjectDialog::GetDesktopPath() const
{
    // Find desktop path
    QDir desktopPath(QDir::homePath());
    if (!desktopPath.cd("Desktop"))
        return QDir::toNativeSeparators(QDir::homePath() + "/");

    // Return the absolute path to it
    return QDir::toNativeSeparators(desktopPath.absolutePath() + "/");
}
