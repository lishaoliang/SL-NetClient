#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QOpenGLWidget>
#ifdef __L_WIN64__
#include <Windows.h>
#include <assert.h>
#include "mythread.h"

#pragma comment(lib,"ws2_32.lib");
static int wsa_startup()
{
    WSADATA wsa;
    if(0 != WSAStartup(MAKEWORD(2,2), &wsa))
    {
        return 1;
    }

    return 0;
}
#else
static int wsa_startup() {return 0;}
#endif

#define T_STREAM_DEC_ID         100

#define T_LOGIN_STR     "{\"ip\":\"%s\",\"port\":%d,\"cmd\":\"login\",\"login\":{\"username\":\"%s\",\"passwd\":\"%s\"}}"
#define T_BASE_STR      "{\"cmd\":\"system\"}"
#define T_REQ_STREAM_STR   "{\"cmd\":\"open_stream\",\"open_stream\":{\"chnn\":%d,\"idx\":%d}}"
#define T_STOP_STREAM_STR   "{\"cmd\":\"close_stream\",\"close_stream\":{\"chnn\":%d,\"idx\":%d}}"
#define T_STREAM   "{\"cmd\":\"stream\",\"stream\":{\"chnn\":%d,\"idx\":%d}}"
#define T_SYNC     "{\"cmd\":\"ntp_sync\",\"ntp_sync\":{\"server\":\"%s\",\"port\":%d}}"

#define T_SET_IMAGE     "{\"cmd\":\"set_image\",\"set_image\":{\"chnn\":%d,\"bright\":%d,\"contrast\":%d,\"saturation\":%d,\"hue\":%d}}"
#define T_SET_STREAM    "{\"cmd\":\"set_stream\",\"set_stream\":{\"chnn\":%d,\"idx\":%d,\"fmt\":\"%s\",\"rc_mode\":\"%s\",\"wh\":\"%s\",\"quality\":\"%s\",\"frame_rate\":%d,\"bitrate\":%d,\"i_interval\":%d}}"
#define T_GET_IMAGE     "{\"cmd\":\"image\",\"image\":{\"chnn\":%d}}"
#define T_GET_ROTATE    "{\"cmd\":\"img_rotate\",\"img_rotate\":{\"chnn\":%d}}"
#define T_GET_IMG_EXPOSURE  "{\"cmd\":\"img_exposure\",\"img_exposure\":{\"chnn\":%d}}"
#define T_GET_IMG_MIRROR_FLIP   "{\"cmd\":\"img_mirror_flip\",\"img_mirror_flip\":{\"chnn\":%d}}"
#define T_GET_IMG_AWB   "{\"cmd\":\"img_awb\",\"img_awb\":{\"chnn\":%d}}"
#define T_GET_WIRELESS  "{\"cmd\":\"wireless\"}"
#define T_GET_IPV4      "{\"cmd\":\"ipv4\"}"
#define T_GET_WIRELESS_IPV4 "{\"cmd\":\"wireless_ipv4\"}"
#define T_GET_STREAM_PIC    "{\"cmd\":\"stream_pic\",\"stream_pic\":{\"chnn\":%d,\"idx\":%d}}"
#define T_GET_OSD       "{\"cmd\":\"osd_timestamp\",\"osd_timestamp\":{\"chnn\":%d}}"
#define T_GET_NTP       "{\"cmd\":\"ntp\"}"
#define T_GET_UPGRADE   "{\"cmd\":\"status_upgrade\",\"ip\":\"%s\",\"port\":%d}"
#define T_GET_ISP       "{\"cmd\":\"info_img_awb\",\"info_img_awb\":{\"chnn\":%d}}"

#define T_SET_IMG_ROTATE    "{\"cmd\":\"set_img_rotate\",\"set_img_rotate\":{\"chnn\":%d,\"rotate\":%d}}"
#define T_SET_IMG_FLIP_MIRROR   "{\"cmd\":\"set_img_mirror_flip\",\"set_img_mirror_flip\":{\"chnn\":%d,\"flip\":%s,\"mirror\":%s}}"
#define T_SET_IMG_EXPOSURE  "{\"cmd\":\"set_img_exposure\",\"set_img_exposure\":{\"chnn\":%d,\"compensation\":%d}}"
#define T_SET_IMG_AWB   "{\"cmd\":\"set_img_awb\",\"set_img_awb\":{\"chnn\":%d,\"awb\":\"%s\",\"b\":%d,\"gb\":%d,\"gr\":%d,\"r\":%d}}"
#define T_SET_IPV4      "{\"cmd\":\"set_ipv4\",\"set_ipv4\":{\"dhcp\":%s,\"ip\":\"%s\",\"gateway\":\"%s\",\"netmask\":\"%s\",\"dns1\":\"%s\",\"dns2\":\"%s\"}}"
#define T_SET_WIRELESS_IPV4     "{\"cmd\":\"set_wireless_ipv4\",\"set_wireless_ipv4\":{\"code\":%d,\"dhcp\":%s,\"ip\":\"%s\",\"gateway\":\"%s\",\"netmask\":\"%s\",\"dns1\":\"%s\",\"dns2\":\"%s\"}}"
#define T_SET_WIRELESS  "{\"cmd\":\"set_wireless\",\"set_wireless\":{\"type\":\"%s\",\"net\":\"%s\",\"ssid\":\"%s\",\"passwd\":\"%s\",\"enc\":\"%s\"}}"
#define T_SET_STREAM_PIC    "{\"cmd\":\"set_stream_pic\",\"set_stream_pic\":{\"chnn\":%d,\"idx\":%d,\"fmt\":\"%s\",\"wh\":\"%s\",\"quality\":\"%s\",\"interval_ms\":%d}}"
#define T_SET_OSD       "{\"cmd\":\"set_osd_timestamp\",\"set_osd_timestamp\":{\"chnn\":%d,\"enable\":%s,\"format\":\"%s\",\"pos\":\"%s\",\"font_size\":\"%s\"}}"
#define T_SET_NTP       "{\"cmd\":\"set_ntp\",\"set_ntp\":{\"enable\":%s,\"server\":\"%s\",\"port\":%d,\"interval\":%d}}"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    setWindowFlags(Qt::FramelessWindowHint);
//    this->showMaximized();
//    ui->stop_btn->setEnabled(false);
    workThread = new MyThread();
    upThread = new UpgradeThread();
    fMessage = new GlobalMessageDialog();
    allSettingDialog = new AllSettingDialog(this);
    netSettingDialog = new NetSettingDialog(this);
    basicDialog = new BasicInformationDialog(this);
    osdDialog = new OSDSettingDialog(this);
    ntpDialog = new NTPSettingDialog(this);
    m_pTranslator = new QTranslator();
    ui->actionChinese->setChecked(true);
    connect(workThread, SIGNAL(sendPix(QPixmap)), this, SLOT(getImage(QPixmap)));
    connect(workThread, SIGNAL(lostSignals()), this, SLOT(lostSignals()));
