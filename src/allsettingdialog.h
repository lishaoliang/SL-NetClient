#ifndef ALLSETTINGDIALOG_H
#define ALLSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class AllSettingDialog;
}

class AllSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AllSettingDialog(QWidget *parent = 0);
    void changeEvent(QEvent *e);
    QString fmt();
    QString rc_mode();
    QString wh();
    QString quality();
    QString type;
    QString net;
    QString enc;

    QString pic_fmt;
    QString pic_wh;
    QString pic_quality;
    QString pic_interval_ms;
    int frame_rate();
    int bitrate();
    int i_interval();
    void translate();
    void setCurrentPage(int i);
    ~AllSettingDialog();

private slots:
    void on_mirror_checkbox_clicked();
    void on_flip_checkbox_clicked();
    void on_rotate_comboBox_currentIndexChanged(const QString &arg1);

    void on_exposure_spinbox_valueChanged(int arg1);
    void on_Image_close_btn_clicked();

    void get_image_exposure(int exposure);
    void get_image_exposure_range(int min, int max);
    void get_image_rotate(int rotate);
    void get_image_rotate_range(QStringList rotate_range);
    void get_image_mirror_flip(bool mirror, bool flip);
    void get_image(int bright, int saturation, int contrast, int hue);
    void get_image_awb(QString awb, int b, int gb, int gr, int r, int b_min, int b_max, int gb_min, int gb_max, int gr_min, int gr_max, int r_min, int r_max);
    void get_image_isp(int b, int gb, int gr, int r);
    void get_stream_pic_range(QStringList wh, QStringList quality, int min_interval, int max_interval);
    void get_stream_pic(QString fmt, QString wh, QString quality, int interval_ms);

    void on_bright_slider_valueChanged(int value);
    void on_contrast_slider_valueChanged(int value);
    void on_saturation_slider_valueChanged(int value);
    void on_hue_slider_valueChanged(int value);

    void on_b_slider_valueChanged(int value);
    void on_gb_slider_valueChanged(int value);
    void on_gr_slider_valueChanged(int value);
    void on_r_slider_valueChanged(int value);
    void on_awb_combobox_currentIndexChanged(int index);

    void get_stream_wh(QStringList wh_range, QStringList quality_range);
    void get_stream_range(int min_frame, int max_frame, int min_bitrate, int max_bitrate, int min_interval, int max_interval);
    void setStreamDialog(QString fmt, QString rc_mode, QString wh, QString quality, int frame_rate, int bitrate, int i_interval);
    void on_pic_stream_accept_clicked();
    void on_pic_stream_reject_clicked();
    void on_stream_setting_accept_clicked();
    void on_stream_setting_reject_clicked();

private:
    Ui::AllSettingDialog *ui;
    int fb_min;
    int fb_max;
    int fgb_min;
    int fgb_max;
    int fgr_min;
    int fgr_max;
    int fr_min;
    int fr_max;
    int exposure_min;
    int exposure_max;
    QStringList fwh_range;
    QStringList fquality_range;
    QStringList fpic_wh_range;
    QStringList fpic_quality_range;
    QStringList frotate_range;
signals:
    void send_flip_mirror(bool flip, bool mirror);
    void send_rotate(int rotate);
    void send_exposure(int exposure);
    void send_awb(QString awb, int b, int gb, int gr ,int r);
    void send_bcsh(int bright, int contrast, int saturation, int hue);
    void send_stream_pic(QString fmt, QString wh, QString quality, int interval_ms);
    void setVedio();
    void get_isp();
};

#endif // ALLSETTINGDIALOG_H
