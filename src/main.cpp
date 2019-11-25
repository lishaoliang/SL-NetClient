#include "mainwindow.h"
#include "loginwidget.h"
#include <QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator m_pTranslator;
    m_pTranslator.load(":/cn.qm");
    qApp->installTranslator(&m_pTranslator);
    LoginWidget w;
//    MainWindow w;
    w.show();

    return a.exec();
}
