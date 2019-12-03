/*
 *	IdePreferencesDialog.cpp
 *	IDE Preferences Dialog
 */

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <Dialogs/IdePreferencesDialog.hpp>
#include <IdePreferences.hpp>
#include <Util.hpp>
#include "ui_IdePreferencesDialog.h"

IdePreferencesDialog::IdePreferencesDialog(QWidget* parent)
	: QDialog(parent),
	  ui(new Ui::IdePreferencesDialog)
{
	ui->setupUi(this);
	SetupUi();
	SetupConnections();
	InitializeDefaultDialog();
}

IdePreferencesDialog::~IdePreferencesDialog()
{
	delete ui;
}

void IdePreferencesDialog::SetupUi()
{
	ui->checkBoxShowWelcomeForm->setChecked(IdePreferences.showWelcomeDialog);
	ui->checkBoxClosingWelcomeFormClosesEditor->setChecked(IdePreferences.welcomeDialogClosesMainWindow);
	ui->checkBoxCompilerDisassembly->setChecked(IdePreferences.compileDisassemble);
	ui->checkBoxToggleShellOnRun->setChecked(IdePreferences.autoToggleShellOnRun);
	ui->checkBoxToggleShellOnScene->setChecked(IdePreferences.autoToggleShellOnScene);

	//SendMessage((HWND)ui->pushButtonUpdateFileAssociations->effectiveWinId(), 0x160C, 0, 0xFFFFFFFF);
}

void IdePreferencesDialog::SetupConnections()
{
	connect(ui->pushButtonClearRecentProjects, &QPushButton::clicked, [=]()
	{
		int index = QMessageBox::question(this, "Clear recent projects list?",
										  "Are you sure you want to clear the recent projects list?",
										  QMessageBox::Yes, QMessageBox::No);
		if (index == QMessageBox::Yes)
		{
			// TODO Use QSettings instead
			QDir recentProjectsPath(QCoreApplication::applicationDirPath());
			QFile recentProjectsList(recentProjectsPath.absoluteFilePath("recent.txt"));
			recentProjectsList.open(QFile::WriteOnly | QFile::Truncate);
		}
	});

	connect(ui->pushButtonUpdateFileAssociations, &QPushButton::clicked, [=]()
	{
		/*
		 *	In HKEY_CLASSES_ROOT
		 *		Create .sproj
		 *			DefaultValue = SwiftProjectFile
		 *		Create SwiftProjectFile
		 *			DefaultValue = Swift Project
		 *
		 *			SUBKEY DefaultIcon  = C:\Path\To\IDE.exe,0
		 *			SUBKEY shell
		 *				SUBKEY open
		 *					SUBKEY command = "C:\Path\To\IDE.exe" "%1"
		 */

		QString pathToIde = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
		QString openCommand = "\"" + pathToIde + "\" \"%1\"";

		CreateKeyWithDefaultValue(HKEY_CLASSES_ROOT, ".sproj", "SwiftProjectFile");
		CreateKeyWithDefaultValue(HKEY_CLASSES_ROOT, "SwiftProjectFile", "Swift Project");
		CreateKeyWithDefaultValue(HKEY_CLASSES_ROOT, "SwiftProjectFile\\DefaultIcon", pathToIde);
		CreateKeyWithDefaultValue(HKEY_CLASSES_ROOT, "SwiftProjectFile\\Shell", "");
		CreateKeyWithDefaultValue(HKEY_CLASSES_ROOT, "SwiftProjectFile\\Shell\\Open", "");
		CreateKeyWithDefaultValue(HKEY_CLASSES_ROOT, "SwiftProjectFile\\Shell\\Open\\Command",
								  openCommand);

		QMessageBox::information(this, "Update file associations", "Successfully updated file associations.", QMessageBox::Ok);
	});

	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
	connect(ui->buttonBox, &QDialogButtonBox::accepted, [=]()
	{
		IdePreferences.showWelcomeDialog = ui->checkBoxShowWelcomeForm->checkState() == Qt::Checked;
		IdePreferences.welcomeDialogClosesMainWindow =
				ui->checkBoxClosingWelcomeFormClosesEditor->checkState() == Qt::Checked;
		IdePreferences.compileDisassemble = ui->checkBoxCompilerDisassembly->checkState() == Qt::Checked;
		IdePreferences.autoToggleShellOnRun = ui->checkBoxToggleShellOnRun->checkState() == Qt::Checked;
		IdePreferences.autoToggleShellOnScene = ui->checkBoxToggleShellOnScene->checkState() == Qt::Checked;

		if (!IdePreferences.Save())
		{
			QMessageBox::critical(this, "Oh boy...", "Failed to save IDE preferences.");
			return;
		}

		close();
	});
}

void IdePreferencesDialog::CreateKeyWithDefaultValue(HKEY parent, const QString& path, const QString& value)
{
	HKEY key;

	if (RegOpenKey(parent, path.toStdWString().data(), &key) != ERROR_SUCCESS)
	{
		RegCreateKey(parent, path.toStdWString().data(), &key);

		if (!value.isEmpty())
			RegSetValueEx(key, L"", 0, REG_SZ, (const BYTE*)value.toStdWString().data(), value.size() * 2);
	}

	RegCloseKey(key);
}
