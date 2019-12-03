/*
 *  TextureEditor.cpp
 *  Texture Editor Form
 */

#include <QFileDialog>
#include <QMessageBox>
#include <QPaintEvent>
#include <MainWindow.hpp>
#include <Forms/TextureEditor.hpp>
#include "ui_TextureEditor.h"

TextureView::TextureView(ProjectTextures::Texture* texture, QWidget* parent)
	: QWidget(parent),
	  watcher(),
	  textureProperties(texture),
	  bgGrid(),
	  image()
{
	// Create transparency grid
	QPainter painter;

	bgGrid = QPixmap(96, 96);
	bgGrid.fill(QColor(234, 234, 234));

	painter.begin(&bgGrid);
	QBrush brush(Qt::white, Qt::SolidPattern);
	painter.fillRect(QRect(0, 0, 48, 48), brush);
	painter.fillRect(QRect(48, 48, 48, 48), brush);
	painter.end();

	// Sync the texture initially
	SyncTexture();

	// Keep the editor in sync with the texture on disk
	watcher.addPath(textureProperties->GetPath());
	connect(&watcher, &QFileSystemWatcher::fileChanged, [=](const QString& path)
	{
		SyncTexture();
	});
}

void TextureView::SyncTexture()
{
	QString texturePath = textureProperties->GetPath();
	if (texturePath == "")
		return;

	image = QImage(texturePath);
}

void TextureView::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);
	painter.fillRect(event->rect(), QBrush(QColor(219, 219, 219)));

	// Calculate where to draw everything relative to
	QPoint imagePosition(event->rect().width() / 2, event->rect().height() / 2);
	imagePosition -= QPoint(image.size().width() / 2, image.size().height() / 2);

	// Draw everything else if we have an image loaded
	if (!image.isNull())
	{
		// Background
		QRect bgRect(imagePosition, image.size());
		painter.drawTiledPixmap(bgRect, bgGrid);

		// Texture
		painter.drawImage(imagePosition, image);

		// Origin marker
		painter.setPen(QPen(QColor(255, 32, 32, 208), 2));
		painter.drawLine(imagePosition + QPoint(textureProperties->originX, textureProperties->originY) - QPoint(24, 0),
						 imagePosition + QPoint(textureProperties->originX, textureProperties->originY) + QPoint(24, 0));
		painter.drawLine(imagePosition + QPoint(textureProperties->originX, textureProperties->originY) - QPoint(0, 24),
						 imagePosition + QPoint(textureProperties->originX, textureProperties->originY) + QPoint(0, 24));
	}

	painter.end();
}

QSize TextureView::GetSize() const
{
	return image.isNull() ? QSize(0, 0) : image.size();
}

TextureEditor::TextureEditor(AssetLink* assetLink, QWidget* parent)
    : AssetForm(assetLink, parent),
      ui(new Ui::TextureEditor),
      texture(dynamic_cast<ProjectTextures::Texture*>(GetAssetLink()->asset))
{
    GetAssetLink()->opened = true;
    ui->setupUi(this);
    SetupUi();
    SetupConnections();
}

TextureEditor::~TextureEditor()
{
    delete textureView;
    delete ui;
}

void TextureEditor::SetupUi()
{
	textureView = new TextureView(texture, ui->frameTarget);
	ui->gridLayoutTarget->addWidget(textureView);

	ui->labelWidth->setText("Width: " + QString::number(textureView->GetSize().width()));
	ui->labelHeight->setText("Height: " + QString::number(textureView->GetSize().height()));
    ui->lineEditName->setText(texture->name);
    ui->lineEditName->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z0-9_]*$")));
    ui->checkBoxAnimated->setChecked(texture->animated);
    ui->checkBoxSmooth->setChecked(texture->smooth);
    ui->checkBoxLazy->setChecked(texture->lazy);
    ui->spinBoxOriginX->setValue(texture->originX);
    ui->spinBoxOriginY->setValue(texture->originY);
	ui->spinBoxOriginX->setMaximum(textureView->GetSize().width());
	ui->spinBoxOriginY->setMaximum(textureView->GetSize().height());
}

