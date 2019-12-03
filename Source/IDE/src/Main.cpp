/*
 *	Main.cpp
 *	Swift IDE
 */

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QFontDatabase>
#include <QString>
#include <QProcess>
#include <MainWindow.hpp>
#include <IdePreferences.hpp>

MainWindow* mainWindow;

void LoadFonts()
{
    QStringList list;
    list << "Inconsolata.otf"
         << "Titillium-Regular.otf"
         << "Titillium-RegularUpright.otf";

    auto constIterator = list.constBegin();
    for (; constIterator != list.constEnd(); ++constIterator)
    {
        QFile res(":/fonts/" + *constIterator);
        if (!res.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(nullptr, "Application", "Cannot load embedded font.");
            return;
        }

        int fontId = QFontDatabase::addApplicationFontFromData(res.readAll());
        if (fontId == -1)
            QMessageBox::warning(nullptr, "Application", "Cannot load embedded font.");
    }
}

bool CheckForUpdates(const QStringList& cmdLineArguments)
{
#ifdef QT_NO_DEBUG
    QStringList arguments;
	arguments << "--check-only";

    QProcess updaterProcess;
    updaterProcess.start("updater.exe", arguments);
    updaterProcess.waitForFinished();

    QByteArray result = updaterProcess.readAll();
    if (result.simplified() == "true")
    {
		QStringList updateArguments;
		if (!cmdLineArguments.empty())
		{
			for(QString arg : cmdLineArguments)
				updateArguments << arg;
		}

		if (updaterProcess.startDetached("updater.exe", updateArguments))
            return true;
    }
#endif

    return false;
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QDir::setCurrent(QCoreApplication::applicationDirPath());

	QStringList cmdLineArguments;
	for(int i = 1; i < argc; ++i)
		cmdLineArguments << argv[i];

	if (CheckForUpdates(cmdLineArguments))
        return 0;

    LoadFonts();

	mainWindow = new MainWindow(cmdLineArguments);
	if (IdePreferences.windowMaximized)
		mainWindow->showMaximized();
	else
		mainWindow->show();

	return app.exec();
}
