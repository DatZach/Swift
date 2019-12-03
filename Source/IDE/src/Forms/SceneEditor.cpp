/*
 *	SceneEditor.cpp
 *	Scene Editor
 */

#include <QDebug>
#include <QStatusBar>
#include <Dialogs/ScenePropertiesDialog.hpp>
#include <Forms/SceneEditor.hpp>
#include <Project/Lexer/TokenStream.hpp>
#include <Project/Project.hpp>
#include <IdePreferences.hpp>
#include <MainWindow.hpp>
#include <Util.hpp>

// ==== Scene View ===

extern MainWindow* mainWindow;

SceneView::SceneView(SceneData::SceneData* sceneData, SceneEditor* sceneEditor, QWidget* parent)
	: QWidget(parent),
	  sceneEditor(sceneEditor),
	  sceneData(sceneData),
	  mode(Mode::View),
	  bgGrid(),
	  textureCache(),
	  entityCache(),
	  mouseDelta(),
	  dragging(false),
	  showGrid(true),
	  gridSize(24),
	  viewX(0),
	  viewY(0),
	  zoomFactor(1.0f),
	  keyAltHeld(false),
	  ignoreNextResize(false)
{
	// Make sure we capture keyboard correctly
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);

	// Create transparency grid
	CreateBgGrid();

	// Center camera initially
	CenterCamera();

	// Setup UI
	QShortcut* toggleGridShortcut = new QShortcut(QKeySequence("Ctrl+G"), this);
	connect(toggleGridShortcut, &QShortcut::activated, [=]()
	{
		showGrid = !showGrid;
		repaint();
	});

	// Build entity cache
	for(Script& script : Project.scripts)
	{
		bool ok;
		Lexer::TokenStream stream(script.GetPath(), ok);
		if (!ok)
			continue;

		// Only process entities
		if (!stream.Accept("entity"))
			continue;

		QString name = stream.ReadWord();
		QString textureIdent = "";
		if (stream.Accept(Lexer::TokenType::Delimiter, "{"))
		{
			while(!stream.Accept(Lexer::TokenType::Delimiter, "}"))
			{
				if (stream.ReadWord() != "Sprite")
					continue;

				stream.Read(); // =
				stream.Read(); // Sprite
				stream.Read(); // (
				if (stream.Accept("Textures"))
				{
					stream.Read(); // .
					textureIdent = stream.ReadWord();
				}
				else if (stream.Pass(Lexer::TokenType::String))
					textureIdent = stream.ReadVariant().toString();
			}
		}

		// Add entry to cache
		entityCache[name] = textureIdent;
	}
}

SceneEditor* SceneView::Editor() const
{
	return qobject_cast<SceneEditor*>(parent());
}

void SceneView::CreateBgGrid()
{
	QPainter painter;

	bgGrid = QPixmap(96, 96);
	bgGrid.fill(QColor(234, 234, 234));

	painter.begin(&bgGrid);
	QBrush brush(Qt::white, Qt::SolidPattern);
	painter.fillRect(QRect(0, 0, 48, 48), brush);
	painter.fillRect(QRect(48, 48, 48, 48), brush);
	painter.end();
}

void SceneView::CenterCamera()
{
	QSize canvasSize = size();

	viewX = canvasSize.width() / 2 - sceneData->configuration.size.width() / 2;
	viewY = canvasSize.height() / 2 - sceneData->configuration.size.height() / 2;
}

