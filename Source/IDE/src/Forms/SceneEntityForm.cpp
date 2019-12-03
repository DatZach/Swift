/*
 *	SceneEntityForm.cpp
 *	Entity Editor - Scene Editor
 */

#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QShortcut>
#include <QPaintEvent>
#include <QGridLayout>
#include <Forms/SceneEditor.hpp>
#include <Forms/SceneEntityForm.hpp>
#include <Project/Project.hpp>
#include <Project/Lexer/TokenStream.hpp>
#include "ui_SceneEntityEditorForm.h"

// ==== EntityListModel ====

// Let's just make a whole new model just so we get icons
class EntityListModel : public QStringListModel
{
	// NOTE Do not add Q_OBJECT, generates linker errors for some reason

private:
	QModelIndex currentIndex;

public:
	EntityListModel(const QStringList& value, QObject* parent = nullptr)
		: QStringListModel(value, parent),
		  currentIndex()
	{
		sort(0);
	}

	QVariant data(const QModelIndex& index, int role) const
	{
		if (role == Qt::DecorationRole)
			return QIcon(":/icons/brick.png");

		if (role == Qt::FontRole && index.row() == currentIndex.row())
		{
			QFont font = QStringListModel::data(index, role).value<QFont>();
			font.setBold(true);

			return font;
		}

		return QStringListModel::data(index, role);
	}

	void SetCurrentIndex(const QModelIndex& currentIndex)
	{
		this->currentIndex = currentIndex;
		dataChanged(currentIndex, currentIndex);
	}
};

// ==== Entity Preview ===

EntityPreview::EntityPreview(QWidget* parent)
	: QWidget(parent),
	  bgGrid(),
	  entity(nullptr)
{
	CreateBgGrid();
}

void EntityPreview::CreateBgGrid()
{
	QPainter painter;

	bgGrid = QPixmap(96, 96);
	bgGrid.fill(QColor(212, 212, 212));

	painter.begin(&bgGrid);
	QBrush brush(Qt::white, Qt::SolidPattern);
	painter.fillRect(QRect(0, 0, 48, 48), brush);
	painter.fillRect(QRect(48, 48, 48, 48), brush);
	painter.end();
}

void EntityPreview::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	// Solid gray background
	painter.setBrush(QColor(192, 192, 192));
	painter.setPen(QColor(144, 144, 144));
	painter.drawRect(0, 0, event->rect().width() - 1, event->rect().height() - 1);

	// Checkered background
	painter.drawTiledPixmap(QRect(0, 0, event->rect().width(), event->rect().height()),
							bgGrid);

	// Draw sprite
	if (entity != nullptr)
	{
		const QPixmap& pixmap = entity->Texture();
		int x = event->rect().width() / 2 - pixmap.width() / 2;
		int y = event->rect().height() / 2 - pixmap.height() / 2;
		painter.drawPixmap(x, y, pixmap);
	}

	painter.end();

	event->accept();
}

int EntityPreview::heightForWidth(int w) const
{
	return w;
}

bool EntityPreview::hasHeightForWidth() const
{
	return true;
}

void EntityPreview::SetCurrentEntity(SceneEntityForm::Entity* entity)
{
	this->entity = entity;
	repaint();
}

SceneEntityForm::Entity* EntityPreview::GetCurrentEntity() const
{
	return entity;
}

// ==== SceneEntityForm ====

SceneEntityForm::Entity::Entity(const QString& name, const QString& textureIdent)
	: name(name),
	  texture()
{
	ProjectTextures::Texture properties;

	// Look through every texture to find this texture
	// TODO Should just add an operator [] overload into Project.textures
	for(ProjectTextures::Texture& value : Project.textures->textures)
	{
		if (value.name != textureIdent)
			continue;

		properties = value;
		break;
	}

	QDir texturePath(Project.projectRootDirectory);
	texturePath.cd("Textures");
	QString filePath = texturePath.filePath(properties.filename);

	if (!texture.load(filePath))
		texture.load(":/icons/brick.png");
}

const QString& SceneEntityForm::Entity::Name() const
{
	return name;
}

