#include "osdsettingdialog.h"
#include "ui_osdsettingdialog.h"
#include <QDebug>
OSDSettingDialog::OSDSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OSDSettingDialog)
{
    ui->setupUi(this);
}

void OSDSettingDialog::changeEvent(QEvent *e)
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

OSDSettingDialog::~OSDSettingDialog()
{
    delete ui;
}

void OSDSettingDialog::on_ok_btn_clicked()
{
    QString pos;
    QString font;
    switch (ui->pos_combobox->currentIndex())
    {
    case 0:
        pos = "right,top";
        break;
    case 1:
        pos = "right,center";
        break;
    case 2:
        pos = "right,bottom";
        break;
    case 3:
        pos = "left,top";
        break;
    case 4:
        pos = "left,center";
        break;
    case 5:
        pos = "left,bottom";
        break;
    case 6:
        pos = "center,top";
        break;
    case 7:
        pos = "center,center";
        break;
    case 8:
        pos = "center,bottom";
        break;
    }
    switch (ui->font_combobox->currentIndex())
    {
    case 0:
        font = "smallest";
        break;
    case 1:
        font = "smaller";
        break;
    case 2:
        font = "small";
        break;
    case 3:
        font = "middle";
        break;
    case 4:
        font = "larg";
        break;
    case 5:
        font = "larger";
        break;
    case 6:
        font = "largest";
        break;
    }
    QString enable;
    if(ui->enable_combobox->currentIndex() == 0)
    {
        enable = "true";
    }else{
        enable = "false";
    }
    emit sendOSDSetting(enable,ui->format_combobox->currentText(),pos,font);
    this->close();
}

void OSDSettingDialog::get_OSD(bool enable, QString format, QString pos, QString font_size)
{
    if(enable)
    {
        ui->enable_combobox->setCurrentIndex(0);
    }else{
        ui->enable_combobox->setCurrentIndex(1);
    }
    for(int i = 0; i< fFormat_range.size(); i++)
    {
//        qDebug()<<format<<"range:"<<fFormat_range.at(i);
        if(format == fFormat_range.at(i))
        {
            ui->format_combobox->setCurrentIndex(i);
            break;
        }
    }
    for(int i = 0; i< fPos_range.size(); i++)
    {
        if(pos == fPos_range.at(i))
        {
            ui->pos_combobox->setCurrentIndex(i);
            break;
        }
    }
    for(int i = 0; i< fFont_size_range.size(); i++)
    {
        if(font_size == fFont_size_range.at(i))
        {
            ui->font_combobox->setCurrentIndex(i);
            break;
        }
    }
}

void OSDSettingDialog::get_osd_range(QStringList format_range, QStringList pos_range, QStringList font_size_range)
{
    fFormat_range.clear();
    fPos_range.clear();
    fFont_size_range.clear();
    ui->format_combobox->clear();
    ui->font_combobox->clear();
    ui->pos_combobox->clear();
    fFormat_range = format_range;
    fPos_range = pos_range;
    fFont_size_range = font_size_range;
    for(int i = 0; i< format_range.size(); i++)
    {
        ui->format_combobox->addItem(format_range.at(i));
    }
    for(int i = 0; i< pos_range.size(); i++)
    {
        ui->pos_combobox->addItem(pos_range.at(i));
    }
    for(int i = 0; i< font_size_range.size(); i++)
    {
        ui->font_combobox->addItem(font_size_range.at(i));
    }
//    ui->retranslateUi(this);
}

void OSDSettingDialog::on_cancel_btn_clicked()
{
    this->close();
}