void SceneView::paintEvent(QPaintEvent* event)
{
	// Center camera on initial resize
	static bool initialResize = true;

	if (initialResize)
	{
		CenterCamera();
		initialResize = false;
	}

	// Grab cursor's position relative to canvas
	QPoint cursorPosition = QCursor::pos() - parentWidget()->mapToGlobal(geometry().topLeft()) -
							QPoint(viewX, viewY);
	QPoint targetCursorPosition = cursorPosition;

	if (!keyAltHeld)
	{
		targetCursorPosition.setX(Util::SnapValue(targetCursorPosition.x(), gridSize));
		targetCursorPosition.setY(Util::SnapValue(targetCursorPosition.y(), gridSize));
	}

	// Update status bar when we redraw, should work just fine
	unsigned int tileCount = 0;
	for(auto& tileMap : sceneData->tiles.maps)
		tileCount += tileMap.tiles.size();

	switch(mode)
	{
		case Mode::View:
			Editor()->labelCurrentMode->setText("View Mode");
			break;

		case Mode::Entity:
			Editor()->labelCurrentMode->setText("Entity Mode");
			break;

		case Mode::Tile:
			Editor()->labelCurrentMode->setText("Tile Mode");
			break;
	}

	Editor()->labelGridSize->setText(QString::number(gridSize) + " x " + QString::number(gridSize) + " grid");
	Editor()->labelZoomFactor->setText(QString::number(zoomFactor, '.', 1) + "x zoom");
	Editor()->labelSceneSize->setText("<img src=\":/icons/scene_size.png\"> " +
									  QString::number(sceneData->configuration.size.width()) + " x " +
									  QString::number(sceneData->configuration.size.height()));
	Editor()->labelCursorPosition->setText("<img src=\":/icons/cursor.png\"> " +
										   QString::number(targetCursorPosition.x()) + ", " +
										   QString::number(targetCursorPosition.y()));
	Editor()->labelEntityCount->setText(QString::number(sceneData->entities.size()) + " Entities");
	Editor()->labelTileCount->setText(QString::number(tileCount) + " Tiles");

	QPainter painter;
	painter.begin(this);

	painter.setViewport(viewX, viewY,
						event->rect().width() * zoomFactor,
						event->rect().height() * zoomFactor);

	double wiggleFactor = ((double)painter.viewport().width() / (double)event->rect().width() +
						   (double)painter.viewport().height() / (double)event->rect().height()) / 2.0;

	// Prefetch some data for cleaner code
	const QSize& sceneSize = sceneData->configuration.size;

	// Transparency background gets drawn first
	painter.drawTiledPixmap(QRectF(-viewX / zoomFactor, -viewY / zoomFactor,
								   event->rect().width() / zoomFactor, event->rect().height() / zoomFactor),
							bgGrid.scaled(bgGrid.size().width() / zoomFactor, bgGrid.size().height() / zoomFactor,
										  Qt::KeepAspectRatioByExpanding));

	// Draw scene background
	// Draw background color
	painter.setBrush(QBrush(sceneData->background.color));
	painter.setPen(Qt::NoPen);
	QRect sceneBgRect(QPoint(0, 0), sceneSize);
	painter.drawRect(sceneBgRect);

	// Draw background texture
	if (!sceneData->background.textureName.isEmpty())
	{
		const QPixmap& bgTexture = GetTexture(sceneData->background.textureName);
		painter.drawPixmap(QRect(QPoint(0, 0), sceneSize), bgTexture);
	}

	// Draw grid
	if (showGrid && zoomFactor > 0.5f)
	{
		unsigned int yCount = sceneSize.height() / gridSize +
							  (sceneSize.height() % gridSize > 0 ? 1 : 0);
		unsigned int xCount = sceneSize.width() / gridSize +
							  (sceneSize.width() % gridSize > 0 ? 1 : 0);

		painter.setPen(QPen(QBrush(QColor(0, 0, 0, 160)), 1.0 / wiggleFactor));

		while(--yCount)
			painter.drawLine(QPoint(0, yCount * gridSize), QPoint(sceneSize.width(), yCount * gridSize));

		while(--xCount)
			painter.drawLine(QPoint(xCount * gridSize, 0), QPoint(xCount * gridSize, sceneSize.height()));
	}

	// Draw tiles
	for(SceneData::Tiles::TileMap& tile : sceneData->tiles.maps)
	{

	}

	// Draw entities
	for(SceneData::Entity& entity : sceneData->entities.entities)
	{
		// TODO Should draw at correct texture origin
		const QPixmap& entityTexture = GetEntityTexture(entity.name);
		painter.drawPixmap(QPoint(entity.x, entity.y), entityTexture);
	}

	// Draw scene bounds outline
	painter.setBrush(QBrush(QColor(0, 0, 0), Qt::NoBrush));
	painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 1.0 / wiggleFactor));
	painter.drawRect(sceneBgRect);

	// Draw mode specific items
	switch(mode)
	{
		case Mode::Entity:
		{
			// NOTE This should never happen, but better safe than sorry
			if (Editor()->EntityEditor() == nullptr)
				break;

			// Get entity, it might be null if none is selected
			auto* entity = Editor()->EntityEditor()->GetCurrentEntity();
			if (entity == nullptr)
				break;

			// Draw ghost
			const QPixmap& entityTexture = GetEntityTexture(entity->Name());
			painter.setOpacity(0.66);
			painter.drawPixmap(targetCursorPosition, entityTexture);
			painter.setOpacity(1.0);
			break;
		}
	}

	painter.end();

	event->accept();
}

