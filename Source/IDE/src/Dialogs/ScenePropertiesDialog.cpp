/*
 *  ScenePropertiesDialog.cpp
 *  Scene Properties Dialog
 */

#include <QColorDialog>
#include <Dialogs/ScenePropertiesDialog.hpp>
#include <Project/Project.hpp>
#include <Util.hpp>
#include "ui_scenepropertiesdialog.h"

ScenePropertiesDialog::ScenePropertiesDialog(SceneData::SceneData* sceneData, int* gridSize, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::ScenePropertiesDialog),
      bgColor(),
      gridSize(gridSize),
      sceneData(sceneData)
{
    ui->setupUi(this);
    InitializeDefaultDialog();

    SetupUi();
    SetupConnections();
}

ScenePropertiesDialog::~ScenePropertiesDialog()
{
    delete ui;
}

void ScenePropertiesDialog::SetupUi()
{
    // Scene Properties
	ui->spinBoxSceneWidth->setValue(sceneData->configuration.size.width());
	ui->spinBoxSceneHeight->setValue(sceneData->configuration.size.height());
    ui->spinBoxGridSize->setValue(*gridSize);
    ui->lineEditMusic->setText(sceneData->configuration.defaultBgMusic);

    // Background
    ui->lineEditBackgroundTexture->setText(sceneData->background.textureName);
	bgColor = sceneData->background.color;
    ui->pushButtonBackgroundColor->setAutoFillBackground(true);
    ui->pushButtonBackgroundColor->setStyleSheet(
                "background-color: rgb(" +
				QString::number(bgColor.red()) + ", " +
                QString::number(bgColor.green()) + ", " +
				QString::number(bgColor.blue()) + "); color: rgb(255, 255, 255)");
}

void ScenePropertiesDialog::SetupConnections()
{
    // Set As Window Size button
    connect(ui->pushButtonSetAsWindowSize, &QPushButton::clicked, [=]()
    {
        Project.configuration->windowWidth = ui->spinBoxSceneWidth->value();
        Project.configuration->windowHeight = ui->spinBoxSceneHeight->value();
    });

    // Background Color button
    connect(ui->pushButtonBackgroundColor, &QPushButton::clicked, [=]()
    {
        bgColor = QColorDialog::getColor(bgColor, this);
        ui->pushButtonBackgroundColor->setStyleSheet(
                    "background-color: rgb(" +
                    QString::number(bgColor.red()) + ", " +
                    QString::number(bgColor.green()) + ", " +
                    QString::number(bgColor.blue()) + "); color: rgb(255, 255, 255)");
    });

    // Accept button
    connect(ui->buttonBox, &QDialogButtonBox::accepted, [=]()
    {
		sceneData->configuration.size.setWidth(ui->spinBoxSceneWidth->value());
		sceneData->configuration.size.setHeight(ui->spinBoxSceneHeight->value());
        *gridSize = ui->spinBoxGridSize->value();
        sceneData->configuration.defaultBgMusic = ui->lineEditMusic->text();

        sceneData->background.textureName = ui->lineEditBackgroundTexture->text();
		sceneData->background.color = bgColor;

		accept();
    });

    // Cancel button
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
