/*
 *	IdePreferences.cpp
 *	IDE Global Preferences
 *
 *	TODO Use QSettings instead!
 */

#include <QSettings>
#include <IdePreferences.hpp>
#include <MainWindow.hpp>

extern MainWindow* mainWindow;

IdePreferencesSingleton::IdePreferencesSingleton()
	: JsonObject(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("preferences.json")),
	  windowPosition(80, 80),
	  windowSize(1280, 720),
	  windowMaximized(true),
	  showWelcomeDialog(true),
	  welcomeDialogClosesMainWindow(true),
	  showProjectExplorer(true),
	  showCompileOutput(false),
	  showEngineDebug(false),
	  projectExplorerDock(0),
	  autoToggleShellOnRun(false),
	  autoToggleShellOnScene(true),
	  compileDisassemble(false)
{
	if (!Load())
		Save();
}

bool IdePreferencesSingleton::Load()
{
	if (!LoadJson())
		return false;

	showWelcomeDialog = GetBoolean("showWelcomeDialog", showWelcomeDialog);
	welcomeDialogClosesMainWindow = GetBoolean("welcomeDialogClosesMainWindow", welcomeDialogClosesMainWindow);
	windowMaximized = GetBoolean("windowMaximized", windowMaximized);
	if (!windowMaximized)
	{
		JsonObject mapWinSize(GetMap("windowSize"));
		windowSize = QSize(mapWinSize.GetInteger("width", windowSize.width()),
						   mapWinSize.GetInteger("height", windowSize.height()));
		JsonObject mapWinPos(GetMap("windowPosition"));
		windowPosition = QPoint(mapWinPos.GetInteger("x", windowPosition.x()),
								mapWinPos.GetInteger("y", windowPosition.y()));
	}

	showCompileOutput = GetBoolean("showCompileOutput", showCompileOutput);
	showEngineDebug = GetBoolean("showEngineDebug", showEngineDebug);
	projectExplorerDock = GetInteger("projectExplorerDock", projectExplorerDock);
	autoToggleShellOnRun = GetBoolean("autoToggleShellOnRun", autoToggleShellOnRun);
	autoToggleShellOnScene = GetBoolean("autoToggleShellOnScene", autoToggleShellOnScene);
	showProjectExplorer = GetBoolean("showProjectExplorer", showProjectExplorer);
	compileDisassemble = GetBoolean("compileDisassemble", compileDisassemble);

	return true;
}

bool IdePreferencesSingleton::Save()
{
	QVariantMap map;

	map.insert("showWelcomeDialog", showWelcomeDialog);
	map.insert("welcomeDialogClosesMainWindow", welcomeDialogClosesMainWindow);
	map.insert("windowMaximized", windowMaximized);
	QVariantMap mapWinSize;
		mapWinSize.insert("width", windowSize.width());
		mapWinSize.insert("height", windowSize.height());
	map.insert("windowSize", mapWinSize);
	QVariantMap mapWinPos;
		mapWinPos.insert("x", windowPosition.x());
		mapWinPos.insert("y", windowPosition.y());
	map.insert("windowPosition", mapWinPos);
	map.insert("showCompileOutput", showCompileOutput);
	map.insert("showEngineDebug", showEngineDebug);
	map.insert("projectExplorerDock", projectExplorerDock);
	map.insert("autoToggleShellOnRun", autoToggleShellOnRun);
	map.insert("autoToggleShellOnScene", autoToggleShellOnScene);
	map.insert("showProjectExplorer", showProjectExplorer);
	map.insert("compileDisassemble", compileDisassemble);

	return SaveJson(map);
}

IdePreferencesSingleton& IdePreferencesSingleton::GetInstance()
{
	static IdePreferencesSingleton instance;
	return instance;
}
