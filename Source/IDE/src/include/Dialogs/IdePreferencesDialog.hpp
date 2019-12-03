/*
 *	IdePreferencesDialog.hpp
 *	IDE Preferences Dialog
 */

#ifndef __IDE_PREFERENCESDIALOG_HPP
#define __IDE_PREFERENCESDIALOG_HPP

#include <Windows.h>
#include <QDialog>

namespace Ui
{
	class IdePreferencesDialog;
}

class IdePreferencesDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::IdePreferencesDialog* ui;

	void SetupUi();
	void SetupConnections();

	void CreateKeyWithDefaultValue(HKEY parent, const QString& path, const QString& value);

public:
	explicit IdePreferencesDialog(QWidget* parent = nullptr);
	~IdePreferencesDialog();
};

#endif
