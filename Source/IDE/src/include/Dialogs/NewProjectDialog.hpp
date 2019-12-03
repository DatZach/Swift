/*
 *  NewProjectDialog.hpp
 *  New Project Dialog
 */

#ifndef __NEW_PROJECT_DIALOG_HPP
#define __NEW_PROJECT_DIALOG_HPP

#include <QDialog>

namespace Ui
{
    class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::NewProjectDialog* ui;

    void SetupUi();
    void SetupConnections();
    QString GetDesktopPath() const;

public:
    explicit NewProjectDialog(QWidget* parent = nullptr);
    ~NewProjectDialog();

private slots:
    void BrowseLocation();
    void Accept();
};

#endif