//    connect(workThread, SIGNAL(sendImage(QImage)), this, SLOT(getImage(QImage)));
    connect(allSettingDialog, SIGNAL(setVedio()), this, SLOT(setVedioStream()));
    connect(allSettingDialog, SIGNAL(send_flip_mirror(bool,bool)), this, SLOT(set_flip_mirror(bool,bool)));
    connect(allSettingDialog, SIGNAL(send_rotate(int)), this, SLOT(set_rotate(int)));
    connect(allSettingDialog, SIGNAL(send_exposure(int)), this, SLOT(set_exposure(int)));
    connect(allSettingDialog, SIGNAL(send_awb(QString,int,int,int,int)), this, SLOT(set_image_awb(QString,int,int,int,int)));
    connect(allSettingDialog, SIGNAL(send_bcsh(int,int,int,int)), this, SLOT(set_image_bcsh(int,int,int,int)));
    connect(allSettingDialog, SIGNAL(send_stream_pic(QString,QString,QString,int)), this, SLOT(set_Stream_Pic(QString,QString,QString,int)));
    connect(allSettingDialog, SIGNAL(get_isp()), this, SLOT(get_isp()));

    connect(netSettingDialog, SIGNAL(send_ipv4(QString,QString,QString,QString,QString,QString)), this, SLOT(set_IPv4(QString,QString,QString,QString,QString,QString)));
    connect(netSettingDialog, SIGNAL(send_wireless_ipv4(QString,QString,QString,QString,QString,QString)), this, SLOT(set_Wireless_IPv4(QString,QString,QString,QString,QString,QString)));
    connect(netSettingDialog, SIGNAL(send_wireless(QString,QString,QString,QString,QString)), this, SLOT(set_Wireless(QString,QString,QString,QString,QString)));

    connect(osdDialog, SIGNAL(sendOSDSetting(QString,QString,QString,QString)), this, SLOT(set_OSD(QString,QString,QString,QString)));
    connect(ntpDialog, SIGNAL(sendNTPSetting(QString,QString,int,int)), this, SLOT(set_NTP(QString,QString,int,int)));
    connect(ntpDialog, SIGNAL(sync(QString,int)), this, SLOT(set_NTP_sync(QString,int)));
    connect(this, SIGNAL(send_stream_range(int,int,int,int,int,int)), allSettingDialog, SLOT(get_stream_range(int,int,int,int,int,int)));
    connect(this, SIGNAL(send_stream(QString,QString,QString,QString,int,int,int)), allSettingDialog, SLOT(setStreamDialog(QString,QString,QString,QString,int,int,int)));
    connect(this, SIGNAL(send_image_exposure(int)), allSettingDialog, SLOT(get_image_exposure(int)));
    connect(this, SIGNAL(send_image_exposure_range(int,int)), allSettingDialog, SLOT(get_image_exposure_range(int,int)));
    connect(this, SIGNAL(send_image_mirror_flip(bool,bool)), allSettingDialog, SLOT(get_image_mirror_flip(bool,bool)));
    connect(this, SIGNAL(send_image(int,int,int,int)), allSettingDialog, SLOT(get_image(int,int,int,int)));
    connect(this, SIGNAL(send_image_awb(QString,int,int,int,int,int,int,int,int,int,int,int,int)), allSettingDialog, SLOT(get_image_awb(QString,int,int,int,int,int,int,int,int,int,int,int,int)));
    connect(this, SIGNAL(send_system_base(QString,QString,QString,QString,QString)), basicDialog, SLOT(get_system_base(QString,QString,QString,QString,QString)));
    connect(this, SIGNAL(send_stream_pic(QString,QString,QString,int)), allSettingDialog, SLOT(get_stream_pic(QString,QString,QString,int)));
    connect(this, SIGNAL(send_stream_pic_range(QStringList,QStringList,int,int)), allSettingDialog, SLOT(get_stream_pic_range(QStringList,QStringList,int,int)));
    connect(this, SIGNAL(send_ipv4(bool,QString,QString,QString,QString,QString)), netSettingDialog, SLOT(get_ipv4(bool,QString,QString,QString,QString,QString)));
    connect(this, SIGNAL(send_wireless_ipv4(bool,QString,QString,QString,QString,QString)), netSettingDialog, SLOT(get_wireless_ipv4(bool,QString,QString,QString,QString,QString)));
    connect(this, SIGNAL(send_wireless(QString,QString,QString,QString,QString)), netSettingDialog, SLOT(get_wireless(QString,QString,QString,QString,QString)));
    connect(this, SIGNAL(send_image_rotate(int)), allSettingDialog, SLOT(get_image_rotate(int)));
    connect(this, SIGNAL(send_image_rotate_range(QStringList)), allSettingDialog, SLOT(get_image_rotate_range(QStringList)));
    connect(this, SIGNAL(send_isp(int,int,int,int)), allSettingDialog, SLOT(get_image_isp(int,int,int,int)));
    connect(this, SIGNAL(send_stream_wh(QStringList,QStringList)), allSettingDialog, SLOT(get_stream_wh(QStringList,QStringList)));
    connect(this, SIGNAL(send_osd_range(QStringList,QStringList,QStringList)), osdDialog, SLOT(get_osd_range(QStringList,QStringList,QStringList)));
    connect(this, SIGNAL(send_osd(bool,QString,QString,QString)), osdDialog, SLOT(get_OSD(bool,QString,QString,QString)));
    connect(this, SIGNAL(send_ntp_timeRange(int,int)), ntpDialog, SLOT(get_timeRange(int,int)));
    connect(this, SIGNAL(send_ntp(bool,QString,int,int)), ntpDialog, SLOT(get_NTP(bool,QString,int,int)));

