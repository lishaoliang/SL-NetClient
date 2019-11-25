#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QTranslator>
#include "Globalmessagedialog.h"
#include "mythread.h"
#include "allsettingdialog.h"
#include "netsettingdialog.h"
#include "basicinformationdialog.h"
#include "osdsettingdialog.h"
#include "ntpsettingdialog.h"
#include "upgradethread.h"

extern "C" {
#include "inc/l_sdk.h"
#include "inc/l_sdk_dec.h"
#include "proto/l_nspp.h"
#include "proto/l_net.h"
#include "proto/l_media.h"
#include "proto/l_md_buf.h"
#include "proto/l_type.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int login(QString ip, QString username, QString passwd, QString port);
    QString getErrorString() const;
    QString width;
    QString height;
    void isLogin(bool);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MyThread *workThread;
    UpgradeThread *upThread;
    QPixmap pix;
    GlobalMessageDialog* fMessage;
    AllSettingDialog* allSettingDialog;
    NetSettingDialog* netSettingDialog;
    BasicInformationDialog* basicDialog;
    OSDSettingDialog* osdDialog;
    NTPSettingDialog* ntpDialog;
    QTranslator *m_pTranslator;

    int ret;
    int upgradetime = 0;
    int id = 0;
    int err_code;
    QString m_IP;
    QString m_user;
    QString m_passwd;
    QString m_port;
    QTimer* timer = nullptr;
    QTimer* upgradeTimer = nullptr;
    static int request_stream(int id, int chnn, int idx);
    static int stop_stream(int id, int chnn, int idx);
    static int set_stream(int id, int chnn, int idx, QString fmt, QString rc_mode, QString wh, QString quality, int frame_rate, int bitrate, int i_interval);
    static int set_Image(int id, int chnn, int bright, int contrast, int saturation, int hue);
    int stream(int id ,int chnn, int idx);
    int get_Image(int id, int chnn);
    int get_Image_exposure(int id, int chnn);
    int get_Image_mirror_flip(int id, int chnn);
    int get_Image_awb(int id ,int chnn);
    int get_Image_Rotate(int id, int chnn);
    int get_System_Base(int id, int chnn);
    int get_wireless(int id, int chnn);
    int get_ipv4(int id, int chnn);
    int get_OSD(int id, int chnn);
    int get_NTP(int id, int chnn);
    int get_wireless_ipv4(int id, int chnn);
    int get_stream_pic(int id, int chnn, int idx);
    int get_image_isp(int id, int chnn);

    static int set_Image_Rotate(int id, int chnn, int rotate);
    static int set_Image_flip_mirror(int id, int chnn, QString flip, QString mirror);
    static int set_Image_exposure(int id, int chnn, int exposure);
    static int set_Image_awb(int id, int chnn, QString awb, int b, int gb, int gr, int r);
    static int set_ipv4(int id, QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    static int set_osd(int id, int chnn, QString enable, QString format, QString pos, QString font_size);
    static int set_ntp(int id, QString enable, QString server, int port, int interval);
    static int set_wireless_ipv4(int id, int code, QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    static int set_wireless(int id, QString type, QString net, QString ssid, QString passwd, QString enc);
    static int set_stream_pic(int id , int chnn, int idx, QString fmt, QString wh, QString quality, int interval_ms);
    static int cb_sdk_media(void* p_obj, int protocol, int id, int chnn, int idx, int md_id, l_md_buf_t* p_data);
    static int ntp_sync(int id, QString server, int port);

    void wait();
    void init();
    void startStream();
    void stopStream();
    void setImage();
    void loginOut();

    void getAllData();
    void getNetData();
private slots:
    void getUpgradeMessage();
    void getImage(QPixmap img);
//    void getImage(QImage img);
    void onTimeOut();

//    void start_stream_slot();
//    void stop_stream_slot();
    void stream_setting_slot();
    void image_setting_slot();
    void osd_setting_slot();
    void ntp_setting_slot();
    void system_base_slot();
    void ipv4_slot();
    void wireless_ipv4_slot();
    void wireless_slot();
    void picture_setting_slot();
    void upgrade_slot();
    void close_slot();
//    void on_start_btn_clicked();
//    void on_stop_btn_clicked();
//    void on_stream_btn_clicked();
//    void on_close_btn_clicked();
//    void on_image_btn_clicked();
    void setVedioStream();
    void set_flip_mirror(bool flip, bool mirror);
    void set_rotate(int rotate);
    void set_exposure(int exposure);
    void set_image_awb(QString awb, int b, int gb, int gr, int r);
    void set_image_bcsh(int bright, int contrast, int saturation, int hue);
    void set_IPv4(QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void set_OSD(QString enable, QString format, QString pos, QString font_size);
    void set_NTP(QString enable, QString server, int port, int interval);
    void set_NTP_sync(QString server, int port);
    void set_Wireless_IPv4(QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void set_Wireless(QString type, QString net, QString ssid, QString passwd, QString enc);
    void set_Stream_Pic(QString fmt, QString wh, QString quality, int interval_ms);
    void setChineseLanguage();
    void setEnglishLanguage();
    void lostSignals();
    void get_isp();
signals:
    void send_image_exposure(int exposure);
    void send_image_exposure_range(int min, int max);
    void send_image_rotate(int rotate);
    void send_image_rotate_range(QStringList rotate_range);
    void send_image_mirror_flip(bool mirror,bool flip);
    void send_image(int bright, int saturation, int contrast, int hue);
    void send_stream_range(int min_frame, int max_frame, int min_bitrate, int max_bitrate, int min_interval, int max_interval);
    void send_stream_wh(QStringList wh,QStringList quality);
    void send_stream(QString fmt, QString rc_mode, QString wh, QString quality, int frame_rate, int bitrate, int i_interval);
    void send_image_awb(QString awb, int b, int gb, int gr, int r, int b_min, int b_max, int gb_min, int gb_max, int gr_min, int gr_max, int r_min, int r_max);
    void send_isp(int b, int gb, int gr, int r);
    void send_system_base(QString hw_ver, QString sw_ver, QString build_time, QString mac, QString mac_wireless);
    void send_ipv4(bool dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void send_osd(bool enable, QString format, QString pos, QString font_size);
    void send_osd_range(QStringList format_range, QStringList pos_range, QStringList font_size_range);
    void send_wireless_ipv4(bool dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2);
    void send_wireless(QString type, QString net, QString ssid, QString passwd, QString enc);
    void send_stream_pic(QString fmt, QString wh, QString quality, int interval_ms);
    void send_stream_pic_range(QStringList wh_range, QStringList quality_range, int min_interval, int max_interval);
    void send_ntp_timeRange(int min, int max);
    void send_ntp(bool enable, QString server, int port, int interval);
};

#endif // MAINWINDOW_H
