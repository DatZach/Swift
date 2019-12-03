/*
 *  SceneEditor.cpp
 *  Scene Editor Form
 */

#ifndef __SCENE_EDITOR_HPP
#define __SCENE_EDITOR_HPP

#include <QMap>
#include <QGridLayout>
#include <QFrame>
#include <QPainter>
#include <Forms/SceneEntityForm.hpp>
#include <Forms/AssetForm.hpp>

class SceneEditor;

class SceneView : public QWidget
{
	Q_OBJECT

public:
	enum class Mode
	{
		View,
		Entity,
		Tile
	};

private:
	SceneEditor* sceneEditor;
	SceneData::SceneData* sceneData;

public:
	Mode mode;

private:
	QPixmap bgGrid;

	QMap<QString, QPixmap> textureCache;
	QMap<QString, QString> entityCache;

	QPoint mouseDelta;
	bool dragging;

	bool showGrid;

	int viewX, viewY;
	float zoomFactor;
	bool keyAltHeld;

	// HACK I don't know a better way to do this
	bool ignoreNextResize;

public:
	int gridSize;

private:
	SceneEditor* Editor() const;

	void CreateBgGrid();
	void CenterCamera();

	const QPixmap& GetTexture(const QString& name);
	const QPixmap& GetEntityTexture(const QString& name);

public:
	explicit SceneView(SceneData::SceneData* sceneData, SceneEditor* sceneEditor, QWidget* parent = nullptr);

	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
	void wheelEvent(QWheelEvent*);
	void resizeEvent(QResizeEvent*);

	void IgnoreNextResize();
};

class SceneEditor : public AssetForm
{
    Q_OBJECT

private:
	friend class SceneView;
	friend class SceneEntityForm;

private:
	QDockWidget* entityEditorWindow;
	SceneEntityForm* sceneEntityForm;
	SceneView* sceneView;
    QFrame* frameTarget;
    QGridLayout* gridLayoutTarget;
	QPixmap bgGrid;

	QMenu* sceneMenu;
	QLabel* labelCurrentMode;
	QLabel* labelGridSize;
	QLabel* labelZoomFactor;
	QLabel* labelSceneSize;
	QLabel* labelCursorPosition;
	QLabel* labelEntityCount;
	QLabel* labelTileCount;

	SceneData::SceneData sceneData;

	void SetupUi();

	SceneEntityForm* EntityEditor() const;

public:
    Scene* scene;

public:
	explicit SceneEditor(AssetLink* assetLink, QWidget* parent = nullptr);
    ~SceneEditor();

    void Save();

	void TabFocusIn();
	void TabFocusOut();

	void Refresh();
};

#endif // SCENEEDITOR_H
