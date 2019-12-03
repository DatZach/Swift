/*
 *  SoundEditor.hpp
 *  Sound Editor Form
 */

#ifndef __SOUND_FORM_HPP
#define __SOUND_FORM_HPP

#include <QWidget>
#include <QCloseEvent>
#include <Forms/AssetForm.hpp>

namespace Ui
{
    class SoundEditor;
}

class SoundEditor : public AssetForm
{
    Q_OBJECT

private:
    Ui::SoundEditor* ui;
    ProjectSounds::Sound* sound;

    void SetupUi();
    void SetupConnections();

public:
    explicit SoundEditor(AssetLink* sound, QWidget* parent = nullptr);
    ~SoundEditor();

    void Save();
};

#endif
