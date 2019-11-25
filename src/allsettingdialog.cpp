#include "allsettingdialog.h"
#include "ui_allsettingdialog.h"
#include <QDebug>
AllSettingDialog::AllSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllSettingDialog)
{
    ui->setupUi(this);
    if(ui->awb_combobox->currentIndex() == 0)
    {
        ui->b_slider->setEnabled(false);
        ui->gb_slider->setEnabled(false);
        ui->gr_slider->setEnabled(false);
        ui->r_slider->setEnabled(false);
    }else{
        ui->b_slider->setEnabled(true);
        ui->gb_slider->setEnabled(true);
        ui->gr_slider->setEnabled(true);
        ui->r_slider->setEnabled(true);
    }
}

void AllSettingDialog::changeEvent(QEvent *e)
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

QString AllSettingDialog::fmt()
{
    return ui->fmt->currentText();
}

QString AllSettingDialog::rc_mode()
{
//    return ui->rc_mode->currentText();
    switch (ui->rc_mode->currentIndex()){
    case 0:
        return "cbr";
    case 1:
        return "vbr";
    }
}

QString AllSettingDialog::wh()
{
    return ui->wh->currentText();
}

QString AllSettingDialog::quality()
{
    switch (ui->quality->currentIndex()){
    case 0:
        return "highest";
        break;
    case 1:
        return "higher";
        break;
    case 2:
        return "high";
        break;
    case 3:
        return "middle";
        break;
    case 4:
        return "low";
        break;
    case 5:
        return "lower";
        break;
    case 6:
        return "lowest";
        break;
    }
//    return ui->quality->currentText();
}

int AllSettingDialog::frame_rate()
{
    return ui->frame_rate->value();
}

int AllSettingDialog::bitrate()
{
    return ui->bitrate->value();
}

int AllSettingDialog::i_interval()
{
    return ui->i_interval->value();
}

void AllSettingDialog::translate()
{
    ui->retranslateUi(this);
}

void AllSettingDialog::setCurrentPage(int i)
{
    ui->tabWidget->setCurrentIndex(i);
}

AllSettingDialog::~AllSettingDialog()
{
    delete ui;
}

void AllSettingDialog::on_bright_slider_valueChanged(int value)
{
    ui->bright_label->setText(QString::number(value));
    emit send_bcsh(value,ui->contrast_slider->value(),ui->saturation_slider->value(),ui->hue_slider->value());
}

void AllSettingDialog::on_contrast_slider_valueChanged(int value)
{
    ui->contrast_label->setText(QString::number(value));
    emit send_bcsh(ui->bright_slider->value(),value,ui->saturation_slider->value(),ui->hue_slider->value());
}

void AllSettingDialog::on_saturation_slider_valueChanged(int value)
{
    ui->saturation_label->setText(QString::number(value));
    emit send_bcsh(ui->bright_slider->value(),ui->contrast_slider->value(),value,ui->hue_slider->value());
}

void AllSettingDialog::on_hue_slider_valueChanged(int value)
{
    ui->hue_label->setText(QString::number(value));
    emit send_bcsh(ui->bright_slider->value(),ui->contrast_slider->value(),ui->saturation_slider->value(),value);
}

void AllSettingDialog::on_mirror_checkbox_clicked()
{
    emit send_flip_mirror(ui->flip_checkbox->isChecked(),ui->mirror_checkbox->isChecked());
}

void AllSettingDialog::on_flip_checkbox_clicked()
{
    emit send_flip_mirror(ui->flip_checkbox->isChecked(),ui->mirror_checkbox->isChecked());
}

void AllSettingDialog::on_exposure_spinbox_valueChanged(int arg1)
{
    emit send_exposure(arg1);
}

void AllSettingDialog::get_image_exposure(int exposure)
{
    ui->exposure_spinbox->setValue(exposure);
}

void AllSettingDialog::get_image_exposure_range(int min, int max)
{
    exposure_min = min;
    exposure_max = max;
    ui->exposure_spinbox->setMinimum(min);
    ui->exposure_spinbox->setMaximum(max);
}

