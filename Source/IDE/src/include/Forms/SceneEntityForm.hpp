/*
 *	SceneEntityEditorForm.hpp
 *	Entity Editor - Scene Editor
 */

#ifndef __SCENE_ENTITY_EDITOR_FORM_HPP
#define __SCENE_ENTITY_EDITOR_FORM_HPP

#include <QPainter>
#include <QWidget>
#include <QList>

namespace Ui
{
	class SceneEntityEditorForm;
}

class SceneEditor;
class EntityPreview;

class SceneEntityForm : public QWidget
{
	Q_OBJECT

public:
	class Entity
	{
	private:
		QString name;
		QPixmap texture;

	public:
		Entity(const QString& name, const QString& textureIdent);

		const QString& Name() const;
		const QPixmap& Texture() const;
	};

private:
	Ui::SceneEntityEditorForm* ui;
	EntityPreview* entityPreview;
	QList<Entity> entities;

	// HACK Thanks for being shit, Qt
	SceneEditor* editor;

	void DiscoverEntities();
	void SetupUi();
	void SetupConnections();

public:
	SceneEntityForm(SceneEditor* editor, QWidget* parent = nullptr);
	~SceneEntityForm();

	void hideEvent(QHideEvent*);

	Entity* GetCurrentEntity() const;
};

class EntityPreview : public QWidget
{
	Q_OBJECT

private:
	QPixmap bgGrid;
	SceneEntityForm::Entity* entity;

	void CreateBgGrid();

public:
	explicit EntityPreview(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent*);

	int heightForWidth(int w) const;
	bool hasHeightForWidth() const;

	void SetCurrentEntity(SceneEntityForm::Entity* entity);
	SceneEntityForm::Entity* GetCurrentEntity() const;
};

#endif
