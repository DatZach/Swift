/*
 *  NewAssetDialog.cpp
 *  New Asset Dialog
 */

#ifndef __NEW_ASSET_DIALOG_HPP
#define __NEW_ASSET_DIALOG_HPP

#include <QDialog>
#include <Project/Project.hpp>

namespace Ui
{
    class NewAssetDialog;
}

class NewAssetDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::NewAssetDialog* ui;

    void SetupUi();
    void SetupConnections();

    void CreateAsset();
    bool CreateTexture();
    bool CreateSound();
    bool CreateScene();
    bool CreateScript();

public:
    explicit NewAssetDialog(ProjectAssetType type, QWidget* parent = nullptr);
    ~NewAssetDialog();
};

#endif
