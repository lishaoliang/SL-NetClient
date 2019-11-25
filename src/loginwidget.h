#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();


private slots:
    void on_loginBtn_clicked();
    void onTimeOut();
    void on_comboBox_activated(const QString &arg1);

private:
    MainWindow w;
    Ui::LoginWidget *ui;
    QTimer* timer = nullptr;
};

#endif // LOGINWIDGET_H
