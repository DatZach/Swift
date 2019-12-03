/*
 *	IdePreferences.hpp
 *	IDE Global Preferences
 */

#ifndef __IDE_PREFERENCES_HPP
#define __IDE_PREFERENCES_HPP

#include <QSize>
#include <QPoint>
#include <Project/JsonObject.hpp>

#define IdePreferences		IdePreferencesSingleton::GetInstance()

class IdePreferencesSingleton : public JsonObject
{
public:
	QPoint	windowPosition;
	QSize	windowSize;
	bool	windowMaximized;
	bool	showWelcomeDialog;
	bool	welcomeDialogClosesMainWindow;

	bool	showProjectExplorer;
	bool	showCompileOutput;
	bool	showEngineDebug;
	int		projectExplorerDock;

	bool	autoToggleShellOnRun;
	bool	autoToggleShellOnScene;

	bool	compileDisassemble;

public:
	IdePreferencesSingleton();

	bool Load();
	bool Save();

	static IdePreferencesSingleton& GetInstance();
};

#endif
