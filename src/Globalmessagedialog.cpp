#include "globalmessagedialog.h"

#include <QDebug>

GlobalMessageDialog::GlobalMessageDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::GlobalMessageDialog)
{
    ui->setupUi(this);
}

GlobalMessageDialog::~GlobalMessageDialog()
{

}

void GlobalMessageDialog::setDialog(const QString& message, bool hasCancel, bool hasOK)
{
    this->show();
    ui->messageLabel->setText(message);
    hasCancel ? ui->cancelBtn->show() : ui->cancelBtn->hide();
    hasOK ? ui->okBtn->show() : ui->okBtn->hide();
}

