#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QsciScintilla;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    QsciScintilla* textEdit;
};

#endif // MAINWINDOW_H
