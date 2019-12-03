#include <Qsci/qsciscintilla.h>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    textEdit = new QsciScintilla;
    setCentralWidget(textEdit);
}
