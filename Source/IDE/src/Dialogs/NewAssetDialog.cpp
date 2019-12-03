/*
 *  NewAssetDialog.cpp
 *  New Asset Dialog
 */

#include <QMessageBox>
#include <Dialogs/NewAssetDialog.hpp>
#include <MainWindow.hpp>
#include <Util.hpp>
#include "ui_NewAssetDialog.h"

extern MainWindow* mainWindow;

NewAssetDialog::NewAssetDialog(ProjectAssetType type, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::NewAssetDialog)
{
    ui->setupUi(this);
    InitializeDefaultDialog();

    SetupUi();
    SetupConnections();

    ui->listWidgetAssets->item((int)type)->setSelected(true);
    ui->listWidgetAssets->setCurrentItem(ui->listWidgetAssets->item((int)type));
}

NewAssetDialog::~NewAssetDialog()
{
    delete ui;
}

void NewAssetDialog::SetupUi()
{
    ui->lineEditName->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z0-9_]*$")));
    ui->lineEditName->setFocus();
}

void NewAssetDialog::SetupConnections()
{
    connect(ui->lineEditName, &QLineEdit::textChanged, [=](const QString& value)
    {
        ui->pushButtonAdd->setEnabled(value.length() > 0);
    });

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &NewAssetDialog::CreateAsset);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::close);
}

void NewAssetDialog::CreateAsset()
{
    switch(ui->listWidgetAssets->currentRow())
    {
        case 0:
            if (!CreateTexture())
                return;
            break;

        case 1:
            if (!CreateSound())
                return;
            break;

        case 2:
            QMessageBox::information(this, "Not Supported", "Swift does not support fonts yet.");
            return;

        case 3:
            if (!CreateScene())
                return;
            break;

        case 4:
            if (!CreateScript())
                return;
            break;
    }

    close();
}

bool NewAssetDialog::CreateTexture()
{
    // Make sure we can use this name
    QString name = ui->lineEditName->text();
    for(int i = 0; i < Project.textures->textures.size(); ++i)
    {
        if (name == Project.textures->textures[i].name)
        {
            QMessageBox::critical(this, "Already exists", "A texture with that name already exists!");
            return false;
        }
    }

    // Create asset
    ProjectTextures::Texture texture;
    texture.name = name;
    Project.textures->textures.push_back(texture);

    // Add it to tree
    AssetLink* assetLink = new AssetLink(&Project.textures->textures.back());
    mainWindow->ProjectExplorerAddItem(assetLink, QIcon(":/icons/rainbow.png"));
    mainWindow->texturesFolder->setExpanded(true);
    mainWindow->ProjectExplorerItemClicked(assetLink->treeItem, -1);

    Project.textures->Save();

    return true;
}

bool NewAssetDialog::CreateSound()
{
    // Make sure we can use this name
    QString name = ui->lineEditName->text();
    for(int i = 0; i < Project.sounds->sounds.size(); ++i)
    {
        if (name == Project.sounds->sounds[i].name)
        {
            QMessageBox::critical(this, "Already exists", "A sound with that name already exists!");
            return false;
        }
    }

    // Create asset
    ProjectSounds::Sound sound;
    sound.name = name;
    Project.sounds->sounds.push_back(sound);

    // Add it to tree
    AssetLink* assetLink = new AssetLink(&Project.sounds->sounds.back());
    mainWindow->ProjectExplorerAddItem(assetLink, QIcon(":/icons/sound.png"));
    mainWindow->soundsFolder->setExpanded(true);
    mainWindow->ProjectExplorerItemClicked(assetLink->treeItem, -1);

    Project.sounds->Save();

    return true;
}

bool NewAssetDialog::CreateScene()
{
    // Make sure we can use this name
    QString name = ui->lineEditName->text();
    if (!name.endsWith(".sd"))
        name += ".sd";

    for(int i = 0; i < Project.scenes.size(); ++i)
    {
        if (name == QFileInfo(Project.scenes[i].name).fileName())
        {
            QMessageBox::critical(this, "Already exists", "A scene with that name already exists!");
            return false;
        }
    }

    // Create scene before loading it
    if (!SceneData::SceneData::Create(name))
    {
        QMessageBox::warning(this, "Error", "Cannot create new scene.");
        return false;
    }

    // Create asset
    Project.scenes.push_back(Scene(name));

    // Add it to tree
    AssetLink* assetLink = new AssetLink(&Project.scenes.back());
    mainWindow->ProjectExplorerAddItem(assetLink, QIcon(":/icons/map.png"));
    mainWindow->scenesFolder->setExpanded(true);
    mainWindow->ProjectExplorerItemClicked(assetLink->treeItem, -1);

    return true;
}

bool NewAssetDialog::CreateScript()
{
    // Make sure we can use this name
    QString name = ui->lineEditName->text();
    if (!name.endsWith(".ss"))
        name += ".ss";

    for(int i = 0; i < Project.scripts.size(); ++i)
    {
        if (name == QFileInfo(Project.scripts[i].name).fileName())
        {
            QMessageBox::critical(this, "Already exists", "A script with that name already exists!");
            return false;
        }
    }

    // Create script before loading it
    if (!Script::Create(name))
    {
        QMessageBox::warning(this, "Error", "Cannot create new script \"" + name + "\".");
        return false;
    }

    // Create asset
    Project.scripts.push_back(Script(name));

    // Add it to tree
    AssetLink* assetLink = new AssetLink(&Project.scripts.back());
    mainWindow->ProjectExplorerAddItem(assetLink, QIcon(":/icons/script.png"));
    mainWindow->scriptsFolder->setExpanded(true);
    mainWindow->ProjectExplorerItemClicked(assetLink->treeItem, -1);

    return true;
}
