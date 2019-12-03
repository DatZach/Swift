/*
 *  MainWindow.ProjectExplorer.cpp
 *  Main Window's Project Explorer
 */

#include <QDebug>
#include <QLineEdit>
#include <QCursor>
#include <MainWindow.hpp>
#include <Forms/AssetForm.hpp>
#include <Forms/TextureEditor.hpp>
#include <Forms/SoundEditor.hpp>
#include <Forms/SceneEditor.hpp>
#include <Forms/ScriptEditor.hpp>
#include <Dialogs/ConfigurationDialog.hpp>
#include <Dialogs/NewAssetDialog.hpp>
#include <Project/Project.hpp>
#include <IdePreferences.hpp>

void MainWindow::CreateProjectExplorer()
{
    // Project Explorer dock
    projectTreeDock = new QDockWidget("Project Explorer", this);
        projectTreeDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
		projectTreeDock->setMinimumSize(240, 128);
	addDockWidget(IdePreferences.projectExplorerDock == 0 ? Qt::RightDockWidgetArea : Qt::LeftDockWidgetArea, projectTreeDock);
	if (!IdePreferences.showProjectExplorer)
		projectTreeDock->hide();

    projectTree = new QTreeWidget(projectTreeDock);
    projectTreeDock->setWidget(projectTree);
        projectTree->setHeaderHidden(true);
        projectTree->setContextMenuPolicy(Qt::CustomContextMenu);
        projectTree->setEditTriggers(QTreeWidget::SelectedClicked);

    connect(projectTree, &QTreeWidget::customContextMenuRequested, [=](QPoint)
    {
        QTreeWidgetItem* item = projectTree->selectedItems().first();
        QMenu* contextMenu = new QMenu(this);

        if (item == nullptr)
            return;

        if (item->parent() == texturesFolder || item == texturesFolder)
        {
            QAction* addTextureAction = new QAction("Add new Texture", projectTree);
            connect(addTextureAction, &QAction::triggered, [=]()
            {
                (new NewAssetDialog(ProjectAssetType::Texture, this))->show();
            });
            contextMenu->addAction(addTextureAction);
        }
        else if (item->parent() == soundsFolder || item == soundsFolder)
        {
            QAction* addSoundAction = new QAction("Add new Sound", projectTree);
            connect(addSoundAction, &QAction::triggered, [=]()
            {
                (new NewAssetDialog(ProjectAssetType::Sound, this))->show();
            });
            contextMenu->addAction(addSoundAction);
        }
        else if (item->parent() == scenesFolder || item == scenesFolder)
        {
            QAction* addSceneAction = new QAction("Add new Scene", projectTree);
            connect(addSceneAction, &QAction::triggered, [=]()
            {
                (new NewAssetDialog(ProjectAssetType::Scene, this))->show();
            });
            contextMenu->addAction(addSceneAction);
        }
        else if (item->parent() == scriptsFolder || item == scriptsFolder)
        {
            QAction* addScriptAction = new QAction("Add new Script", projectTree);
            connect(addScriptAction, &QAction::triggered, [=]()
            {
                (new NewAssetDialog(ProjectAssetType::Script, this))->show();
            });
            contextMenu->addAction(addScriptAction);
        }

        if (item->parent() != nullptr)
        {
            contextMenu->addSeparator();

            // Rename action
            QAction* renameAction = new QAction("Rename", projectTree);
            connect(renameAction, &QAction::triggered, [=]()
            {
                QTreeWidgetItem* item = projectTree->selectedItems().first();
                AssetLink* assetLink = reinterpret_cast<AssetLink*>(item->data(0, Qt::UserRole).value<void*>());
                if (assetLink->opened)
                    return;

                projectTree->editItem(item, 0);
            });

            connect(projectTree->itemDelegate(), &QAbstractItemDelegate::closeEditor,
            [=](QObject*, QAbstractItemDelegate::EndEditHint)
            {
                ProjectExplorerRename(projectTree->selectedItems().first());
            });

            contextMenu->addAction(renameAction);

            // Delete action
            QAction* deleteAction = new QAction("Delete", projectTree);
            connect(deleteAction, &QAction::triggered, this, &MainWindow::ProjectExplorerDelete);
            contextMenu->addAction(deleteAction);
        }

        contextMenu->exec(QCursor::pos());
    });

    connect(projectTree, &QTreeWidget::itemDoubleClicked, this, &MainWindow::ProjectExplorerItemClicked);
    connect(projectTree, &QTreeWidget::itemChanged, [=](QTreeWidgetItem* item, int)
    {
        AssetLink* assetLink = reinterpret_cast<AssetLink*>(item->data(0, Qt::UserRole).value<void*>());
        // TODO Allow renaming while asset's open
        if (assetLink->opened)
        {
            item->setText(0, assetLink->GetAssetName());
            return;
        }

        ProjectExplorerRename(item);
    });
}

