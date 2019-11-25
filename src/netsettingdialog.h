#ifndef NETSETTINGDIALOG_H
#define NETSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class NetSettingDialog;
}

class NetSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSettingDialog(QWidget *parent = 0);
    void changeEvent(QEvent *e);
    QString type;
    QString net;
    QString enc;
    void setCurrentPage(int i);
    ~NetSettingDialog();

private slots:
    void get_ipv4(bool dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void get_wireless_ipv4(bool dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void get_wireless(QString type, QString net, QString ssid, QString passwd, QString enc);

    void on_ipv4_accept_clicked();
    void on_ipv4_reject_clicked();
    void on_wireless_dhcp_combobox_2_currentIndexChanged(int index);
    void on_wireless_ipv4_accept_2_clicked();
    void on_wireless_ipv4_reject_2_clicked();
    void on_wireless_accept_2_clicked();
    void on_wireless_reject_2_clicked();

    void on_type_combobox_2_currentIndexChanged(int index);

private:
    Ui::NetSettingDialog *ui;

signals:
    void send_ipv4(QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void send_wireless_ipv4(QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void send_wireless(QString type, QString net, QString ssid, QString passwd, QString enc);
};

#endif // NETSETTINGDIALOG_H
