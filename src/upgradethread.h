#ifndef UPGRADETHREAD_H
#define UPGRADETHREAD_H
#include <QThread>
#include <QObject>
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

class UpgradeThread : public QThread
{
    Q_OBJECT
public:
    UpgradeThread();
    void run();

private:
    int upgradetime = 0;
signals:
    void timerStop();
};

#endif // UPGRADETHREAD_H
