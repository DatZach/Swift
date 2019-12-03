/*
 *  mainwindow.hpp
 *  Main Window
 */

#ifndef __MAIN_WINDOW_HPP
#define __MAIN_WINDOW_HPP

#include <QMainWindow>

namespace Ui
{
   class MainWindow;
}

class QAction;
class QMenu;
class QsciScintilla;

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    QsciScintilla *textEdit;

public:
    MainWindow();
    ~MainWindow();
};

#endif
