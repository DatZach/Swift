/*
 *  ConfigurationDialog.hpp
 *  Configuration Dialog
 */

#ifndef __CONFIGURATION_DIALOG_HPP
#define __CONFIGURATION_DIALOG_HPP

#include <QDialog>
#include <Project/Project.hpp>

namespace Ui
{
    class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ConfigurationDialog* ui;
    ProjectConfiguration* configuration;

    void SetupUi();
    void SetupConnections();
	void ParseStates();

public:
    explicit ConfigurationDialog(ProjectConfiguration* configuration, QWidget* parent = nullptr);
    ~ConfigurationDialog();
};

#endif
