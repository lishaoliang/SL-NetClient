#include "upgradethread.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#define T_GET_UPGRADE   "{\"cmd\":\"status_upgrade\",\"ip\":\"%s\",\"port\":%d}"

UpgradeThread::UpgradeThread()
{

}

void UpgradeThread::run()
{
    upgradetime++;
    char upgrade[128] = { 0 };
    snprintf(upgrade, 124, T_GET_UPGRADE);
//    qDebug()<<"upupup"<<upgradetime;
    char* p_res = NULL;
    l_sdk_request(0, upgrade, &p_res);
//    qDebug()<<p_res;
    QJsonDocument temp = QJsonDocument::fromJson(p_res);
    QJsonObject recJson = temp.object();
//    qDebug()<<recJson.value("status_upgrade").toObject().value("status").toString();
    if(recJson.value("status_upgrade").toObject().value("status").toString() == "done")
    {
        if(recJson.value("status_upgrade").toObject().value("percentage").toInt() == 100)
        {
//            qDebug()<<"succesful";
            QMessageBox msgBox;
            msgBox.setText(tr("Upgrade success!"));
            msgBox.exec();
            emit timerStop();
        }else{
//            qDebug()<<"failed";
            QMessageBox msgBox;
            msgBox.setText(tr("Upgrade failed!"));
            msgBox.exec();
            emit timerStop();
        }
    }
    if(upgradetime>60)
    {
//        qDebug()<<"failed";
        upgradetime = 0;
        QMessageBox msgBox;
        msgBox.setText(tr("Upgrade failed!"));
        msgBox.exec();
        emit timerStop();
    }
}
