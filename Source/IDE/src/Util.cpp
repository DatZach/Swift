/*
 *  Util.cpp
 *  Utilities
 */

#include <QDir>
#include <QProcess>
#include <Util.hpp>

void Util::OpenPathInShell(const QString& path)
{
    QStringList args;

#ifdef Q_OS_MAC
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "activate";
    args << "-e";
    args << "open POSIX file \"" + path + "\"";
    args << "-e";
    args << "end tell";
    QProcess::startDetached("osascript", args);
#endif

#ifdef Q_OS_WIN
    args << "/open," << QDir::toNativeSeparators(path);
    QProcess::startDetached("explorer", args);
#endif
}

QString Util::RStrip(const QString& str)
{
    int n = str.size() - 1;
    for (; n >= 0; --n)
    {
        if (!str.at(n).isSpace())
            return str.left(n + 1);
    }

    return "";
}

int Util::SnapValue(int value, int x)
{
	if (x == 0)
		return value;

	while(value % x != 0)
		--value;

	return value;
}
