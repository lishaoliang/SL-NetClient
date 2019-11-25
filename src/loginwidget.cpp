#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    int ret;
    ret = l_sdk_init("");
    ret = l_sdk_discover_open("");
    ret = l_sdk_discover_run(true);
//    char* p_res = NULL;
    timer = new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()),this, SLOT(onTimeOut()));
    timer->start(2000);
//    l_sdk_discover_get_devs(&p_res);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_loginBtn_clicked()
{
    if(w.login(ui->ipEdit->text(), ui->userEdit->text(), ui->passwdEdit->text(), ui->portEdit->text()) == 0)
    {
        timer->stop();
        this->close();
        l_sdk_discover_run(false);
        l_sdk_discover_close();
    }
}

void LoginWidget::onTimeOut()
{
    char* p_res = NULL;
    l_sdk_discover_get_devs(&p_res);
    QString tempStr = QString(p_res).remove("[");
    tempStr = tempStr.remove("]");
    QString ipStr[100];
    int ipNum = 0;
    for(int i = 1; i<100 ;i++)
    {
        ipStr[i] = tempStr.section("\"ip\"",i,i);
        if(ipStr[i] == "")
        {
            break;
        }else{
            ipStr[i] = ipStr[i].section(",",0,0);
            ipStr[i] = ipStr[i].mid(2);
            ipStr[i] = ipStr[i].left(ipStr[i].size()-1);
            QByteArray ba = ipStr[i].right(1).toLatin1();
            const char *s = ba.data();
            while(*s && *s>='0' && *s<='9') s++;
            if(*s)
            {
                ipStr[i] = ipStr[i].left(ipStr[i].size()-1);
            }
            ipNum++;
        }
    }
    ui->comboBox->clear();
    for(int i = 0; i < ipNum; i++)
    {
        ui->comboBox->addItem(ipStr[i+1]);
        if(ui->ipEdit->text() == ipStr[i+1])
        {
            ui->comboBox->setCurrentIndex(i);
        }
    }

}

void LoginWidget::on_comboBox_activated(const QString &arg1)
{
    if(arg1 != "")
    {
        ui->ipEdit->setText(arg1);
    }
}
