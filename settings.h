#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QFileDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:
    void on_cancelButton_clicked();
    void on_saveButton_clicked();
    void on_browseButton_clicked();

private:
    Ui::Settings *ui;
    QSettings settings;
};

#endif // SETTINGS_H
