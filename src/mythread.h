#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QLabel>
#include <QSize>
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
class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread();
    void run();
    int width;
    int height;
    int i = 0;
signals:
    void sendPix(QPixmap pix);
    void sendImage(QImage img);
    void lostSignals();
};

#endif // MYTHREAD_H