void AllSettingDialog::get_image_rotate(int rotate)
{
//    if(rotate == 180)
//    {
//        ui->rotate_comboBox->setCurrentIndex(1);
//    }else{
//        ui->rotate_comboBox->setCurrentIndex(0);
//    }
    for(int i = 0; i< frotate_range.size(); i++)
    {
        if(QString::number(rotate) == frotate_range.at(i))
        {
            ui->rotate_comboBox->setCurrentIndex(i);
            break;
        }
    }
}

void AllSettingDialog::get_image_rotate_range(QStringList rotate_range)
{
    frotate_range.clear();
    ui->rotate_comboBox->clear();
    frotate_range = rotate_range;
    for(int i = 0; i < rotate_range.size(); i++)
    {
        ui->rotate_comboBox->addItem(rotate_range.at(i));
    }
}

void AllSettingDialog::get_image_mirror_flip(bool mirror, bool flip)
{
    ui->mirror_checkbox->setChecked(mirror);
    ui->flip_checkbox->setChecked(flip);
}

void AllSettingDialog::get_image(int bright, int saturation, int contrast, int hue)
{
    ui->bright_slider->setValue(bright);
    ui->saturation_slider->setValue(saturation);
    ui->contrast_slider->setValue(contrast);
    ui->hue_slider->setValue(hue);
    ui->bright_label->setText(QString::number(bright));
    ui->saturation_label->setText(QString::number(saturation));
    ui->contrast_label->setText(QString::number(contrast));
    ui->hue_label->setText(QString::number(hue));
}

void AllSettingDialog::get_image_awb(QString awb, int b, int gb, int gr, int r, int b_min, int b_max, int gb_min, int gb_max, int gr_min, int gr_max, int r_min, int r_max)
{
//    ui->awb_combobox->setCurrentText(awb);
    if(awb == "auto")
    {
        ui->awb_combobox->setCurrentIndex(0);
    }else{
        ui->awb_combobox->setCurrentIndex(1);
    }
    fb_min = b_min;
    fb_max = b_max;
    fgb_min = gb_min;
    fgb_max = gb_max;
    fgr_min = gr_min;
    fgr_max = gr_max;
    fr_min = r_min;
    fr_max = r_max;
//    qDebug()<< b_min <<b_max <<gb_min<< gb_max <<gr_min<< gr_max <<r_min<< r_max;
    ui->b_slider->setMaximum(fb_max);
    ui->b_slider->setMinimum(fb_min);
    ui->gb_slider->setMaximum(fgb_max);
    ui->gb_slider->setMinimum(fgb_min);
    ui->gr_slider->setMaximum(fgr_max);
    ui->gr_slider->setMinimum(fgr_min);
    ui->r_slider->setMaximum(fr_max);
    ui->r_slider->setMinimum(fr_min);
    ui->b_slider->setValue(b);
    ui->gb_slider->setValue(gb);
    ui->gr_slider->setValue(gr);
    ui->r_slider->setValue(r);
    ui->b_label->setText(QString::number(b));
    ui->gb_label->setText(QString::number(gb));
    ui->gr_label->setText(QString::number(gr));
    ui->r_label->setText(QString::number(r));
}

void AllSettingDialog::get_image_isp(int b, int gb, int gr, int r)
{
    ui->b_slider->setValue(b);
    ui->gb_slider->setValue(gb);
    ui->gr_slider->setValue(gr);
    ui->r_slider->setValue(r);
    ui->b_label->setText(QString::number(b));
    ui->gb_label->setText(QString::number(gb));
    ui->gr_label->setText(QString::number(gr));
    ui->r_label->setText(QString::number(r));
}

void AllSettingDialog::get_stream_pic_range(QStringList wh, QStringList quality, int min_interval, int max_interval)
{
    fpic_wh_range.clear();
    fpic_quality_range.clear();
    fpic_wh_range = wh;
    fpic_quality_range = quality;
    ui->pic_quality_combobox->clear();
    qDebug()<<quality;
    for(int i = 0; i < quality.size(); i++)
    {
        ui->pic_quality_combobox->addItem(quality.at(i));
    }
    ui->pic_wh_combobox->clear();
    for(int i = 0; i < wh.size(); i++)
    {
        ui->pic_wh_combobox->addItem(wh.at(i));
    }
    ui->pic_interval_ms_spinbox->setMinimum(min_interval);
    ui->pic_interval_ms_spinbox->setMaximum(max_interval);
    ui->pic_interval_ms_spinbox->setToolTip(QString::number(min_interval)+"-"+QString::number(max_interval));
}