void TextureEditor::SetupConnections()
{
    // Texture Name Line Edit
    connect(ui->lineEditName, &QLineEdit::textChanged, [=]()
    {
        texture->name = ui->lineEditName->text();
        GetAssetLink()->treeItem->setText(0, texture->name);
        SetModified();
    });

    // Load Texture Button
    connect(ui->buttonLoadTexture, &QPushButton::clicked, [=]()
    {
        // Get new file path
        QString filePath = QFileDialog::getOpenFileName(this, "Open an image", QDir::currentPath(),
														"Supported formats (*.png *.bmp *.jpg *.jpeg *.gif)");
        if (filePath == "")
            return;

        // Delete old texture if it exists so we can overwrite it later
        if (QFile::exists(texture->GetPath()))
            QFile::remove(texture->GetPath());

        // Update texture filename and sync texture in view
        texture->filename = filePath;
		QFile::copy(filePath, texture->GetPath());
        textureView->SyncTexture();
		ui->labelWidth->setText("Width: " + QString::number(textureView->GetSize().width()));
		ui->labelHeight->setText("Height: " + QString::number(textureView->GetSize().height()));
		ui->spinBoxOriginX->setMaximum(textureView->GetSize().width());
		ui->spinBoxOriginY->setMaximum(textureView->GetSize().height());

        // We're dirty now
        SetModified();
    });

    // Animated Checkbox
    connect(ui->checkBoxAnimated, &QCheckBox::toggled, [=](bool value)
    {
        texture->animated = value;
        SetModified();
    });

    // Smooth Checkbox
    connect(ui->checkBoxSmooth, &QCheckBox::toggled, [=](bool value)
    {
        texture->smooth = value;
        SetModified();
    });

    // Lazy Checkbox
    connect(ui->checkBoxLazy, &QCheckBox::toggled, [=](bool value)
    {
        texture->lazy = value;
        SetModified();
    });

    // Origin Spinboxes
    connect(ui->spinBoxOriginX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
    [=](int value)
    {
        texture->originX = value;
        SetModified();
    });

    connect(ui->spinBoxOriginY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
    [=](int value)
    {
        texture->originY = value;
        SetModified();
    });

    // Origin buttons
    connect(ui->buttonOriginTopLeft, &QPushButton::clicked, [=]()
    {
        texture->originX = 0;
        texture->originY = 0;
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginTopCenter, &QPushButton::clicked, [=]()
    {
		texture->originX = textureView->GetSize().width() / 2;
        texture->originY = 0;
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginTopRight, &QPushButton::clicked, [=]()
    {
		texture->originX = textureView->GetSize().width();
        texture->originY = 0;
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginLeftCenter, &QPushButton::clicked, [=]()
    {
        texture->originX = 0;
		texture->originY = textureView->GetSize().height() / 2;
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginCenter, &QPushButton::clicked, [=]()
    {
		texture->originX = textureView->GetSize().width() / 2;
		texture->originY = textureView->GetSize().height() / 2;
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginRightCenter, &QPushButton::clicked, [=]()
    {
		texture->originX = textureView->GetSize().width();
		texture->originY = textureView->GetSize().height() / 2;
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginBottomLeft, &QPushButton::clicked, [=]()
    {
        texture->originX = 0;
		texture->originY = textureView->GetSize().height();
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginBottomCenter, &QPushButton::clicked, [=]()
    {
		texture->originX = textureView->GetSize().width() / 2;
		texture->originY = textureView->GetSize().height();
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });

    connect(ui->buttonOriginBottomRight, &QPushButton::clicked, [=]()
    {
		texture->originX = textureView->GetSize().width();
		texture->originY = textureView->GetSize().height();
		ui->spinBoxOriginX->setValue(texture->originX);
		ui->spinBoxOriginY->setValue(texture->originY);
		textureView->update();
		SetModified();
    });
}

void TextureEditor::Save()
{
    // Save (all) textures
    Project.textures->Save();
    SetModified(false);
}
