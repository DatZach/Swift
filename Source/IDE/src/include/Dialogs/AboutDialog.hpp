/*
 *  AboutDialog.hpp
 *  About Dialog
 */

#ifndef __ABOUT_DIALOG_HPP
#define __ABOUT_DIALOG_HPP

#include <QDialog>

namespace Ui
{
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::AboutDialog* ui;

protected:
    void mousePressEvent(QMouseEvent*);

public:
    explicit AboutDialog(QWidget* parent = nullptr);
    ~AboutDialog();
};

#endif