void AllSettingDialog::get_stream_pic(QString fmt, QString wh, QString quality, int interval_ms)
{
    if(fmt == "jpeg")
    {
        ui->pic_fmt_combobox->setCurrentIndex(0);
    }
    for(int i = 0; i< fpic_wh_range.size(); i++)
    {
        if(wh == fpic_wh_range.at(i))
        {
            ui->pic_wh_combobox->setCurrentIndex(i);
            break;
        }
    }
    for(int i = 0; i< fpic_quality_range.size(); i++)
    {
        if(quality == fpic_quality_range.at(i))
        {
            ui->pic_quality_combobox->setCurrentIndex(i);
            break;
        }
    }
    ui->pic_interval_ms_spinbox->setValue(interval_ms);
}

void AllSettingDialog::on_rotate_comboBox_currentIndexChanged(const QString &arg1)
{
    emit send_rotate(arg1.toInt());
}
void AllSettingDialog::on_Image_close_btn_clicked()
{
    this->close();
}

void AllSettingDialog::on_b_slider_valueChanged(int value)
{
    ui->b_label->setText(QString::number(value));
    if(ui->awb_combobox->currentIndex() == 0)
    {
        emit send_awb(QString("auto"),value,ui->gb_slider->value(),ui->gr_slider->value(),ui->r_slider->value());
    }else{
        emit send_awb(QString("manual"),value,ui->gb_slider->value(),ui->gr_slider->value(),ui->r_slider->value());
    }
}

void AllSettingDialog::on_gb_slider_valueChanged(int value)
{
    ui->gb_label->setText(QString::number(value));
    if(ui->awb_combobox->currentIndex() == 0)
    {
        emit send_awb(QString("auto"),ui->b_slider->value(),value,ui->gr_slider->value(),ui->r_slider->value());
    }else{
        emit send_awb(QString("manual"),ui->b_slider->value(),value,ui->gr_slider->value(),ui->r_slider->value());
    }
}

void AllSettingDialog::on_gr_slider_valueChanged(int value)
{
    ui->gr_label->setText(QString::number(value));
    if(ui->awb_combobox->currentIndex() == 0)
    {
        emit send_awb(QString("auto"),ui->b_slider->value(),ui->gb_slider->value(),value,ui->r_slider->value());
    }else{
        emit send_awb(QString("manual"),ui->b_slider->value(),ui->gb_slider->value(),value,ui->r_slider->value());
    }
}

void AllSettingDialog::on_r_slider_valueChanged(int value)
{
    ui->r_label->setText(QString::number(value));
    if(ui->awb_combobox->currentIndex() == 0)
    {
        emit send_awb(QString("auto"),ui->b_slider->value(),ui->gb_slider->value(),ui->gr_slider->value(),value);
    }else{
        emit send_awb(QString("manual"),ui->b_slider->value(),ui->gb_slider->value(),ui->gr_slider->value(),value);
    }
}

void AllSettingDialog::on_awb_combobox_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->b_slider->setEnabled(false);
        ui->gb_slider->setEnabled(false);
        ui->gr_slider->setEnabled(false);
        ui->r_slider->setEnabled(false);
        emit send_awb(QString("auto"),ui->b_slider->value(),ui->gb_slider->value(),ui->gr_slider->value(),ui->r_slider->value());
    }else{
        ui->b_slider->setEnabled(true);
        ui->gb_slider->setEnabled(true);
        ui->gr_slider->setEnabled(true);
        ui->r_slider->setEnabled(true);
        emit get_isp();

        emit send_awb(QString("manual"),ui->b_slider->value(),ui->gb_slider->value(),ui->gr_slider->value(),ui->r_slider->value());
    }
}

