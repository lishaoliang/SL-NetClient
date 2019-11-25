#include "netsettingdialog.h"
#include "ui_netsettingdialog.h"

NetSettingDialog::NetSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetSettingDialog)
{
    ui->setupUi(this);
}

void NetSettingDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NetSettingDialog::setCurrentPage(int i)
{
    ui->tabWidget->setCurrentIndex(i);
}

NetSettingDialog::~NetSettingDialog()
{
    delete ui;
}

void NetSettingDialog::get_ipv4(bool dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2)
{
    if(dhcp)
    {
        ui->dhcp_combobox->setCurrentIndex(0);
    }else{
        ui->dhcp_combobox->setCurrentIndex(1);
    }
    ui->IP_lineEdit->setText(ip);
    ui->gateway_lineEdit->setText(gateway);
    ui->netmask_lineEdit->setText(netmask);
    ui->dns1_lineEdit->setText(dns1);
    ui->dns2_lineEdit->setText(dns2);
}

void NetSettingDialog::get_wireless_ipv4(bool dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2)
{
    if(dhcp)
    {
        ui->wireless_dhcp_combobox_2->setCurrentIndex(0);
    }else{
        ui->wireless_dhcp_combobox_2->setCurrentIndex(1);
    }
    ui->wireless_IP_lineEdit_2->setText(ip);
    ui->wireless_gateway_lineEdit_2->setText(gateway);
    ui->wireless_netmask_lineEdit_2->setText(netmask);
    ui->wireless_dns1_lineEdit_2->setText(dns1);
    ui->wireless_dns2_lineEdit_2->setText(dns2);
}

void NetSettingDialog::get_wireless(QString type, QString net, QString ssid, QString passwd, QString enc)
{
    if(type == "ap")
    {
        ui->type_combobox_2->setCurrentIndex(0);
        ui->net_combobox_2->setEnabled(true);
        ui->enc_combobox_2->setEnabled(true);
    }else{
        ui->type_combobox_2->setCurrentIndex(1);
        ui->net_combobox_2->setEnabled(false);
        ui->enc_combobox_2->setEnabled(false);
    }
    if(net == "5g")
    {
        ui->net_combobox_2->setCurrentIndex(1);
    }else{
        ui->net_combobox_2->setCurrentIndex(0);
    }
    ui->ssid_lineEdit_2->setText(ssid);
    ui->passwd_lineEdit_2->setText(passwd);
    if(enc == "wpa/wpa2 psk")
    {
        ui->enc_combobox_2->setCurrentIndex(0);
    }else{
        if(enc == "wpa2 psk")
        {
            ui->enc_combobox_2->setCurrentIndex(1);
        }else{
            if(enc == "psk")
            {
                ui->enc_combobox_2->setCurrentIndex(2);
            }else{
                ui->enc_combobox_2->setCurrentIndex(3);
            }
        }
    }
}

void NetSettingDialog::on_ipv4_accept_clicked()
{
    if(ui->dhcp_combobox->currentIndex() == 0)
    {
        emit send_ipv4("true", ui->IP_lineEdit->text(), ui->gateway_lineEdit->text(), ui->netmask_lineEdit->text(), ui->dns1_lineEdit->text(), ui->dns2_lineEdit->text());
    }else{
        emit send_ipv4("false", ui->IP_lineEdit->text(), ui->gateway_lineEdit->text(), ui->netmask_lineEdit->text(), ui->dns1_lineEdit->text(), ui->dns2_lineEdit->text());
    }
    this->close();
}

void NetSettingDialog::on_ipv4_reject_clicked()
{
    this->close();
}

void NetSettingDialog::on_wireless_dhcp_combobox_2_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->net_combobox_2->setEnabled(false);
        ui->enc_combobox_2->setEnabled(false);
    }else{
        ui->net_combobox_2->setEnabled(true);
        ui->enc_combobox_2->setEnabled(true);
    }
}

void NetSettingDialog::on_wireless_ipv4_accept_2_clicked()
{
    if(ui->wireless_dhcp_combobox_2->currentIndex() == 0)
    {
        emit send_wireless_ipv4("true", ui->wireless_IP_lineEdit_2->text(), ui->wireless_gateway_lineEdit_2->text(), ui->wireless_netmask_lineEdit_2->text(), ui->wireless_dns1_lineEdit_2->text(), ui->wireless_dns2_lineEdit_2->text());
    }else{
        emit send_wireless_ipv4("false", ui->wireless_IP_lineEdit_2->text(), ui->wireless_gateway_lineEdit_2->text(), ui->wireless_netmask_lineEdit_2->text(), ui->wireless_dns1_lineEdit_2->text(), ui->wireless_dns2_lineEdit_2->text());
    }
    this->close();
}

void NetSettingDialog::on_wireless_ipv4_reject_2_clicked()
{
    this->close();
}

void NetSettingDialog::on_wireless_accept_2_clicked()
{
    if(ui->type_combobox_2->currentIndex() == 0)
    {
        type = "ap";
    }else{
        type = "sta";
    }
    if(ui->net_combobox_2->currentIndex() == 0)
    {
        net = "2.4g";
    }else{
        net = "5g";
    }
    if(ui->enc_combobox_2->currentIndex() == 0)
    {
        enc = "wpa/wpa2 psk";
    }else{
        if(ui->enc_combobox_2->currentIndex() == 1)
        {
            enc = "wpa2 psk";
        }else{
            if(ui->enc_combobox_2->currentIndex() == 2)
            {
                enc = "psk";
            }else{
                enc = "none";
            }
        }
    }
    emit send_wireless(type, net, ui->ssid_lineEdit_2->text(), ui->passwd_lineEdit_2->text(), enc);
    this->close();
}

void NetSettingDialog::on_wireless_reject_2_clicked()
{
    this->close();
}

void NetSettingDialog::on_type_combobox_2_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->net_combobox_2->setEnabled(true);
        ui->enc_combobox_2->setEnabled(true);
    }else{
        ui->net_combobox_2->setEnabled(false);
        ui->enc_combobox_2->setEnabled(false);
    }
}
