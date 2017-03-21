#ifndef VEHICLE_H
#define VEHICLE_H
#include <QPainter>
#include <QList>

class Pixel : public QObject
{    
    Q_OBJECT

    QColor mColor;

protected:
    double xPos;
    double yPos;
    int length;
    int width;

public:
    Pixel(double x, double y, int w, int l);

    void paint(QPainter * p);
    void setColor(QColor color){ mColor = color; }
};

#endif // VEHICLE_H
