/*
 *  ConfigurationDialog.cpp
 *  Configuration Dialog
 */

#include <Dialogs/ConfigurationDialog.hpp>
#include <Project/Lexer/TokenStream.hpp>
#include <Util.hpp>
#include "ui_configurationdialog.h"

ConfigurationDialog::ConfigurationDialog(ProjectConfiguration* configuration, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::ConfigurationDialog),
      configuration(configuration)
{
    ui->setupUi(this);

    InitializeDefaultDialog();
    SetupUi();
    SetupConnections();
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}

void ConfigurationDialog::SetupUi()
{
	ParseStates();

    // Game
    ui->lineEditTitle->setText(configuration->title);
	ui->comboBoxInitialState->setCurrentIndex(
				ui->comboBoxInitialState->findText(configuration->initialState));
	if (ui->comboBoxInitialState->currentIndex() == -1)
		ui->comboBoxInitialState->setCurrentIndex(0);

    // Video
    ui->spinBoxVideoWindowWidth->setValue(configuration->windowWidth);
    ui->spinBoxVideoWindowHeight->setValue(configuration->windowHeight);
    ui->checkBoxVideoFullscreen->setChecked(configuration->fullscreen);
    ui->checkBoxVideoVsync->setChecked(configuration->vsync);
    ui->checkBoxVideoCursor->setChecked(configuration->cursor);
    ui->spinBoxVideoFramerate->setValue(configuration->targetFramerate);
    ui->comboBoxVideoAaLevel->setCurrentIndex(
                ui->comboBoxVideoAaLevel->findText(QString::number(configuration->antiAliasingLevel)));
    ui->comboBoxTextureSheetSize->setCurrentIndex(
                ui->comboBoxTextureSheetSize->findText(QString::number(configuration->sheetSize)));

    // Version Information
    ui->lineEditVersion->setText(configuration->version);
    ui->checkBoxIncrementVersion->setChecked(configuration->incrementBuild);
    ui->lineEditDescription->setText(configuration->description);
    ui->lineEditProduct->setText(configuration->product);
    ui->lineEditCompany->setText(configuration->company);
    ui->lineEditCopyright->setText(configuration->copyright);
    ui->lineEditTrademark->setText(configuration->trademark);
}

void ConfigurationDialog::SetupConnections()
{
    // OK Button
    connect(ui->pushButtonOk, &QPushButton::clicked, [=]()
    {
        Project.configuration->Save();
        close();
    });

    // Game tab
    connect(ui->lineEditTitle, &QLineEdit::textChanged,
            [=](const QString& value) { configuration->title = value; });
	connect(ui->comboBoxInitialState, &QComboBox::currentTextChanged, [=](const QString& value)
	{
		if (value == "<None>")
			Project.configuration->initialState = "";
		else
			Project.configuration->initialState = value;
	});

    // Video tab
    connect(ui->spinBoxVideoWindowWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value) { configuration->windowWidth = value; });
    connect(ui->spinBoxVideoWindowHeight, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value) { configuration->windowHeight = value; });
    connect(ui->checkBoxVideoFullscreen, &QCheckBox::toggled,
            [=](bool value) { configuration->fullscreen = value; });
    connect(ui->checkBoxVideoVsync, &QCheckBox::toggled,
            [=](bool value) { configuration->vsync = value; });
    connect(ui->checkBoxVideoCursor, &QCheckBox::toggled,
            [=](bool value) { configuration->cursor = value; });
    connect(ui->spinBoxVideoFramerate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value) { configuration->targetFramerate = value; });
    connect(ui->comboBoxVideoAaLevel, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) { configuration->antiAliasingLevel = ui->comboBoxVideoAaLevel->itemText(index).toInt(); });
    connect(ui->comboBoxTextureSheetSize, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) { configuration->sheetSize = ui->comboBoxTextureSheetSize->itemText(index).toInt(); });

    // Version Information tab
    connect(ui->lineEditVersion, &QLineEdit::textChanged,
            [=](const QString& value) { configuration->version = value; });
    connect(ui->checkBoxIncrementVersion, &QCheckBox::toggled,
            [=](bool value) { configuration->incrementBuild = value; });
    connect(ui->lineEditDescription, &QLineEdit::textChanged,
            [=](const QString& value) { configuration->description = value; });
    connect(ui->lineEditProduct, &QLineEdit::textChanged,
            [=](const QString& value) { configuration->product = value; });
    connect(ui->lineEditCompany, &QLineEdit::textChanged,
            [=](const QString& value) { configuration->company = value; });
    connect(ui->lineEditCopyright, &QLineEdit::textChanged,
            [=](const QString& value) { configuration->copyright = value; });
    connect(ui->lineEditTrademark, &QLineEdit::textChanged,
            [=](const QString& value) { configuration->trademark = value; });
}

void ConfigurationDialog::ParseStates()
{
	for(Script& script : Project.scripts)
	{
		bool ok;
		Lexer::TokenStream tokenStream(script.GetPath(), ok);
		if (!ok)
			continue;

		if (!tokenStream.Accept("state"))
			continue;

		ui->comboBoxInitialState->addItem(tokenStream.ReadWord());
	}
}
