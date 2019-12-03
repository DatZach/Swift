/*
 *  Util.hpp
 *  Utilities
 */

#ifndef __UTILITIES_HPP
#define __UTILITIES_HPP

#include <QDesktopWidget>

#define NoHelpButton() \
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint)

#define CenterWidget() \
    setGeometry(                                    \
        QStyle::alignedRect(                        \
            Qt::LeftToRight,                        \
            Qt::AlignCenter,                        \
            size(),                                 \
            qApp->desktop()->availableGeometry()    \
        )                                           \
    )

#define InitializeDefaultDialog() \
	NoHelpButton();				  \
    CenterWidget();

class Util
{
public:
    static void OpenPathInShell(const QString& path);
    static QString RStrip(const QString& str);
	static int SnapValue(int value, int x);
};

#endif
