/*
 *  AssetForm.hpp
 *  Asset Form
 */

#ifndef __ASSET_FORM_HPP
#define __ASSET_FORM_HPP

#include <QtWidgets>
#include <QTreeWidgetItem>
#include <Project/Project.hpp>

class AssetLink
{
public:
    //int         		tabItemIndex;
    QWidget*            tabItem;
    QTreeWidgetItem*	treeItem;
    ProjectAsset*		asset;
    bool                opened;

public:
    AssetLink(ProjectAsset* asset);

    ProjectAssetType GetType() const;
    QString GetAssetName() const;
};

class AssetForm : public QWidget
{
    Q_OBJECT

private:
    AssetLink* assetLink;
    bool modified;

protected:
    void SetModified(bool value = true);
    AssetLink* GetAssetLink() const;

    void closeEvent(QCloseEvent*);

public:
    AssetForm(AssetLink* assetLink, QWidget* parent = nullptr);

    virtual void Save() = 0;

	virtual void TabFocusIn();
	virtual void TabFocusOut();
};

#endif
