#include "basicinformationdialog.h"
#include "ui_basicinformationdialog.h"

BasicInformationDialog::BasicInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicInformationDialog)
{
    ui->setupUi(this);
}

void BasicInformationDialog::changeEvent(QEvent *e)
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

BasicInformationDialog::~BasicInformationDialog()
{
    delete ui;
}

void BasicInformationDialog::get_system_base(QString hw_ver, QString sw_ver, QString build_time, QString mac, QString mac_wireless)
{
    ui->hw_ver_label->setText(hw_ver);
    ui->sw_ver_label->setText(sw_ver);
    ui->Build_time_label->setText(build_time);
    ui->mac_label->setText(mac);
    ui->mac_wireless_label->setText(mac_wireless);
}