void MainWindow::SyncProjectExplorer()
{
    // Clear project tree before we sync it
    projectTree->clear();

    // Header
    QString projectRootName = "Project '" + Project.project->targetName + "'";
    QTreeWidgetItem* projectRoot = new QTreeWidgetItem(QStringList(projectRootName));
        projectRoot->setIcon(0, QIcon(":/icons/application.png"));
    projectTree->addTopLevelItem(projectRoot);

    // Textures
    texturesFolder = new QTreeWidgetItem(QStringList("Textures"));
        texturesFolder->setIcon(0, QIcon(":/icons/folder.png"));
    projectTree->addTopLevelItem(texturesFolder);

    for(ProjectTextures::Texture& texture : Project.textures->textures)
        ProjectExplorerAddItem(new AssetLink(&texture), QIcon(":/icons/rainbow.png"));

    // Sounds
    soundsFolder = new QTreeWidgetItem(QStringList("Sounds"));
        soundsFolder->setIcon(0, QIcon(":/icons/folder.png"));
    projectTree->addTopLevelItem(soundsFolder);

    for(ProjectSounds::Sound& sound : Project.sounds->sounds)
        ProjectExplorerAddItem(new AssetLink(&sound), QIcon(":/icons/sound.png"));

    // Fonts
    fontsFolder = new QTreeWidgetItem(QStringList("Fonts"));
        fontsFolder->setIcon(0, QIcon(":/icons/folder.png"));
    projectTree->addTopLevelItem(fontsFolder);

    for(ProjectFonts::Font& font : Project.fonts->fonts)
        ProjectExplorerAddItem(new AssetLink(&font), QIcon(":/icons/font.png"));

    // Scenes
    scenesFolder = new QTreeWidgetItem(QStringList("Scenes"));
        scenesFolder->setIcon(0, QIcon(":/icons/folder.png"));
    projectTree->addTopLevelItem(scenesFolder);

    for(Scene& scene : Project.scenes)
        ProjectExplorerAddItem(new AssetLink(&scene), QIcon(":/icons/map.png"));

    // Scripts
    scriptsFolder = new QTreeWidgetItem(QStringList("Scripts"));
        scriptsFolder->setIcon(0, QIcon(":/icons/folder.png"));
    projectTree->addTopLevelItem(scriptsFolder);

    for(Script& script : Project.scripts)
        ProjectExplorerAddItem(new AssetLink(&script), QIcon(":/icons/script.png"));

    // Configuration
    ProjectExplorerAddItem(new AssetLink(Project.configuration), QIcon(":/icons/preferences.png"));
}

void MainWindow::ProjectExplorerAddItem(AssetLink* assetLink, const QIcon& icon)
{
    QTreeWidgetItem* folder = nullptr;
    switch(assetLink->GetType())
    {
        case ProjectAssetType::Texture:
            folder = texturesFolder;
            break;

        case ProjectAssetType::Sound:
            folder = soundsFolder;
            break;

        case ProjectAssetType::Font:
            folder = fontsFolder;
            break;

        case ProjectAssetType::Scene:
            folder = scenesFolder;
            break;

        case ProjectAssetType::Script:
            folder = scriptsFolder;
    }

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(assetLink->GetAssetName()));
        item->setIcon(0, icon);
        item->setData(0, Qt::UserRole, qVariantFromValue(static_cast<void*>(assetLink)));
    if (folder != nullptr)
        item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);
    assetLink->treeItem = item;

    if (folder == nullptr)
        projectTree->addTopLevelItem(item);
    else
        folder->addChild(item);
}

void MainWindow::ProjectExplorerItemClicked(QTreeWidgetItem* item, int)
{
    AssetLink* assetLink = reinterpret_cast<AssetLink*>(item->data(0, Qt::UserRole).value<void*>());
    if (assetLink == nullptr)
        return;

    if (!assetLink->opened)
    {
        switch(assetLink->GetType())
        {
            case ProjectAssetType::Texture:
            {
                ProjectTextures::Texture* texture = (ProjectTextures::Texture*)assetLink->asset;
                assetLink->tabItem = new TextureEditor(assetLink, workspaceTabView);
                workspaceTabView->addTab(assetLink->tabItem, QIcon(":/icons/rainbow.png"), texture->name);
                break;
            }

            case ProjectAssetType::Sound:
            {
                ProjectSounds::Sound* sound = (ProjectSounds::Sound*)assetLink->asset;
                assetLink->tabItem = new SoundEditor(assetLink, workspaceTabView);
                workspaceTabView->addTab(assetLink->tabItem, QIcon(":/icons/sound.png"), sound->name);
                break;
            }

            case ProjectAssetType::Scene:
            {
                Scene* scene = (Scene*)assetLink->asset;
                assetLink->tabItem = new SceneEditor(assetLink, workspaceTabView);
                workspaceTabView->addTab(assetLink->tabItem, QIcon(":/icons/map.png"), scene->name);
                break;
            }

            case ProjectAssetType::Script:
            {
                Script* script = (Script*)assetLink->asset;
                assetLink->tabItem = new ScriptEditor(assetLink, workspaceTabView);
                workspaceTabView->addTab(assetLink->tabItem, QIcon(":/icons/script.png"), script->name);
                break;
            }

            case ProjectAssetType::Configuration:
            {
                ProjectConfiguration* projectConfiguration = (ProjectConfiguration*)assetLink->asset;
                ConfigurationDialog* configurationDialog = new ConfigurationDialog(projectConfiguration, this);
                configurationDialog->show();
            }
        }
    }

    int tabIndex = workspaceTabView->indexOf(assetLink->tabItem);
    workspaceTabView->setCurrentIndex(tabIndex);
}

