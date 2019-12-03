/*
 *  SoundEditor.cpp
 *  Sound Editor Form
 */

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <Forms/SoundEditor.hpp>
#include "ui_SoundEditor.h"

SoundEditor::SoundEditor(AssetLink* assetLink, QWidget* parent)
    : AssetForm(assetLink, parent),
      ui(new Ui::SoundEditor),
      sound(dynamic_cast<ProjectSounds::Sound*>(GetAssetLink()->asset))
{
    // Reserve asset
    GetAssetLink()->opened = true;
    ui->setupUi(this);

    SetupUi();
    SetupConnections();
}

SoundEditor::~SoundEditor()
{
    delete ui;
}

void SoundEditor::SetupUi()
{
    ui->lineEditName->setText(sound->name);
    ui->lineEditName->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z0-9_]*$")));
    QStringList fileNameSplit = sound->filename.split(".", QString::SkipEmptyParts);
    ui->labelExtension->setText("Extension: " + (fileNameSplit.count() >= 2 ? fileNameSplit.at(1) : ""));

    if (sound->mode.toLower() == "file")
        ui->radioButtonFileStreaming->setChecked(true);
    else if (sound->mode.toLower() == "memory")
        ui->radioButtonMemoryStreaming->setChecked(true);

    ui->comboBoxSampleRate->setCurrentIndex(ui->comboBoxSampleRate->findText(QString::number(sound->sampleRate)));
    ui->comboBoxBitRate->setCurrentIndex(ui->comboBoxBitRate->findText(QString::number(sound->bitRate)));
}

void SoundEditor::SetupConnections()
{
    // Sound Name Line Edit
    connect(ui->lineEditName, &QLineEdit::textChanged, [=]()
    {
        sound->name = ui->lineEditName->text();
        GetAssetLink()->treeItem->setText(0, sound->name);

        SetModified();
    });

    // Load Sound Button
    connect(ui->pushButtonLoadSound, &QPushButton::clicked, [=]()
    {
        QString filePath = QFileDialog::getOpenFileName(this, "Open a sound", QDir::currentPath(),
                                                        "Supported formats (*.ogg *.wav *.xm *.mod *.it *.s3m)");
        if (filePath == "")
            return;

        sound->filename = filePath;
        QStringList fileNameSplit = sound->filename.split(".", QString::SkipEmptyParts);
        ui->labelExtension->setText("Extension: " + (fileNameSplit.count() >= 2 ? fileNameSplit.at(1) : ""));

        SetModified();
    });

    // File Stream Mode Radio Buttons
    connect(ui->radioButtonFileStreaming, &QPushButton::toggled, [=](bool checked)
    {
        sound->mode = checked ? "file" : "memory";
        SetModified();
    });

    // Sample Rate Combobox
    connect(ui->comboBoxSampleRate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    [=](int index)
    {
        sound->sampleRate = ui->comboBoxSampleRate->itemText(index).toInt();
        SetModified();
    });

    // Bit Rate Combobox
    connect(ui->comboBoxBitRate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    [=](int index)
    {
        sound->bitRate = ui->comboBoxBitRate->itemText(index).toInt();
        SetModified();
    });
}

void SoundEditor::Save()
{
    // Save (all) sounds
    Project.sounds->Save();

    // We're no longer dirty
    SetModified(false);
}
