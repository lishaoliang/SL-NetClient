#include "mythread.h"
#include <QPixmap>
#include <Windows.h>
#include <assert.h>
#include <QDebug>
#include <QTime>
#define T_STREAM_DEC_ID         100

MyThread::MyThread()
{

}

void MyThread::run()
{
//    qDebug()<<"run:"<<QTime::currentTime();
    i++;
    uchar* d = new uchar[width*height*3];
    l_md_data_t* p_md_data = NULL;
    if (0 == l_sdk_dec_get_md_data(T_STREAM_DEC_ID, &p_md_data))
    {
        assert (NULL != p_md_data);
        if (NULL != p_md_data)
        {
            i = 0;
//            qDebug()<<"getImage:"<<QTime::currentTime();
            memcpy(d,p_md_data->p_rgb, width*height*3);
//            qDebug()<<"1:"<<QTime::currentTime();
            QImage img(d, width, height, QImage::Format_RGB888);
//            qDebug()<<"2:"<<QTime::currentTime();
            l_sdk_dec_free_md_data(p_md_data);
//            emit sendImage(img);
//            qDebug()<<"3:"<<QTime::currentTime();
            QPixmap pix = QPixmap::fromImage(img);
//            QPixmap pix = QPixmap();
//            qDebug()<<"4:"<<QTime::currentTime();
            emit sendPix(pix);
//            qDebug()<<"send"<<QTime::currentTime();
        }
    }
    if(i>300)
    {
        emit lostSignals();
    }
    delete d;
}



