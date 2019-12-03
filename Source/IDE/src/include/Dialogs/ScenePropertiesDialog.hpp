/*
 *  ScenePropertiesDialog.hpp
 *  Scene Properties Dialog
 */

#ifndef __SCENE_PROPERTIES_DIALOG_HPP
#define __SCENE_PROPERTIES_DIALOG_HPP

#include <QDialog>
#include <QColor>
#include <Project/SceneData.hpp>

namespace Ui
{
    class ScenePropertiesDialog;
}

class ScenePropertiesDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ScenePropertiesDialog* ui;
    QColor bgColor;
    int* gridSize; // Yeah it's a hack, think I give a fuck?
    SceneData::SceneData* sceneData;

    void SetupUi();
    void SetupConnections();

public:
    explicit ScenePropertiesDialog(SceneData::SceneData* sceneData, int* gridSize, QWidget* parent = nullptr);
    ~ScenePropertiesDialog();
};

#endif
