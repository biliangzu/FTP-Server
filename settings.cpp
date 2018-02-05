#include "settings.h"
#include "ui_settings.h"
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

//    ui->serverPort->setText((settings.value("serverPort").isNull()) ? QString::number(1234) : settings.value("serverPort").toString());
//    ui->maxUsers->setValue((settings.value("maxUsers").isNull()) ? 5 : settings.value("maxUsers").toInt());
//    ui->rootPath->setText((settings.value("rootPath").isNull()) ? QDir::homePath() : settings.value("rootPath").toString());
//    ui->welcomeMessage->setText((settings.value("welcomeMessage").isNull()) ? "Welcome to Jussie's FTP Server!" : settings.value("welcomeMessage").toString());
//    ui->allowAnon->setChecked((settings.value("allowAnonUsers").isNull()) ? false : settings.value("allowAnonUsers").toBool());

    ui->serverPort->setText(settings.value("serverPort").toString());
    ui->maxUsers->setValue(settings.value("maxUsers").toInt());
    ui->rootPath->setText(settings.value("rootPath").toString());
    ui->welcomeMessage->setText(settings.value("welcomeMessage").toString());
    ui->allowAnon->setChecked(settings.value("allowAnonUsers").toBool());
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_cancelButton_clicked()
{
    this->close();
}

void Settings::on_saveButton_clicked()
{
    settings.setValue("serverPort", ui->serverPort->text().toInt());
    settings.setValue("maxUsers", ui->maxUsers->text().toInt());
    settings.setValue("rootPath", ui->rootPath->text());
    settings.setValue("welcomeMessage", ui->welcomeMessage->text());
    settings.setValue("allowAnonUsers", ui->allowAnon->isChecked());

    this->close();
}

void Settings::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 QDir::homePath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->rootPath->setText(dir);
}