const QPixmap& SceneEntityForm::Entity::Texture() const
{
	return texture;
}

#include <QDebug>

SceneEntityForm::SceneEntityForm(SceneEditor* editor, QWidget* parent)
	: QWidget(parent),
	  ui(new Ui::SceneEntityEditorForm),
	  entityPreview(nullptr),
	  entities(),
	  editor(editor)
{
	ui->setupUi(this);

	SetupUi();
	SetupConnections();
}

SceneEntityForm::~SceneEntityForm()
{
	delete ui;
}

void SceneEntityForm::DiscoverEntities()
{
	for(Script& script : Project.scripts)
	{
		bool ok;
		Lexer::TokenStream stream(script.GetPath(), ok);
		if (!ok)
			continue;

		// Only process entities
		if (!stream.Accept("entity"))
			continue;

		QString ident = stream.ReadWord();

		// Process texture identifier
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

		// Add this entity
		entities.push_back(Entity(ident, textureIdent));
	}
}

void SceneEntityForm::SetupUi()
{
	// Discover entities before setting up the UI
	DiscoverEntities();

	// Now populate a list of entity names
	QStringList entityNames;
	for(Entity& entity : entities)
		entityNames << entity.Name();

	// Create filter proxy and list view model
	QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
	proxy->setSourceModel(new EntityListModel(entityNames));
	proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->entitiesList->setModel(proxy);
	ui->entitiesList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// Filter entity list on the fly
	connect(ui->filterLineEdit, &QLineEdit::textChanged, [=](const QString& text)
	{
		proxy->setFilterWildcard(text);
	});

	// Set an alphanumeric validator for the filter line edit
	ui->filterLineEdit->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z0-9_]*$")));

	// Create preview panel
	QFrame* frame = new QFrame(this);
		frame->setFrameShadow(QFrame::Raised);
		frame->setFrameStyle(QFrame::StyledPanel);

	QGridLayout* gridLayoutTarget = new QGridLayout(frame);
		gridLayoutTarget->setSpacing(0);
		gridLayoutTarget->setContentsMargins(0, 0, 0, 0);

	frame->setLayout(gridLayoutTarget);

	entityPreview = new EntityPreview(this);
	gridLayoutTarget->addWidget(entityPreview);

	ui->verticalLayout->insertWidget(0, frame);
}

void SceneEntityForm::SetupConnections()
{
	// When we press return we need to switch focus to entity list
	connect(ui->filterLineEdit, &QLineEdit::returnPressed, [=]()
	{
		ui->entitiesList->setCurrentIndex(ui->entitiesList->indexAt(QPoint(0, 0)));
		ui->entitiesList->setFocus();
	});

	// Update currently selected entity
	connect(ui->entitiesList, &QListView::activated, [=](const QModelIndex& index)
	{
		// HACK Tell our model what the currently selected item is so its data()
		//		can style everything correctly...
		QSortFilterProxyModel* proxy = reinterpret_cast<QSortFilterProxyModel*>(ui->entitiesList->model());
		reinterpret_cast<EntityListModel*>(proxy->sourceModel())->SetCurrentIndex(proxy->mapToSource(index));

		// Grab name of selected entity
		QString name = ui->entitiesList->model()->data(index, Qt::DisplayRole).toString();

		for(Entity& entity : entities)
		{
			if (entity.Name() != name)
				continue;

			entityPreview->SetCurrentEntity(&entity);
			editor->Refresh();
			return;
		}

		entityPreview->SetCurrentEntity(nullptr);
	});

	// Control + F to set focus on the filter
	QShortcut* filterShortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
	connect(filterShortcut, &QShortcut::activated, [=]()
	{
		ui->filterLineEdit->setFocus();
	});
}

void SceneEntityForm::hideEvent(QHideEvent* event)
{
	editor->sceneView->mode = SceneView::Mode::View;
	event->accept();
}

SceneEntityForm::Entity* SceneEntityForm::GetCurrentEntity() const
{
	return entityPreview->GetCurrentEntity();
}
