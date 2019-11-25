#ifndef OSDSETTINGDIALOG_H
#define OSDSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class OSDSettingDialog;
}

class OSDSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OSDSettingDialog(QWidget *parent = 0);
    void changeEvent(QEvent *e);
    ~OSDSettingDialog();

private slots:
    void on_ok_btn_clicked();
    void on_cancel_btn_clicked();
    void get_OSD(bool enable, QString format, QString pos, QString font_size);
    void get_osd_range(QStringList format_range, QStringList pos_range, QStringList font_size_range);

private:
    Ui::OSDSettingDialog *ui;
    QStringList fFormat_range;
    QStringList fPos_range;
    QStringList fFont_size_range;

signals:
    void sendOSDSetting(QString enable, QString format, QString pos, QString font_size);
};

#endif // OSDSETTINGDIALOG_H