void SceneView::mousePressEvent(QMouseEvent* event)
{
	switch(event->button())
	{
		case Qt::LeftButton:
		{
			// NOTE This should never happen, but better safe than sorry
			if (Editor()->EntityEditor() == nullptr)
				break;

			// Get entity, it might be null if none is selected
			auto* currentEntity = Editor()->EntityEditor()->GetCurrentEntity();
			if (currentEntity == nullptr)
				break;

			SceneData::Entity entity;
			entity.name = currentEntity->Name();
			entity.x = event->pos().x() - viewX;
			entity.y = event->pos().y() - viewY;

			// Snap to grid if we're not holding alt
			if (!keyAltHeld)
			{
				entity.x = Util::SnapValue(entity.x, gridSize);
				entity.y = Util::SnapValue(entity.y, gridSize);
			}

			sceneData->entities.entities.push_back(entity);
			sceneEditor->SetModified();
			break;
		}

		case Qt::RightButton:
		{
			size_t i = sceneData->entities.size();
			while(i--)
			{
				SceneData::Entity& entity = sceneData->entities[i];

				const QPixmap& entityTexture = GetEntityTexture(entity.name);
				QRect rect(entity.x, entity.y, entityTexture.size().width(), entityTexture.size().height());

				if (rect.contains(event->pos().x() - viewX, event->pos().y() - viewY))
				{
					sceneData->entities.entities.removeAt(i);
					break;
				}
			}

			break;
		}

		case Qt::MiddleButton:
			mouseDelta = event->pos();
			dragging = true;
			break;
	}

	repaint();
	event->accept();
}

void SceneView::mouseReleaseEvent(QMouseEvent* event)
{
	switch(event->button())
	{
		case Qt::MiddleButton:
			dragging = false;
			break;
	}

	event->accept();
}

void SceneView::mouseMoveEvent(QMouseEvent* event)
{
	if (dragging)
	{
		viewX += event->pos().x() - mouseDelta.x();
		viewY += event->pos().y() - mouseDelta.y();

		mouseDelta = event->pos();
	}

	// HACK We shouldn't /always/ be repainting here
	repaint();
	event->accept();
}

void SceneView::wheelEvent(QWheelEvent* event)
{
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;

	if (!numPixels.isNull())
		zoomFactor += 0.1f * numPixels.y();
	else if (!numDegrees.isNull())
	{
		QPoint numSteps = numDegrees / 15;
		zoomFactor += 0.1f * numSteps.y();
	}

	if (zoomFactor > 3.0f)
		zoomFactor = 3.0f;
	else if (zoomFactor < 0.25f)
		zoomFactor = 0.25f;

	repaint();
	event->accept();
}

void SceneView::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Alt)
		keyAltHeld = true;

	event->accept();
}

void SceneView::keyReleaseEvent(QKeyEvent* event)
{
	switch(event->key())
	{
		case Qt::Key_Alt:
			keyAltHeld = false;
			break;
	}

	event->accept();
}

void SceneView::resizeEvent(QResizeEvent* event)
{
	// Ignore this resize if it was requested
	if (ignoreNextResize)
		ignoreNextResize = false;
	else
	{
		// Preserve view center
		int offsetX = viewX - event->oldSize().width() / 2;
		int offsetY = viewY - event->oldSize().height() / 2;

		viewX = offsetX + event->size().width() / 2;
		viewY = offsetY + event->size().height() / 2;
	}

	event->accept();
}