//    connect(ui->actionstart_stream, SIGNAL(triggered(bool)), this, SLOT(start_stream_slot()));
//    connect(ui->actionstop_stream, SIGNAL(triggered(bool)), this, SLOT(stop_stream_slot()));
    connect(ui->actionstream_setting, SIGNAL(triggered(bool)), this, SLOT(stream_setting_slot()));
    connect(ui->actionimage_setting, SIGNAL(triggered(bool)), this, SLOT(image_setting_slot()));
    connect(ui->actionosd_setting, SIGNAL(triggered(bool)), this, SLOT(osd_setting_slot()));
    connect(ui->actionntp_setting, SIGNAL(triggered(bool)), this, SLOT(ntp_setting_slot()));
    connect(ui->actionsystem_base, SIGNAL(triggered(bool)), this, SLOT(system_base_slot()));
    connect(ui->actionWired_IP_address, SIGNAL(triggered(bool)), this, SLOT(ipv4_slot()));
    connect(ui->actionwireless_IP_address, SIGNAL(triggered(bool)), this, SLOT(wireless_ipv4_slot()));
    connect(ui->actionwireless_setting, SIGNAL(triggered(bool)), this, SLOT(wireless_slot()));
    connect(ui->actionpicture_setting, SIGNAL(triggered(bool)), this, SLOT(picture_setting_slot()));
    connect(ui->actionupgrade, SIGNAL(triggered(bool)), this, SLOT(upgrade_slot()));
    connect(ui->actionclose, SIGNAL(triggered(bool)), this, SLOT(close_slot()));

    connect(ui->actionChinese, SIGNAL(triggered(bool)), this, SLOT(setChineseLanguage()));
    connect(ui->actionEnglish, SIGNAL(triggered(bool)), this, SLOT(setEnglishLanguage()));
}

int MainWindow::login(QString ip, QString username, QString passwd, QString port)
{
//    this->wait();
    wsa_startup();
    ret = l_sdk_init("");
    m_IP = ip;
    m_user = username;
    m_passwd = passwd;
    m_port = port;
//    printf("(%s.%d)sdk init,ret=%d\n", __FILE__, __LINE__, ret);
    // 添加媒体数据监听者
    QString str = "my listener 1";
    char* temp = str.toLatin1().data();
    l_sdk_md_add_listener(temp, cb_sdk_media, NULL);

    // 打开一个解码器
    ret = l_sdk_dec_open(T_STREAM_DEC_ID,  "{\"pix_fmt\":\"RGB888\"}" );
    char req_login[128] = { 0 };
    snprintf(req_login, 124, T_LOGIN_STR, ip.toLatin1().data(), port.toInt(), username.toLatin1().data(), passwd.toLatin1().data());
    id = 0;
    ret = l_sdk_login(&id, req_login);
    if(0 != ret)
    {
        err_code = ret;
        this->isLogin(0 == ret ? true : false);
    }else{
        ret = l_sdk_dec_bind(T_STREAM_DEC_ID, id, 0, 0, 0);
        this->isLogin(0 == ret ? true : false);
    }
    return ret;
}

QString MainWindow::getErrorString() const
{
    switch (err_code) {
    case L_SDK_OK:
        return QString(tr("Normal"));
    case L_SDK_ERR_INIT:
        return QString(tr("The SDK was not initialized, or initialized incorrectly"));
    case L_SDK_ERR_TIME_OUT:
        return QString(tr("Wait for a timeout"));
    case L_SDK_ERR_PARAM:
        return QString(tr("Parameter Error"));
    case L_SDK_ERR_OPEN:
        return QString(tr("Open failed"));
    case L_SDK_ERR_NO_DATA:
        return QString(tr("No data, or no new data refresh"));
    default:
        return QString(tr("unknown"));
    }
}

