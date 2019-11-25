#include "ntpsettingdialog.h"
#include "ui_ntpsettingdialog.h"

NTPSettingDialog::NTPSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NTPSettingDialog)
{
    ui->setupUi(this);
}

void NTPSettingDialog::changeEvent(QEvent *e)
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

NTPSettingDialog::~NTPSettingDialog()
{
    delete ui;
}

void NTPSettingDialog::on_ok_btn_clicked()
{
    QString enable;
    if(ui->enable_combobox->currentIndex() == 0)
    {
        enable = "true";
    }else{
        enable = "false";
    }
//    int time;
//    if(ui->interval_combobox->currentIndex() == 0)
//    {
//        time = 3600;
//    }else if(ui->interval_combobox->currentIndex() == 1)
//    {
//        time = 86400;
//    }else if(ui->interval_combobox->currentIndex() == 2)
//    {
//        time = 604800;
//    }else if(ui->interval_combobox->currentIndex() == 3)
//    {
//        time = 2592000;
//    }else{
//        time = 5184000;
//    }
    emit sendNTPSetting(enable,ui->server_combobox->currentText(),ui->port_spinbox->value(),ui->spinBox->value());
    this->close();
}

void NTPSettingDialog::on_cancel_btn_clicked()
{
    this->close();
}

void NTPSettingDialog::get_timeRange(int min, int max)
{
    ui->spinBox->setMinimum(min);
    ui->spinBox->setMaximum(max);
    ui->label_5->setText("("+QString::number(min)+"-"+QString::number(max)+")");
}

void NTPSettingDialog::get_NTP(bool enable, QString server, int port, int interval)
{
    if(enable)
    {
        ui->enable_combobox->setCurrentIndex(0);
    }else{
        ui->enable_combobox->setCurrentIndex(1);
    }
    ui->server_combobox->setCurrentText(server);
    ui->port_spinbox->setValue(port);
    ui->spinBox->setValue(interval);
    int month = (interval)/(24*3600*30);
    int day = (interval)%(24*3600*30)/(24*3600);
    int hour = (interval)%(24*3600*30)%(24*3600)/3600;
    int minutes = (interval)%(24*3600*30)%(24*3600)%3600/60;
    int seconds = (interval)%(24*3600*30)%(24*3600)%3600%60;
    ui->interval_label->setText(QString::number(month)+tr("month")+QString::number(day)+tr("day")+QString::number(hour)+tr("hour")+QString::number(minutes)+tr("minutes")+QString::number(seconds)+tr("second"));
}

void NTPSettingDialog::on_spinBox_valueChanged(int arg1)
{
    int month = (arg1)/(24*3600*30);
    int day = (arg1)%(24*3600*30)/(24*3600);
    int hour = (arg1)%(24*3600*30)%(24*3600)/3600;
    int minutes = (arg1)%(24*3600*30)%(24*3600)%3600/60;
    int seconds = (arg1)%(24*3600*30)%(24*3600)%3600%60;
    ui->interval_label->setText(QString::number(month)+tr("month")+QString::number(day)+tr("day")+QString::number(hour)+tr("hour")+QString::number(minutes)+tr("minutes")+QString::number(seconds)+tr("second"));
}

//void NTPSettingDialog::on_sync_btn_clicked()
//{
//    emit sync(ui->server_combobox->currentText(), ui->port_spinbox->value());
//}