const QPixmap& SceneView::GetTexture(const QString& name)
{
	// If the cache already contains this texture return it
	if (textureCache.contains(name))
		return textureCache[name];

	// Otherwise we need to load it
	ProjectTextures::Texture properties;
	QPixmap texture;

	// Look through every texture to find this texture
	// TODO Should just add an operator [] overload into Project.textures
	for(ProjectTextures::Texture& value : Project.textures->textures)
	{
		if (value.name != name)
			continue;

		properties = value;
		break;
	}

	// Load texture

	// TODO Don't resolve paths like this
	QDir texturePath(Project.projectRootDirectory);
	texturePath.cd("Textures");
	QString filePath = texturePath.filePath(properties.filename);

	// NOTE This should be fine, since we can't load it anyway we'll just add
	//		an empty pixmap to the cache and "use" that instead.
	if (!texture.load(filePath))
		texture.load(":/icons/brick.png");

	// Add it to the cache and return the value
	textureCache[name] = texture;

	return textureCache[name];
}

const QPixmap& SceneView::GetEntityTexture(const QString& name)
{
	// If this entity's texture is already cached return that
	if (entityCache.contains(name))
		return GetTexture(entityCache[name]);

	// Technically we shouldn't get this far anyways so this should be fine
	entityCache[name] = "";

	return GetTexture(entityCache[name]);
}

void SceneView::IgnoreNextResize()
{
	ignoreNextResize = true;
}

// ==== Scene Editor ====

SceneEditor::SceneEditor(AssetLink* assetLink, QWidget* parent)
	: AssetForm(assetLink, parent),
	  entityEditorWindow(nullptr),
	  sceneEntityForm(nullptr),
	  sceneView(nullptr),
	  frameTarget(nullptr),
	  gridLayoutTarget(nullptr),
	  sceneMenu(nullptr),
	  labelCurrentMode(nullptr),
	  labelGridSize(nullptr),
	  labelZoomFactor(nullptr),
	  labelSceneSize(nullptr),
	  labelCursorPosition(nullptr),
	  labelEntityCount(nullptr),
	  labelTileCount(nullptr),
	  scene(dynamic_cast<Scene*>(GetAssetLink()->asset))
{
	// NOTE This must be here because the initializer list is retarded
	sceneData = scene->sceneData;

	// Reserve asset
	GetAssetLink()->opened = true;

	// Setup UI
	SetupUi();
}

SceneEditor::~SceneEditor()
{

}

void SceneEditor::SetupUi()
{
	sceneEntityForm = new SceneEntityForm(this, this);

	frameTarget = new QFrame(this);
		frameTarget->setFrameShape(QFrame::StyledPanel);
		frameTarget->setFrameShadow(QFrame::Raised);
	gridLayoutTarget = new QGridLayout(frameTarget);
		gridLayoutTarget->setSpacing(0);
		gridLayoutTarget->setContentsMargins(0, 0, 0, 0);

	setLayout(gridLayoutTarget);

	sceneView = new SceneView(&sceneData, this, frameTarget);
	gridLayoutTarget->addWidget(sceneView);

	// Add status bar
	labelCurrentMode = new QLabel("View Mode", mainWindow->statusBar());
	labelGridSize = new QLabel(mainWindow->statusBar());
	labelZoomFactor = new QLabel(mainWindow->statusBar());
	labelSceneSize = new QLabel(mainWindow->statusBar());
	labelSceneSize->setTextFormat(Qt::RichText);
	labelCursorPosition = new QLabel(mainWindow->statusBar());
	labelCursorPosition->setTextFormat(Qt::RichText);
	labelEntityCount = new QLabel(mainWindow->statusBar());
	labelTileCount = new QLabel(mainWindow->statusBar());

	mainWindow->statusBar()->addWidget(labelCurrentMode, 73);
	mainWindow->statusBar()->addWidget(labelGridSize, 5);
	mainWindow->statusBar()->addWidget(labelZoomFactor, 5);
	mainWindow->statusBar()->addWidget(labelSceneSize, 2);
	mainWindow->statusBar()->addWidget(labelCursorPosition, 10);
	mainWindow->statusBar()->addWidget(labelEntityCount, 2);
	mainWindow->statusBar()->addWidget(labelTileCount, 3);
}

