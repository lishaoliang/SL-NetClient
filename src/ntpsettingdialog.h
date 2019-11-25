#ifndef NTPSETTINGDIALOG_H
#define NTPSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class NTPSettingDialog;
}

class NTPSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NTPSettingDialog(QWidget *parent = 0);
    void changeEvent(QEvent *e);
    ~NTPSettingDialog();

private slots:
    void on_ok_btn_clicked();
    void on_cancel_btn_clicked();
    void get_timeRange(int min, int max);
    void get_NTP(bool enable, QString server, int port, int interval);
    void on_spinBox_valueChanged(int arg1);
//    void on_sync_btn_clicked();

private:
    Ui::NTPSettingDialog *ui;
signals:
    void sendNTPSetting(QString enable, QString server, int port, int interval);
    void sync(QString server, int port);
};

#endif // NTPSETTINGDIALOG_H
