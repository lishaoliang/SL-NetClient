#ifndef PAINTERWIDGET_H
#define PAINTERWIDGET_H
#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QWidget>
class PainterWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    PainterWidget(QWidget* parent = nullptr);
    ~PainterWidget();
    void updateImage(QImage img);

protected:
    void paintEvent(QPaintEvent*);

private:
    QImage image;
};

#endif // PAINTERWIDGET_H
