/*
 *  ScriptEditor.hpp
 *  Script Editor Form
 */

#ifndef __SCRIPT_EDITOR_HPP
#define __SCRIPT_EDITOR_HPP

#include <QFileSystemWatcher>
#include <QFrame>
#include <QGridLayout>
#include <Forms/AssetForm.hpp>
#include <Forms/JsEdit.hpp>

class ScriptEditor : public AssetForm
{
    Q_OBJECT

private:
	QFileSystemWatcher watcher;
    QFrame frameTarget;
    QGridLayout gridLayoutTarget;
    JSEdit editor;
    Script* asset;

    void SetupUi();
    void SetupConnections();
    void LoadAsset();

public:
    explicit ScriptEditor(AssetLink* script, QWidget* parent = nullptr);
    ~ScriptEditor();

    void Save();

	void TabFocusIn();
	void TabFocusOut();

    void RunToLine(int line);
    void ClearErrors();
};

#endif
