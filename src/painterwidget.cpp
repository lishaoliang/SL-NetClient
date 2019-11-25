#include "painterwidget.h"
#include <QPainter>
#include <QDebug>

PainterWidget::PainterWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

PainterWidget::~PainterWidget()
{

}

void PainterWidget::updateImage(QImage img)
{
    image = img.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    update();
}

void PainterWidget::paintEvent(QPaintEvent *)
{
    qDebug()<<"crash";
    QPainter painter;
    painter.begin(this);
    painter.drawImage(QPoint(0, 0), image);

    painter.end();
}
