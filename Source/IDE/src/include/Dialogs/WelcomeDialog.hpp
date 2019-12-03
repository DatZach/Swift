/*
 *  WelcomeDialog.hpp
 *  Welcome Dialog
 */

#ifndef __WELCOME_DIALOG_HPP
#define __WELCOME_DIALOG_HPP

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui
{
    class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::WelcomeDialog* ui;
    bool shouldCloseParent;
	bool canCloseIde;

    void SetupUi();
    void SetupConnections();

private slots:
    void CreateNewProject();
    void OpenProject();
    void OpenRecentProject(QTreeWidgetItem* item, int index);

protected:
    void closeEvent(QCloseEvent* event);

public:
	explicit WelcomeDialog(bool canCloseIde, QWidget* parent = nullptr);
    ~WelcomeDialog();
};

#endif
