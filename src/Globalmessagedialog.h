#ifndef GLOBALMESSAGEDIALOG_H
#define GLOBALMESSAGEDIALOG_H

#include <QDialog>
#include "ui_Globalmessagedialog.h"

namespace Ui {
class GlobalMessageDialog;
}

class GlobalMessageDialog : public QDialog
{
	Q_OBJECT

public:
    GlobalMessageDialog(QWidget *parent = nullptr);
    ~GlobalMessageDialog();

    void setDialog(const QString&, bool, bool);

private slots:
	void on_cancelBtn_clicked() { reject(); }
	void on_okBtn_clicked() { accept(); }

private:
    Ui::GlobalMessageDialog* ui;
};

#endif // GLOBALMESSAGEDIALOG_H