void SceneEditor::Save()
{
	// Save scene
	scene->sceneData = sceneData;
	scene->sceneData.Save(scene->sceneData.filename);

	// We're no longer dirty
	SetModified(false);
}

void SceneEditor::TabFocusIn()
{
	// Hide shell window if needed
	if (IdePreferences.autoToggleShellOnScene)
	{
		sceneView->IgnoreNextResize();
		mainWindow->shellDock->hide();
	}

	/*
	 *	Scene
	 *		Entity Window
	 *		Tile Window
	 *		---------------
	 *		Properties
	 */

	sceneMenu = new QMenu("Scene");

	// Entities
	QAction* entityWindowAction = sceneMenu->addAction(QIcon(":/icons/brick.png"), "Edit Entities");
	entityWindowAction->setShortcut(QKeySequence("Ctrl+W,Ctrl+E"));
	connect(entityWindowAction, &QAction::triggered, [=]()
	{
		entityEditorWindow = new QDockWidget("Entities", this);
			entityEditorWindow->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
			entityEditorWindow->setMinimumSize(240, 480);
			entityEditorWindow->setWidget(sceneEntityForm);
		mainWindow->addDockWidget(Qt::LeftDockWidgetArea, entityEditorWindow);

		sceneView->mode = SceneView::Mode::Entity;
	});

	// Tiles
	QAction* tileWindowAction = sceneMenu->addAction(QIcon(":/icons/map.png"), "Edit Tiles");
	tileWindowAction->setShortcut(QKeySequence("Ctrl+W,Ctrl+T"));
	connect(tileWindowAction, &QAction::triggered, [=]()
	{

	});

	// --------
	sceneMenu->addSeparator();

	// Properties
	QAction* propertiesAction = sceneMenu->addAction(QIcon(":/icons/preferences.png"), "Properties");
	propertiesAction->setShortcut(QKeySequence("Ctrl+W,Ctrl+P"));
	connect(propertiesAction, &QAction::triggered, [=]()
	{
		ScenePropertiesDialog* propertiesDialog = new ScenePropertiesDialog(&sceneData, &sceneView->gridSize, this);
		propertiesDialog->setModal(true);

		if (propertiesDialog->exec() == QDialog::Accepted)
			SetModified();
	});

	mainWindow->menuBar()->insertMenu(mainWindow->ui->menuAnalyze->menuAction(), sceneMenu);

	// Show status bar
	labelCurrentMode->show();
	labelGridSize->show();
	labelZoomFactor->show();
	labelSceneSize->show();
	labelCursorPosition->show();
	labelEntityCount->show();
	labelTileCount->show();

	// Reshow utility windows that were previously opened
	if (sceneView->mode == SceneView::Mode::Entity && entityEditorWindow != nullptr)
	{
		sceneView->IgnoreNextResize();
		entityEditorWindow->show();
	}
}

void SceneEditor::TabFocusOut()
{
	// Show shell window if needed
	if (IdePreferences.autoToggleShellOnScene)
	{
		sceneView->IgnoreNextResize();
		mainWindow->shellDock->show();
	}

	// Destroy scene menu if it exists
	if (sceneMenu != nullptr)
	{
		mainWindow->menuBar()->removeAction(sceneMenu->menuAction());
	}

	// Hide status bar
	labelCurrentMode->hide();
	labelGridSize->hide();
	labelZoomFactor->hide();
	labelSceneSize->hide();
	labelCursorPosition->hide();
	labelEntityCount->hide();
	labelTileCount->hide();

	// Hide utility windows
	if (entityEditorWindow != nullptr)
	{
		// HACK hide() modifies mode, except we don't want it modified in this specific case
		SceneView::Mode mode = sceneView->mode;
		sceneView->IgnoreNextResize();
		entityEditorWindow->hide();
		sceneView->mode = mode;
	}
}

void SceneEditor::Refresh()
{
	// NOTE This method is supposed to refresh the state of the editor
	//		if "external" tools modify its state. For instance the scene
	//		entity editor form changing its currently selected entity.
	sceneView->repaint();
}

SceneEntityForm* SceneEditor::EntityEditor() const
{
	return sceneEntityForm;
}
