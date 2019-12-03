/*
 *  AboutDialog.cpp
 *  About Dialog
 */

#include <Dialogs/AboutDialog.hpp>
#include <Compiler.hpp>
#include <Util.hpp>
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    InitializeDefaultDialog();

    // Set compiler version text
    QString compilerVersion = Compiler.Execute("version");
    if (compilerVersion.length() > 0)
        ui->labelCompilerVersion->setText(compilerVersion.remove(0, 15));

    // Set revision text
    QFile revisionFile("revision");
    if (revisionFile.open(QIODevice::ReadOnly))
    {
        ui->labelRevision->setText(revisionFile.readLine());
        revisionFile.close();
    }

    // Set license key text
    QFile licenseKeyFile("licensekey");
    if (licenseKeyFile.open(QIODevice::ReadOnly))
    {
        ui->labelLicenseKey->setText(licenseKeyFile.readLine());
        licenseKeyFile.close();
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::mousePressEvent(QMouseEvent*)
{
    close();
}
