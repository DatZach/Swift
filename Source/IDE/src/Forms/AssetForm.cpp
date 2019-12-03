/*
 *  AssetForm.cpp
 *  Asset Form
 */

#include <QCloseEvent>
#include <MainWindow.hpp>
#include <Forms/AssetForm.hpp>

extern MainWindow* mainWindow;

AssetLink::AssetLink(ProjectAsset* asset)
    : asset(asset),
      tabItem(nullptr),
      treeItem(nullptr),
      opened(false)
{

}

ProjectAssetType AssetLink::GetType() const
{
    if (dynamic_cast<ProjectTextures::Texture*>(asset))
        return ProjectAssetType::Texture;

    if (dynamic_cast<ProjectSounds::Sound*>(asset))
        return ProjectAssetType::Sound;

    if (dynamic_cast<ProjectFonts::Font*>(asset))
        return ProjectAssetType::Font;

    if (dynamic_cast<Scene*>(asset))
        return ProjectAssetType::Scene;

    if (dynamic_cast<Script*>(asset))
        return ProjectAssetType::Script;

    if (dynamic_cast<ProjectConfiguration*>(asset))
        return ProjectAssetType::Configuration;

    return ProjectAssetType::Unknown;
}

QString AssetLink::GetAssetName() const
{
    if (auto* texture = dynamic_cast<ProjectTextures::Texture*>(asset))
        return texture->name;

    if (auto* sound = dynamic_cast<ProjectSounds::Sound*>(asset))
        return sound->name;

    if (auto* font = dynamic_cast<ProjectFonts::Font*>(asset))
        return font->name;

    if (auto* scene = dynamic_cast<Scene*>(asset))
        return scene->name;

    if (auto* script = dynamic_cast<Script*>(asset))
        return script->name;

    if (dynamic_cast<ProjectConfiguration*>(asset))
        return "Configuration";

	return "<Unknown Asset>";
}

AssetForm::AssetForm(AssetLink* assetLink, QWidget* parent)
    : QWidget(parent),
      assetLink(assetLink),
      modified(false)
{

}

void AssetForm::SetModified(bool value)
{
    modified = value;

    int tabIndex = mainWindow->workspaceTabView->indexOf(assetLink->tabItem);
    QString tabText = assetLink->GetAssetName() + (modified ? "*" : "");
    mainWindow->workspaceTabView->setTabText(tabIndex, tabText);
}

AssetLink* AssetForm::GetAssetLink() const
{
    return assetLink;
}

void AssetForm::closeEvent(QCloseEvent* event)
{
    if (!modified)
    {
        assetLink->opened = false;
        assetLink->tabItem = nullptr;
        event->accept();
        return;
    }

    int index = QMessageBox::question(this, "Save changes?", "You have unsaved changes. "
                                                             "Would you like to save them?",
                                      QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
    switch(index)
    {
        case QMessageBox::Yes:
            Save();
            event->accept();
            break;

        case QMessageBox::No:
            event->accept();
            break;

        case QMessageBox::Cancel:
        default:
            event->ignore();
            return;
    }

    modified = false;
    assetLink->opened = false;
    assetLink->tabItem = nullptr;
}

void AssetForm::TabFocusIn()
{

}

void AssetForm::TabFocusOut()
{

}
