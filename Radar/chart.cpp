#include "chart.h"
#include <QDebug>
#include "mainwindow.h"

Chart::Chart()
{
}

void Chart::drawRadar(QPainter &painter, QRect geometry)
{
    gx = geometry.x() + MX;
    gy = geometry.y() + MY;
    gw = geometry.width() - (2 * MX);
    gh = geometry.height() - (2 * MY);

    double middleX = gx + (gw / 2);
    double middleY = gy + (gh / 2);
    double radius  = ((gw / 2) < (gh/2))?
                     (gw / 2) + MARGIN :
                     (gh / 2) + MARGIN;
    QPen pen;
    pen.setWidth(1);
    pen.setColor(backgroundColor);
    painter.setPen(pen);
    painter.setBrush(QColor(77, 77, 77));
    painter.drawRect(geometry);

    /* ******************* frame ************************ */
    pen.setStyle(Qt::SolidLine);
    pen.setColor(gridColor);
    pen.setWidth(2);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);

    /* ******************* ellipce draw ************************ */
    QRadialGradient radialgradient(QPointF(middleX, middleY),radius);
    radialgradient.setColorAt(0,  QColor(96, 128, 0, 40 ) );
    radialgradient.setColorAt(0.5,  QColor(77, 153, 0, 80) );
    radialgradient.setColorAt(1,  QColor(51, 102, 0, 120) );
    painter.setBrush(radialgradient);
    painter.drawEllipse(QPointF(middleX, middleY),radius, radius);

    /* ******************* grid ************************ */
    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    pen.setBrush(Qt::NoBrush);
    pen.setColor(Qt::white);
    painter.setPen(pen);

    /* ******************* rad grid ************************ */
    double gridM = radius / gridNum;
    double gridRad;
    for(int i{ 0 }; i < 360; i+=15)
    {
        gridRad = (PI * i) / 180;
        painter.drawLine(static_cast<int>(middleX) , static_cast<int>(middleY) ,
                         static_cast<int>(middleX + (radius * sin(gridRad))),
                         static_cast<int>(middleY - (radius * cos(gridRad))));
    }

    /* ******************* length grid ************************ */
    double sc = 0.;
    for(int py{1}; py < gridNum; ++py)
    {
        painter.drawEllipse(QPointF(middleX, middleY),py * gridM, py * gridM);
        /* top */
        painter.drawText(QPointF(middleX , middleY - gridM - ((py-1) * gridM )),
                         QString().asprintf("%1.1f", sc+=0.5 ));
        /* down */
        painter.drawText(QPointF(middleX , middleY + 15 + gridM + ((py-1) * gridM )),
                         QString().asprintf("%1.1f", sc ));
        /* right */
        painter.drawText(QPointF(middleX + gridM + ((py-1) * gridM ) , middleY),
                         QString().asprintf("%1.1f", sc ));
        /* left */
        painter.drawText(QPointF(middleX - gridM - 15 - ((py-1) * gridM ) , middleY),
                         QString().asprintf("%1.1f", sc ));
    }

    /* ******************* desc ************************ */
    QFont font;
    font.setPointSize(8);
    painter.setFont(font);
    pen.setColor(textColor);
    painter.setPen(pen);
    for(int i{ 0 }, align_x{ 0 }, align_y{ 0 }; i < 360; i+=15)
    {
        gridRad = (PI * i) / 180.;
        if(i > 90 && i <= 180)
        {
            align_y = 10;
        }
        else if(i > 180 && i <= 270)
        {
            align_x = -25;
        }
        else if(i > 270 && i <= 360)
        {
            align_y = 0;
        }
        painter.drawText(QPointF(static_cast<int>(middleX +align_x+ (radius * sin(gridRad))),
                         static_cast<int>(middleY +align_y- (radius * cos(gridRad)))),
                         QString().asprintf("%3d°",i ));
    }

}

void Chart::drawRadarData(QPainter &painter, QVector<double> &data)
{
    int middleX = gx + (gw / 2);
    int middleY = gy + (gh / 2);
    double radius  = ((gw / 2) < (gh/2))?
                     (gw / 2) + MARGIN :
                     (gh / 2) + MARGIN;
    radius /= 4000.;

    QPen pen(Qt::green);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2);
    painter.setPen(pen);

    double rad  = 0;
    double rad2 = 0;
    for(int i{ 49 }; i > -50; --i)
    {
       rad  = (PI * (i-1)) / 50.;
       rad2 = (PI * i) / 50.;

    /* ******************* chart ************************ */
        painter.drawLine(middleX + static_cast<int>(radius * data[i + 50] * sin(rad2)),
                middleY + static_cast<int>(radius * data[i + 50] * cos(rad2)),
                middleX + static_cast<int>(radius * data[i-1 + 50] * sin(rad)),
                middleY + static_cast<int>(radius * data[i-1 + 50] * cos(rad)));
    }
    /* ******************* lsat to first ************************ */
    painter.drawLine(middleX + static_cast<int>(radius * data[99] * sin((PI * 49) / 50.)),
            middleY + static_cast<int>(radius * data[99] * cos((PI * 49) / 50.)),
            middleX + static_cast<int>(radius * data[0] * sin((PI * -50) / 50.)),
            middleY + static_cast<int>(radius * data[0] * cos((PI * -50) / 50.)));

    pen.setStyle(Qt::SolidLine);
    pen.setColor( QColor(20, 200, 20));

    pen.setWidth(5);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);

    rad = (PI * handHead--) / 50.0;
    if(handHead == -50)
    {
        handHead = 50;
    }

    /* ******************* indicator ************************ */
    painter.drawLine(QLineF(middleX, middleY,
                            middleX + radius * 4000 * sin(rad),
                            middleY + radius * 4000 * cos(rad)));

}