void AllSettingDialog::get_stream_wh(QStringList wh_range, QStringList quality_range)
{
    fwh_range.clear();
    fquality_range.clear();
    fwh_range = wh_range;
    fquality_range = quality_range;
    ui->quality->clear();
    for(int i = 0; i< quality_range.size(); i++)
    {
        ui->quality->addItem(quality_range.at(i));
    }
    ui->wh->clear();
    for(int i = 0; i< wh_range.size(); i++)
    {
        ui->wh->addItem(wh_range.at(i));
    }
}

void AllSettingDialog::get_stream_range(int min_frame, int max_frame, int min_bitrate, int max_bitrate, int min_interval, int max_interval)
{
    ui->frame_rate->setMinimum(min_frame);
    ui->frame_rate->setMaximum(max_frame);
    ui->bitrate->setMinimum(min_bitrate);
    ui->bitrate->setMaximum(max_bitrate);
    ui->i_interval->setMinimum(min_interval);
    ui->i_interval->setMaximum(max_interval);
}

void AllSettingDialog::setStreamDialog(QString fmt, QString rc_mode, QString wh, QString quality, int frame_rate, int bitrate, int i_interval)
{
    ui->fmt->setCurrentText(fmt);
    ui->rc_mode->setCurrentText(rc_mode);
    ui->wh->setCurrentText(wh);
    for(int i = 0; i< fwh_range.size(); i++)
    {
        if(wh == fwh_range.at(i))
        {
            ui->wh->setCurrentIndex(i);
            break;
        }
    }
    for(int i = 0; i< fquality_range.size(); i++)
    {
        if(quality == fquality_range.at(i))
        {
            ui->quality->setCurrentIndex(i);
            break;
        }
    }
//    if(quality == "highest")
//    {
//        ui->quality->setCurrentIndex(0);
//    }else{
//        if(quality == "higher"){
//            ui->quality->setCurrentIndex(1);
//        }else{
//            if(quality == "high"){
//                ui->quality->setCurrentIndex(2);
//            }else{
//                if(quality == "middle"){
//                    ui->quality->setCurrentIndex(3);
//                }else{
//                    if(quality == "low"){
//                        ui->quality->setCurrentIndex(4);
//                    }else{
//                        if(quality == "lower"){
//                            ui->quality->setCurrentIndex(5);
//                        }else{
//                            ui->quality->setCurrentIndex(6);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    ui->quality->setCurrentText(quality);
    ui->frame_rate->setValue(frame_rate);
    ui->bitrate->setValue(bitrate);
    ui->i_interval->setValue(i_interval);
}

void AllSettingDialog::on_pic_stream_accept_clicked()
{
    if(ui->pic_fmt_combobox->currentIndex() == 0)
    {
        pic_fmt = "jpeg";
    }
//    if(ui->pic_wh_combobox->currentIndex() == 0)
//    {
//        pic_wh = "2592*1944";
//    }else{
//        if(ui->pic_wh_combobox->currentIndex() == 1)
//        {
//            pic_wh = "2304*1728";
//        }else{
//            pic_wh = "1600*1200";
//        }
//    }
    pic_wh = ui->pic_wh_combobox->currentText();
    switch (ui->pic_quality_combobox->currentIndex()){
    case 0:
        pic_quality = "highest";
        break;
    case 1:
        pic_quality = "higher";
        break;
    case 2:
        pic_quality = "high";
        break;
    case 3:
        pic_quality = "middle";
        break;
    case 4:
        pic_quality = "low";
        break;
    case 5:
        pic_quality = "lower";
        break;
    case 6:
        pic_quality = "lowest";
        break;
    }
    emit send_stream_pic(pic_fmt,pic_wh,pic_quality,ui->pic_interval_ms_spinbox->value());
    this->close();
}

void AllSettingDialog::on_pic_stream_reject_clicked()
{
    this->close();
}

void AllSettingDialog::on_stream_setting_accept_clicked()
{
    emit setVedio();
}

void AllSettingDialog::on_stream_setting_reject_clicked()
{
    this->close();
}
