#ifndef BASICINFORMATIONDIALOG_H
#define BASICINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class BasicInformationDialog;
}

class BasicInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BasicInformationDialog(QWidget *parent = 0);
    void changeEvent(QEvent *e);
    ~BasicInformationDialog();

private slots:
    void get_system_base(QString hw_ver, QString sw_ver, QString build_time, QString mac, QString mac_wireless);

private:
    Ui::BasicInformationDialog *ui;
};

#endif // BASICINFORMATIONDIALOG_H