void MainWindow::isLogin(bool ok)
{
    if(ok) {
        fMessage->accept();
        this->showMaximized();
        startStream();

    }else{
        fMessage->setDialog(this->getErrorString(), false, true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::request_stream(int id, int chnn, int idx)
{
    // 请求码流
    char req_stream[128] = { 0 };
    snprintf(req_stream, 124, T_REQ_STREAM_STR, chnn, idx);

    char* p_res = NULL;
    int ret = l_sdk_request(id, req_stream, &p_res);
//    qDebug()<<"p_res"<<p_res<<"ret:"<<ret;
//    printf("(%s.%d)ret=%d,res=%s\n", __FILE__, __LINE__, ret, (NULL != p_res) ? p_res : "error!");

    if (NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::stop_stream(int id, int chnn, int idx)
{
    char stop_stream[128] = { 0 };
    snprintf(stop_stream, 124, T_STOP_STREAM_STR, chnn, idx);

    char* p_res = NULL;
    int ret = l_sdk_request(id, stop_stream, &p_res);
//    printf("(%s.%d)ret=%d,res=%s\n", __FILE__, __LINE__, ret, (NULL != p_res) ? p_res : "error!");

    if (NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::stream(int id, int chnn, int idx)
{
    char stop_stream[128] = { 0 };
    snprintf(stop_stream, 124, T_STREAM, chnn, idx);

    char* p_res = NULL;
    int ret = l_sdk_request(id, stop_stream, &p_res);
//    qDebug()<<p_res;
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    QString fmt = recJson.value("stream").toObject().value("fmt").toString();
    QString rc_mode = recJson.value("stream").toObject().value("rc_mode").toString();
    QString wh = recJson.value("stream").toObject().value("wh").toString();
    QString tempWidth = wh.split("*").at(0);
    QString tempHeight = wh.split("*").at(1);
    workThread->width = tempWidth.toInt();
    workThread->height = tempHeight.toInt();
    QString quality = recJson.value("stream").toObject().value("quality").toString();
    int frame_rate = recJson.value("stream").toObject().value("frame_rate").toInt();
    int min_frame = recJson.value("stream").toObject().value("range").toObject().value("frame_rate").toObject().value("min").toInt();
    int max_frame = recJson.value("stream").toObject().value("range").toObject().value("frame_rate").toObject().value("max").toInt();
    int bitrate = recJson.value("stream").toObject().value("bitrate").toInt();
    int min_bitrate = recJson.value("stream").toObject().value("range").toObject().value("bitrate").toObject().value("min").toInt();
    int max_bitrate = recJson.value("stream").toObject().value("range").toObject().value("bitrate").toObject().value("max").toInt();
    int i_interval = recJson.value("stream").toObject().value("i_interval").toInt();
    int min_interval = recJson.value("stream").toObject().value("range").toObject().value("i_interval").toObject().value("min").toInt();
    int max_interval = recJson.value("stream").toObject().value("range").toObject().value("i_interval").toObject().value("max").toInt();
    QJsonArray fwh = recJson.value("stream").toObject().value("range").toObject().value("wh").toArray();
    QJsonArray fquality = recJson.value("stream").toObject().value("range").toObject().value("quality").toArray();
//    qDebug()<<fwh;
    QStringList quality_range;
    QStringList wh_range;
    for(int i = 0; i< fwh.size(); i++)
    {
        wh_range.append(fwh.at(i).toString());
    }
    for(int i = 0; i< fquality.size(); i++)
    {
        quality_range.append(fquality.at(i).toString());
    }
//    qDebug()<<wh_range;
    emit send_stream_wh(wh_range,quality_range);
    emit send_stream_range(min_frame,max_frame,min_bitrate,max_bitrate,min_interval,max_interval);
//    qDebug()<<min_frame<<max_frame<<min_bitrate<<max_bitrate<<min_interval<<max_interval;
    emit send_stream(fmt,rc_mode,wh,quality, frame_rate,bitrate,i_interval);
    if (NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_stream(int id, int chnn, int idx, QString fmt, QString rc_mode, QString wh, QString quality, int frame_rate, int bitrate, int i_interval)
{
    char set_stream[256] = { 0 };
    snprintf(set_stream, 252, T_SET_STREAM, chnn, idx, fmt.toLatin1().data(), rc_mode.toLatin1().data(), wh.toLatin1().data(), quality.toLatin1().data(), frame_rate, bitrate, i_interval);
    char* p_res = NULL;
    int ret = l_sdk_request(id, set_stream, &p_res);
//    qDebug()<<"ssssssssssssssss:"<<set_stream;
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    int code = recJson.value("code").toInt();
    if(code == 0)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Setup complete"));
        msgBox.exec();
    }else{
        QMessageBox msgBox;
        msgBox.setText(tr("Setup failed"));
        msgBox.exec();
    }
    if (NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_Image(int id, int chnn, int bright, int contrast, int saturation, int hue)
{
    char setImage_stream[128] = { 0 };
    snprintf(setImage_stream, 124,T_SET_IMAGE, chnn, bright, contrast, saturation, hue);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, setImage_stream, &p_res);
//    qDebug()<<"asdasdasdasdasda:"<<setImage_stream;
//    printf("(%s.%d)ret=%d,res=%s\n", __FILE__, __LINE__, ret, (NULL != p_res) ? p_res : "error!");
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_Image(int id, int chnn)
{
    char setImage_stream[128] = { 0 };
    snprintf(setImage_stream, 124,T_GET_IMAGE, chnn);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, setImage_stream, &p_res);
//    printf("(%s.%d)ret=%d,res=%s\n", __FILE__, __LINE__, ret, (NULL != p_res) ? p_res : "error!");
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    int bright = recJson.value("image").toObject().value("bright").toInt();
    int saturation = recJson.value("image").toObject().value("saturation").toInt();
    int contrast = recJson.value("image").toObject().value("contrast").toInt();
    int hue = recJson.value("image").toObject().value("hue").toInt();
    emit send_image(bright, saturation, contrast, hue);
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_Image_exposure(int id, int chnn)
{
    char getImage_exposure[128] = { 0 };
    snprintf(getImage_exposure, 124,T_GET_IMG_EXPOSURE, chnn);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, getImage_exposure, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    int compensation = recJson.value("img_exposure").toObject().value("compensation").toInt();
    int min = recJson.value("img_exposure").toObject().value("range").toObject().value("compensation").toObject().value("min").toInt();
    int max = recJson.value("img_exposure").toObject().value("range").toObject().value("compensation").toObject().value("max").toInt();
    emit send_image_exposure_range(min, max);
    emit send_image_exposure(compensation);
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_Image_mirror_flip(int id, int chnn)
{
    char getImage_mirror_flip[128] = { 0 };
    snprintf(getImage_mirror_flip, 124,T_GET_IMG_MIRROR_FLIP, chnn);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, getImage_mirror_flip, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    bool mirror = recJson.value("img_mirror_flip").toObject().value("mirror").toBool();
    bool flip = recJson.value("img_mirror_flip").toObject().value("flip").toBool();
    emit send_image_mirror_flip(mirror, flip);
//    printf("(%s.%d)ret=%d,res=%s\n", __FILE__, __LINE__, ret, (NULL != p_res) ? p_res : "error!");
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_Image_awb(int id, int chnn)
{
    char getImage_mirror_awb[128] = { 0 };
    snprintf(getImage_mirror_awb, 124,T_GET_IMG_AWB, chnn);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, getImage_mirror_awb, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    emit send_image_awb(recJson.value("img_awb").toObject().value("awb").toString(),recJson.value("img_awb").toObject().value("b").toInt(),recJson.value("img_awb").toObject().value("gb").toInt(),
                        recJson.value("img_awb").toObject().value("gr").toInt(),recJson.value("img_awb").toObject().value("r").toInt(),
                        recJson.value("img_awb").toObject().value("range").toObject().value("b").toObject().value("min").toInt(),recJson.value("img_awb").toObject().value("range").toObject().value("b").toObject().value("max").toInt(),
                        recJson.value("img_awb").toObject().value("range").toObject().value("gb").toObject().value("min").toInt(),recJson.value("img_awb").toObject().value("range").toObject().value("gb").toObject().value("max").toInt(),
                        recJson.value("img_awb").toObject().value("range").toObject().value("gr").toObject().value("min").toInt(),recJson.value("img_awb").toObject().value("range").toObject().value("gr").toObject().value("max").toInt(),
                        recJson.value("img_awb").toObject().value("range").toObject().value("r").toObject().value("min").toInt(),recJson.value("img_awb").toObject().value("range").toObject().value("r").toObject().value("max").toInt());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_Image_Rotate(int id, int chnn)
{
    char image_rotate[128] = { 0 };
    snprintf(image_rotate, 124, T_GET_ROTATE, chnn);

    char* p_res = NULL;
    int ret = l_sdk_request(id, image_rotate, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    QJsonArray range = recJson.value("img_rotate").toObject().value("range").toObject().value("rotate").toArray();
    QStringList rotate_range;
    for(int i = 0; i< range.size(); i++)
    {
        rotate_range.append(QString::number(range.at(i).toInt()));
    }
    qDebug()<<rotate_range;
    emit send_image_rotate_range(rotate_range);
    emit send_image_rotate(recJson.value("img_rotate").toObject().value("rotate").toInt());

    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_System_Base(int id, int chnn)
{
    char base_str[128] = { 0 };
    snprintf(base_str, 124, T_BASE_STR, chnn);

    char* p_res = NULL;
    int ret = l_sdk_request(id, base_str, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    emit send_system_base(recJson.value("system").toObject().value("hw_ver").toString(), recJson.value("system").toObject().value("sw_ver").toString(), recJson.value("system").toObject().value("build_time").toString(),
                          recJson.value("system").toObject().value("mac").toString(), recJson.value("system").toObject().value("mac_wireless").toString());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_wireless(int id, int chnn)
{
    char wireless_str[128] = { 0 };
    snprintf(wireless_str, 124, T_GET_WIRELESS, chnn);

    char* p_res = NULL;
    int ret = l_sdk_request(id, wireless_str, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    emit send_wireless(recJson.value("wireless").toObject().value("type").toString(), recJson.value("wireless").toObject().value("net").toString(), recJson.value("wireless").toObject().value("ssid").toString(),
                       recJson.value("wireless").toObject().value("passwd").toString(), recJson.value("wireless").toObject().value("enc").toString());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_ipv4(int id, int chnn)
{
    char ipv4_str[128] = { 0 };
    snprintf(ipv4_str, 124, T_GET_IPV4, chnn);

    char* p_res = NULL;
    int ret = l_sdk_request(id, ipv4_str, &p_res);
//    qDebug()<<"aaaaaaaaaaa"<<p_res;
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    emit send_ipv4(recJson.value("ipv4").toObject().value("dhcp").toBool(),recJson.value("ipv4").toObject().value("ip").toString(),recJson.value("ipv4").toObject().value("gateway").toString(),
                   recJson.value("ipv4").toObject().value("netmask").toString(),recJson.value("ipv4").toObject().value("dns1").toString(),recJson.value("ipv4").toObject().value("dns2").toString());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_OSD(int id, int chnn)
{
    char osd_str[128] = { 0 };
    snprintf(osd_str, 124, T_GET_OSD, chnn);

    char* p_res = NULL;
    int ret = l_sdk_request(id, osd_str, &p_res);
//    qDebug()<<"getOSD:"<<p_res;
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    QJsonArray format = recJson.value("osd_timestamp").toObject().value("range").toObject().value("format").toArray();
    QJsonArray pos = recJson.value("osd_timestamp").toObject().value("range").toObject().value("pos").toArray();
    QJsonArray font_size = recJson.value("osd_timestamp").toObject().value("range").toObject().value("font_size").toArray();
    QStringList format_range;
    for(int i = 0; i< format.size(); i++)
    {
        format_range.append(format.at(i).toString());
    }
    QStringList pos_range;
    for(int i = 0; i< pos.size(); i++)
    {
        pos_range.append(pos.at(i).toString());
    }
    QStringList font_size_range;
    for(int i = 0; i< font_size.size(); i++)
    {
        font_size_range.append(font_size.at(i).toString());
    }
    emit send_osd_range(format_range,pos_range,font_size_range);
    emit send_osd(recJson.value("osd_timestamp").toObject().value("enable").toBool(), recJson.value("osd_timestamp").toObject().value("format").toString(),
                  recJson.value("osd_timestamp").toObject().value("pos").toString(), recJson.value("osd_timestamp").toObject().value("font_size").toString());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_NTP(int id, int chnn)
{
    char ntp_str[128] = { 0 };
    snprintf(ntp_str, 124, T_GET_NTP, chnn);

    char* p_res = NULL;
    int ret = l_sdk_request(id, ntp_str, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    emit send_ntp_timeRange(recJson.value("ntp").toObject().value("range").toObject().value("interval").toObject().value("min").toInt(),
                            recJson.value("ntp").toObject().value("range").toObject().value("interval").toObject().value("max").toInt());
    emit send_ntp(recJson.value("ntp").toObject().value("enable").toBool(), recJson.value("ntp").toObject().value("server").toString(),
                  recJson.value("ntp").toObject().value("port").toInt(), recJson.value("ntp").toObject().value("interval").toInt());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_wireless_ipv4(int id, int chnn)
{
    char wireless_ipv4[128] = { 0 };
    snprintf(wireless_ipv4, 124, T_GET_WIRELESS_IPV4, chnn);

    char* p_res = NULL;
    int ret = l_sdk_request(id, wireless_ipv4, &p_res);
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    emit send_wireless_ipv4(recJson.value("wireless_ipv4").toObject().value("dhcp").toBool(),recJson.value("wireless_ipv4").toObject().value("ip").toString(),recJson.value("wireless_ipv4").toObject().value("gateway").toString(),
                   recJson.value("wireless_ipv4").toObject().value("netmask").toString(),recJson.value("wireless_ipv4").toObject().value("dns1").toString(),recJson.value("wireless_ipv4").toObject().value("dns2").toString());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_stream_pic(int id, int chnn, int idx)
{
    char stream_pic[128] = { 0 };
    snprintf(stream_pic, 124, T_GET_STREAM_PIC, chnn, idx);
//    qDebug()<<stream_pic;
    char* p_res = NULL;
    int ret = l_sdk_request(id, stream_pic, &p_res);
//    qDebug()<<p_res;
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    QJsonArray wh = recJson.value("stream_pic").toObject().value("range").toObject().value("wh").toArray();
    QJsonArray quality = recJson.value("stream_pic").toObject().value("range").toObject().value("quality").toArray();
    int min_interval = recJson.value("stream_pic").toObject().value("range").toObject().value("interval_ms").toObject().value("min").toInt();
    int max_interval = recJson.value("stream_pic").toObject().value("range").toObject().value("interval_ms").toObject().value("max").toInt();
    QStringList wh_range;
    QStringList quality_range;
    for(int i = 0; i < quality.size(); i++)
    {
        quality_range.append(quality.at(i).toString());
    }
    for(int i = 0; i < wh.size(); i++)
    {
        wh_range.append(wh.at(i).toString());
    }
    emit send_stream_pic_range(wh_range,quality_range,min_interval,max_interval);
    emit send_stream_pic(recJson.value("stream_pic").toObject().value("fmt").toString(),recJson.value("stream_pic").toObject().value("wh").toString(),
                         recJson.value("stream_pic").toObject().value("quality").toString(),recJson.value("stream_pic").toObject().value("interval_ms").toInt());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::get_image_isp(int id, int chnn)
{
    char isp[128] = { 0 };
    snprintf(isp, 124, T_GET_ISP, chnn);
    char* p_res = NULL;
    int ret = l_sdk_request(id, isp, &p_res);
//    qDebug()<<"isp"<<p_res;
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
    emit send_isp(recJson.value("info_img_awb").toObject().value("b").toInt(),recJson.value("info_img_awb").toObject().value("gb").toInt(),
                  recJson.value("info_img_awb").toObject().value("gr").toInt(),recJson.value("info_img_awb").toObject().value("r").toInt());
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_Image_Rotate(int id, int chnn, int rotate)
{
    char setImage_rotate[128] = { 0 };
    snprintf(setImage_rotate, 124,T_SET_IMG_ROTATE, chnn, rotate);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, setImage_rotate, &p_res);
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_Image_flip_mirror(int id, int chnn, QString flip, QString mirror)
{
    char setImage_mirror[128] = { 0 };
    snprintf(setImage_mirror, 124,T_SET_IMG_FLIP_MIRROR, chnn, flip.toLatin1().data(), mirror.toLatin1().data());

    char* p_res = NULL;
    int ret  = l_sdk_request(id, setImage_mirror, &p_res);
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_Image_exposure(int id, int chnn, int exposure)
{
    char setImage_exposure[128] = { 0 };
    snprintf(setImage_exposure, 124,T_SET_IMG_EXPOSURE, chnn, exposure);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, setImage_exposure, &p_res);
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_Image_awb(int id, int chnn, QString awb, int b, int gb, int gr, int r)
{
    char setImage_awb[128] = { 0 };
    snprintf(setImage_awb, 124,T_SET_IMG_AWB, chnn, awb.toLatin1().data(), b, gb, gr, r);

    char* p_res = NULL;
    int ret  = l_sdk_request(id, setImage_awb, &p_res);
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_ipv4(int id, QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2)
{
    char set_ipv4[256] = { 0 };
    snprintf(set_ipv4, 252, T_SET_IPV4, dhcp.toLatin1().data(), ip.toLatin1().data(), gateway.toLatin1().data(), netmask.toLatin1().data(), dns1.toLatin1().data(), dns2.toLatin1().data());

    char* p_res = NULL;
//    qDebug()<<set_ipv4;
    int ret = l_sdk_request(id, set_ipv4, &p_res);
//    qDebug()<<"sdasdasdads"<<p_res;
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_osd(int id, int chnn, QString enable, QString format, QString pos, QString font_size)
{
    char set_osd[256] = { 0 };
    snprintf(set_osd, 252, T_SET_OSD, chnn, enable.toLatin1().data(), format.toLatin1().data(), pos.toLatin1().data(), font_size.toLatin1().data());

    char* p_res = NULL;
//    qDebug()<<"set_osd:"<<set_osd;
    int ret = l_sdk_request(id, set_osd, &p_res);
//    qDebug()<<"setOSD:"<<p_res;
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_ntp(int id, QString enable, QString server, int port, int interval)
{
    char set_ntp[256] = { 0 };
    snprintf(set_ntp, 252, T_SET_NTP, enable.toLatin1().data(), server.toLatin1().data(), port, interval);
//    qDebug()<<enable<<enable.toLatin1().data();
    char* p_res = NULL;
//    qDebug()<<"set_ntp:"<<set_ntp;
    int ret = l_sdk_request(id, set_ntp, &p_res);
//    qDebug()<<"setOSD:"<<p_res;
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_wireless_ipv4(int id, int code, QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2)
{
    char set_wireless_ipv4[256] = { 0 };
    snprintf(set_wireless_ipv4, 252, T_SET_WIRELESS_IPV4, code, dhcp.toLatin1().data(), ip.toLatin1().data(), gateway.toLatin1().data(), netmask.toLatin1().data(), dns1.toLatin1().data(), dns2.toLatin1().data());

    char* p_res = NULL;
//    qDebug()<<set_wireless_ipv4;
    int ret = l_sdk_request(id, set_wireless_ipv4, &p_res);
//    qDebug()<<"asdasdsdsdsdsdsdsddsd"<<p_res;
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_wireless(int id, QString type, QString net, QString ssid, QString passwd, QString enc)
{
    char set_wireless[256] = { 0 };
    snprintf(set_wireless, 252, T_SET_WIRELESS, type.toLatin1().data(), net.toLatin1().data(), ssid.toLatin1().data(), passwd.toLatin1().data(), enc.toLatin1().data());

    char* p_res = NULL;
//    qDebug()<<set_wireless;
    int ret = l_sdk_request(id, set_wireless, &p_res);
//    qDebug()<<"sssssssssssssss"<<p_res;
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::set_stream_pic(int id, int chnn, int idx, QString fmt, QString wh, QString quality, int interval_ms)
{
    char set_stream_pic[256] = { 0 };
    snprintf(set_stream_pic, 252, T_SET_STREAM_PIC, chnn, idx, fmt.toLatin1().data(), wh.toLatin1().data(), quality.toLatin1().data(), interval_ms);
//    qDebug()<<set_stream_pic;
    char* p_res = NULL;
    int ret = l_sdk_request(id, set_stream_pic, &p_res);
//    qDebug()<<"ret:"<<ret<<"p_res:"<<p_res;
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int MainWindow::cb_sdk_media(void *p_obj, int protocol, int id, int chnn, int idx, int md_id, l_md_buf_t *p_data)
{
    // p_obj 是调用 l_sdk_md_add_listener函数的第三个参数指针, 这里没有用到
    // protocol 为 proto/l_nspp.h 文件中定义主子协议类型
    // id 为登录ID
    // chnn 设备通道
    // idx 流序列号 参见 "proto/l_media.h" l_md_idx_e

    int proto_main = L_NET_PROTO_MAIN(protocol);
    int proto_sub = L_NET_PROTO_SUB(protocol);

    if (p_data->ver == L_MD_F_VER)
    {
        //printf("fmt:%d,type:%d,len:%d\n", p_data->f_v1.fmt, p_data->f_v1.v_type, p_data->f_v1.len);

        if (L_FMT_H264 == p_data->f_v1.fmt)
        {
            if (L_MVT_I == p_data->f_v1.v_type)
            {
                // 关键帧
            }
            else
            {
                // 非关键帧
            }

            // H264
            int h264_len = p_data->f_v1.len;
            unsigned char* p_h264 = (unsigned char*)(p_data->p_buf + p_data->start);

            //printf("h264 [%x,%x,%x,%x,%x]\n", p_h264[0], p_h264[1], p_h264[2], p_h264[3], p_h264[4]);
        }
        else if (L_FMT_H265 == p_data->f_v1.fmt)
        {

        }
        else if (L_FMT_AUDIO_B < p_data->f_v1.fmt && p_data->f_v1.fmt < L_FMT_AUDIO_E)
        {

        }
        else if (L_FMT_PIC_B < p_data->f_v1.fmt && p_data->f_v1.fmt < L_FMT_PIC_E)
        {

        }
    }

    return 0;
}

int MainWindow::ntp_sync(int id, QString server, int port)
{
    char ntp_sync[128] = { 0 };
    snprintf(ntp_sync, 124, T_SYNC, server.toLatin1().data(), port);
//    qDebug()<<"sadasdads"<<ntp_sync;
    char* p_res = NULL;
    int ret = l_sdk_request(id, ntp_sync, &p_res);
//    qDebug()<<p_res;
    if(NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

void MainWindow::wait()
{
    fMessage->setDialog(tr("logging..."), false, false);
    fMessage->show();
}

void MainWindow::init()
{
    wsa_startup();

    ret = l_sdk_init("");
    printf("(%s.%d)sdk init,ret=%d\n", __FILE__, __LINE__, ret);
    // 添加媒体数据监听者
    QString str = "my listener 1";
    char* temp = str.toLatin1().data();
    l_sdk_md_add_listener(temp, cb_sdk_media, NULL);

    // 打开一个解码器
    ret = l_sdk_dec_open(T_STREAM_DEC_ID,  "{\"pix_fmt\":\"RGB888\"}" );
//    printf("(%s.%d)sdk dec open,ret=%d\n", __FILE__, __LINE__, ret);

    // 请求登录
//    char req_login[128] = { 0 };
//    snprintf(req_login, 124, T_LOGIN_STR, "192.168.3.150", 80, "admin", "123456");

//    id = 0;
//    qDebug()<<req_login;
//    ret = l_sdk_login(&id, req_login);
//    printf("(%s.%d)sdk login,ret=%d,id=%d\n", __FILE__, __LINE__, ret, id);
//    start();
}

void MainWindow::startStream()
{
    if (0 == ret)
    {
        // 登录成功, 后请求流
        request_stream(id, 0, 0);
        // 绑定解码器
        l_sdk_dec_bind(T_STREAM_DEC_ID, id, 0, 0, 0);
    }
    stream(id, 0, 0);
    timer = new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    workThread->run();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(16);
}

void MainWindow::stopStream()
{
    stop_stream(id, 0, 0);
}

void MainWindow::setImage()
{
//    set_stream(id, 0, 0, "h264", "vbr", "1920*1080", "high", 25, 4096, 90);
    stream(id, 0, 0);
//    get_Image(id, 0);
//    set_Image(id, 0, 90, 90, 90, 90);
}

void MainWindow::loginOut()
{
    l_sdk_logout(id);
    l_sdk_dec_close(T_STREAM_DEC_ID);
    l_sdk_quit();
}

void MainWindow::getAllData()
{
    stream(id, 0, 0);
    get_Image(id, 0);
    get_Image_Rotate(id,0);
    get_Image_exposure(id,0);
    get_Image_mirror_flip(id, 0);
    get_Image_awb(id, 0);
    get_stream_pic(id, 0, 64);
}

void MainWindow::getNetData()
{
    get_ipv4(id, 0);
    get_wireless(id, 0);
    get_wireless_ipv4(id, 0);
}

void MainWindow::getUpgradeMessage()
{
    upThread->run();
//    upgradetime++;
//    char upgrade[128] = { 0 };
//    snprintf(upgrade, 124, T_GET_UPGRADE);
//    qDebug()<<"upupup"<<upgradetime;
//    char* p_res = NULL;
//    l_sdk_request(id, upgrade, &p_res);
//    QJsonDocument temp = QJsonDocument::fromJson(p_res);
//    QJsonObject recJson = temp.object();
//    if(recJson.value("status_upgrade ").toObject().value("status").toString() == "done")
//    {
//        if(recJson.value("status_upgrade ").toObject().value("percentage ").toInt() == 100)
//        {
//            qDebug()<<"succesful";
//            QMessageBox msgBox;
//            msgBox.setText(tr("Upgrade success!"));
//            msgBox.exec();
//            upgradeTimer->stop();
//        }else{
//            qDebug()<<"failed";

//            QMessageBox msgBox;
//            msgBox.setText(tr("Upgrade failed!"));
//            msgBox.exec();
//            upgradeTimer->stop();
//        }
//    }
//    if(upgradetime>60)
//    {
//        qDebug()<<"failed";
//        upgradetime = 0;
//        QMessageBox msgBox;
//        msgBox.setText(tr("Upgrade failed!"));
//        msgBox.exec();
//        upgradeTimer->stop();
//    }
}

//void MainWindow::getImage(QImage img)
//{
//    ui->label->resize(ui->widget->size());
//    img = img.scaled(ui->label->size(), Qt::KeepAspectRatio);
//    ui->label->updateImage(img);
//}


void MainWindow::getImage(QPixmap img)
{
    ui->label->resize(ui->widget->size());
    img = img.scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setPixmap(img);
}

void MainWindow::onTimeOut()
{
//    qDebug()<<"TimeOut:"<<QTime::currentTime();
    workThread->run();
}

//void MainWindow::start_stream_slot()
//{
//    startStream();
//    connect(workThread, SIGNAL(lostSignals()), this, SLOT(lostSignals()));
//    ui->actionstart_stream->setEnabled(false);
//    ui->actionstop_stream->setEnabled(true);
//}

//void MainWindow::stop_stream_slot()
//{
////    request_stream(id, 0, 64);
//////    l_sdk_dec_bind(T_STREAM_DEC_ID, id, 0, 64, 0);

////    l_md_data_t* p_md_data = NULL;
////    l_sdk_dec_get_md_data(T_STREAM_DEC_ID, &p_md_data);
////    qDebug()<<p_md_data;
////    int w = p_md_data->w;
////    int h = p_md_data->h;
////    qDebug()<<"w:"<<p_md_data->w<<"h:"<<p_md_data->h;
////    uchar* d = new uchar[w*h*3];
////    memcpy(d,p_md_data->p_rgb,w*h*3);
////    QImage img(d, w, h, QImage::Format_RGB888);
////    qDebug()<<img;
////    qDebug()<<img.save("E:/Qt_project/IPCtest/123.jpg", "jpg");

//    stopStream();
//    workThread->disconnect(SIGNAL(lostSignals()));
//    ui->actionstop_stream->setEnabled(false);
//    ui->actionstart_stream->setEnabled(true);
//}

void MainWindow::stream_setting_slot()
{
    getAllData();
    allSettingDialog->setCurrentPage(1);
    allSettingDialog->exec();
}

void MainWindow::image_setting_slot()
{
    getAllData();
    allSettingDialog->setCurrentPage(0);
    allSettingDialog->exec();
}

void MainWindow::osd_setting_slot()
{
    get_OSD(id,0);
    osdDialog->exec();
}

void MainWindow::ntp_setting_slot()
{
    get_NTP(id,0);
    ntpDialog->exec();
}

void MainWindow::system_base_slot()
{
    get_System_Base(id,0);
    basicDialog->exec();
}

void MainWindow::ipv4_slot()
{
    getNetData();
    netSettingDialog->setCurrentPage(0);
    netSettingDialog->exec();
}

void MainWindow::wireless_ipv4_slot()
{
    getNetData();
    netSettingDialog->setCurrentPage(1);
    netSettingDialog->exec();
}

void MainWindow::wireless_slot()
{
    getNetData();
    netSettingDialog->setCurrentPage(2);
    netSettingDialog->exec();
}

void MainWindow::picture_setting_slot()
{
    getAllData();
    allSettingDialog->setCurrentPage(2);
    allSettingDialog->exec();
}

void MainWindow::upgrade_slot()
{
    QString openFileName, openFilePath;
    openFileName = QFileDialog::getOpenFileName(this, tr("Please choose upgrade file"),
                                                "C:/", "(*.lpk)");
    QJsonDocument upgrade;
    QJsonObject object;
    if(openFileName != "")
    {
        QFileInfo openFileInfo;
        openFileInfo = QFileInfo(openFileName);
        openFilePath = openFileInfo.filePath();
        object.insert("cmd","upgrade");
        object.insert("ip",m_IP);
        object.insert("port",m_port.toInt());
        object.insert("path",openFilePath);
        QJsonObject obj_upgrade;
        obj_upgrade.insert("username",m_user);
        obj_upgrade.insert("passwd",m_passwd);
        object.insert("upgrade",obj_upgrade);
        upgrade.setObject(object);
//        qDebug()<<upgrade.toJson(QJsonDocument::Compact);
        char set_upgrade[256] = { 0 };
        snprintf(set_upgrade, 252, upgrade.toJson(QJsonDocument::Compact), 0, 0);
//        qDebug()<<"sssssssssssss:"<<set_upgrade;
        char* p_res = NULL;
        l_sdk_request(0, set_upgrade, &p_res);
//        qDebug()<<"saaaaaaaaaa"<<p_res;
        upgradeTimer = new QTimer();
        upgradeTimer->setTimerType(Qt::PreciseTimer);
        connect(upgradeTimer, SIGNAL(timeout()), this, SLOT(getUpgradeMessage()));
        connect(upThread, SIGNAL(timerStop()), upgradeTimer, SLOT(stop()));

        upgradeTimer->start(500);
    }
}

void MainWindow::close_slot()
{
    loginOut();
    this->close();
}

void MainWindow::setVedioStream()
{
    stopStream();
    ui->label->clear();
//    QString tempWidth = vedioSetDialog->wh().split("*").at(0);
//    QString tempHeight = vedioSetDialog->wh().split("*").at(1);
//    workThread->width = tempWidth.toInt();
//    workThread->height = tempHeight.toInt();
    set_stream(id, 0, 0, allSettingDialog->fmt(), allSettingDialog->rc_mode(),allSettingDialog->wh(), allSettingDialog->quality(), allSettingDialog->frame_rate(), allSettingDialog->bitrate(), allSettingDialog->i_interval());
    startStream();
}

void MainWindow::set_flip_mirror(bool flip, bool mirror)
{
    QString s_flip = flip ? "true" : "false";
    QString s_mirror = mirror ? "true" : "false";
    set_Image_flip_mirror(id, 0, s_flip.toLatin1().data(), s_mirror.toLatin1().data());
}

void MainWindow::set_rotate(int rotate)
{
//    qDebug()<<"rotate:"<<rotate;
    set_Image_Rotate(id, 0, rotate);
}

void MainWindow::set_exposure(int exposure)
{
    set_Image_exposure(id, 0, exposure);
}

void MainWindow::set_image_awb(QString awb, int b, int gb, int gr, int r)
{
    set_Image_awb(id, 0, awb, b, gb, gr, r);
}

void MainWindow::set_image_bcsh(int bright, int contrast, int saturation, int hue)
{
    set_Image(id, 0, bright, contrast, saturation, hue);
}

void MainWindow::set_IPv4(QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2)
{
    set_ipv4(id, dhcp, ip, gateway, netmask, dns1, dns2);
}

void MainWindow::set_OSD(QString enable, QString format, QString pos, QString font_size)
{
    set_osd(id, 0, enable, format, pos, font_size);
}

void MainWindow::set_NTP(QString enable, QString server, int port, int interval)
{
    set_ntp(id, enable, server, port, interval);
}

void MainWindow::set_NTP_sync(QString server, int port)
{
    ntp_sync(id, server, port);
}

void MainWindow::set_Wireless_IPv4(QString dhcp, QString ip, QString gateway, QString netmask, QString dns1, QString dns2)
{
    set_wireless_ipv4(id, 0, dhcp, ip, gateway, netmask, dns1, dns2);
}

void MainWindow::set_Wireless(QString type, QString net, QString ssid, QString passwd, QString enc)
{
    set_wireless(id, type, net, ssid, passwd, enc);
}

void MainWindow::set_Stream_Pic(QString fmt, QString wh, QString quality, int interval_ms)
{
    set_stream_pic(id, 0, 64, fmt, wh, quality, interval_ms);
}

void MainWindow::setChineseLanguage()
{
    m_pTranslator->load(":/cn.qm");
    qApp->installTranslator(m_pTranslator);
//    qDebug()<<qApp->installTranslator(m_pTranslator);
    ui->retranslateUi(this);
    ui->actionChinese->setChecked(true);
    ui->actionEnglish->setChecked(false);
}

void MainWindow::setEnglishLanguage()
{
    m_pTranslator->load(":/en.qm");
    qApp->installTranslator(m_pTranslator);
//    qDebug()<<qApp->installTranslator(m_pTranslator);

    ui->retranslateUi(this);
    ui->actionChinese->setChecked(false);
    ui->actionEnglish->setChecked(true);
}

void MainWindow::lostSignals()
{
    timer->stop();
    ui->label->clear();
    ui->label->setText(tr("The connection has been disconnected. Please close the software and log in again!"));
}

void MainWindow::get_isp()
{
    get_image_isp(id, 0);
}

