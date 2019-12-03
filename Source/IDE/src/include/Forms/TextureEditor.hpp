/*
 *  TextureEditor.hpp
 *  Texture Editor Form
 */

#ifndef __TEXTURE_EDITOR_HPP
#define __TEXTURE_EDITOR_HPP

#include <QFileSystemWatcher>
#include <QWidget>
#include <QPainter>
#include <Forms/AssetForm.hpp>

namespace Ui
{
    class TextureEditor;
}

class TextureView : public QWidget
{
	Q_OBJECT

private:
	QFileSystemWatcher watcher;
	ProjectTextures::Texture* textureProperties;
	QPixmap bgGrid;
	QImage image;

public:
	explicit TextureView(ProjectTextures::Texture* texture, QWidget* parent = nullptr);

	void SyncTexture();

	void paintEvent(QPaintEvent*);

	QSize GetSize() const;
};

class TextureEditor : public AssetForm
{
    Q_OBJECT

private:
    Ui::TextureEditor* ui;
	TextureView* textureView;
    ProjectTextures::Texture* texture;

    void SetupUi();
    void SetupConnections();

public:
    explicit TextureEditor(AssetLink* assetLink, QWidget* parent = nullptr);
    ~TextureEditor();

    void Save();
};

#endif