void MainWindow::ProjectExplorerRename(QTreeWidgetItem* item)
{
    if (item == nullptr)
        return;

    AssetLink* assetLink = reinterpret_cast<AssetLink*>(item->data(0, Qt::UserRole).value<void*>());
    QString newName = item->text(0);

    // TODO Allow renaming while asset's open
    if (assetLink->opened)
        return;

    switch(assetLink->GetType())
    {
        case ProjectAssetType::Texture:
        {
            ProjectTextures::Texture* texture = dynamic_cast<ProjectTextures::Texture*>(assetLink->asset);
            texture->name = newName;
            Project.textures->Save();
            break;
        }

        case ProjectAssetType::Sound:
        {
            ProjectSounds::Sound* sound = dynamic_cast<ProjectSounds::Sound*>(assetLink->asset);
            sound->name = newName;
            Project.sounds->Save();
            break;
        }

        case ProjectAssetType::Scene:
        {
            // Don't allow us to break projects
            if (!newName.endsWith(".sd"))
                newName += ".sd";

            Scene* scene = dynamic_cast<Scene*>(assetLink->asset);
            if (!scene->Rename(newName))
            {
                QMessageBox::critical(this, "Error", "Cannot rename scene.");
                item->setText(0, scene->name);
                break;
            }

            scene->name = newName;
            item->setText(0, scene->name);
            break;
        }

        case ProjectAssetType::Script:
        {
            // Don't allow us to break projects
            if (!newName.endsWith(".ss"))
                newName += ".ss";

            Script* script = dynamic_cast<Script*>(assetLink->asset);
            if (!script->Rename(newName))
            {
                QMessageBox::critical(this, "Error", "Cannot rename script.");
                item->setText(0, script->name);
            }

            script->name = newName;
            item->setText(0, script->name);
            break;
        }
    }
}

void MainWindow::ProjectExplorerDelete()
{
    QTreeWidgetItem* item = projectTree->selectedItems().first();
    AssetLink* assetLink = reinterpret_cast<AssetLink*>(item->data(0, Qt::UserRole).value<void*>());
    if (assetLink == nullptr)
        return;

    // Make sure they actually want to delete this
    int index = QMessageBox::question(this, "Delete this asset?",
                                      "Are you sure you want to delete \"" + assetLink->GetAssetName() + "\".",
                                      QMessageBox::Yes, QMessageBox::No);
    if (index == QMessageBox::No)
        return;

    // Remove tab
    workspaceTabView->removeTab(workspaceTabView->indexOf(assetLink->tabItem));

	// Force close event to trigger
	try
	{
		AssetForm* assetForm = dynamic_cast<AssetForm*>(assetLink->tabItem);
		if (assetForm != nullptr)
		{
			assetForm->TabFocusOut();
			assetForm->close();
		}
	}
	catch(std::bad_cast)
	{
		// Wahh, cry about it
	}

    // Remove specific resource
    switch(assetLink->GetType())
    {
        case ProjectAssetType::Texture:
            texturesFolder->removeChild(assetLink->treeItem);
            Project.textures->Delete(dynamic_cast<ProjectTextures::Texture*>(assetLink->asset));
            break;

        case ProjectAssetType::Sound:
            soundsFolder->removeChild(assetLink->treeItem);
            Project.sounds->Delete(dynamic_cast<ProjectSounds::Sound*>(assetLink->asset));
            break;

        case ProjectAssetType::Scene:
            scenesFolder->removeChild(assetLink->treeItem);
            Project.DeleteScene(dynamic_cast<Scene*>(assetLink->asset));
            break;

        case ProjectAssetType::Script:
            scriptsFolder->removeChild(assetLink->treeItem);
            Project.DeleteScript(dynamic_cast<Script*>(assetLink->asset));
            break;
    }
}
