#include <QDebug>
#include "chart.h"
#include "mainwindow.h"

Chart::Chart()
{
}

void Chart::drawRadar(QPainter &painter, QRect geometry)
{
    gridXY.setX(geometry.x() + MAX_XY);
    gridXY.setY(geometry.y() + MAX_XY);
    gridWidth = geometry.width() - (2 * MAX_XY);
    gridHeight = geometry.height() - (2 * MAX_XY);

    int middleX = gridXY.x() + (gridWidth / 2);
    int middleY = gridXY.y() + (gridHeight / 2);
    int radius  = (gridWidth < gridHeight)?
                     (gridWidth / 2) + MARGIN :
                     (gridHeight / 2) + MARGIN;
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
    QRadialGradient radialgradient(QPoint(middleX, middleY),radius);
    radialgradient.setColorAt(0,  QColor(96, 128, 0, 40 ) );
    radialgradient.setColorAt(0.5,  QColor(77, 153, 0, 80) );
    radialgradient.setColorAt(1,  QColor(51, 102, 0, 120) );
    painter.setBrush(radialgradient);
    painter.drawEllipse(QPoint(middleX, middleY),radius, radius);

    /* ******************* grid ************************ */
    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    pen.setBrush(Qt::NoBrush);
    pen.setColor(Qt::white);
    painter.setPen(pen);

    /* ******************* rad grid ************************ */
    int gridM = radius / GRID_NUMBER;
    double gridRad;
    for(int i{ 0 }; i < 360; i+=15)
    {
        gridRad = (PI * i) / 180;
        painter.drawLine(middleX, middleY,
                         middleX + (radius * sin(gridRad)),
                         middleY - (radius * cos(gridRad)));
    }

    /* ******************* length grid ************************ */
    double len = 0.;
    for(int i{ 1 }; i < GRID_NUMBER; ++i)
    {
        painter.drawEllipse(QPoint(middleX, middleY), i * gridM, i * gridM);
        /* top */
        painter.drawText(QPoint(middleX , middleY - gridM - ((i-1) * gridM )),
                         QString().asprintf("%1.1f", len += 0.5 ));
        /* down */
        painter.drawText(QPoint(middleX , middleY + 15 + gridM + ((i-1) * gridM )),
                         QString().asprintf("%1.1f", len ));
        /* right */
        painter.drawText(QPoint(middleX + gridM + ((i-1) * gridM ) , middleY),
                         QString().asprintf("%1.1f", len ));
        /* left */
        painter.drawText(QPoint(middleX - gridM - 15 - ((i-1) * gridM ) , middleY),
                         QString().asprintf("%1.1f", len ));
    }
    /* ******************* desc ************************ */
    QFont font;
    font.setPointSize(8);
    painter.setFont(font);
    pen.setColor(textColor);
    painter.setPen(pen);
    for(int i{ 0 }, alignX{ 0 }, alignY{ 0 }; i < 360; i+=15)
    {
        gridRad = (PI * i) / 180.;
        if(i > 90 && i <= 180)
        {
            alignY = 10;
        }
        else if(i > 180 && i <= 270)
        {
            alignX = -25;
        }
        else if(i > 270 && i <= 360)
        {
            alignY = 0;
        }
        painter.drawText(QPoint(middleX + alignX + (radius * sin(gridRad)),
                         middleY +alignY- (radius * cos(gridRad))),
                         QString().asprintf("%3d°", i));
    }

}

void Chart::drawRadarData(QPainter &painter, QVector<double> &data)
{
    int middleX = gridXY.x() + (gridWidth / 2);
    int middleY = gridXY.y() + (gridHeight / 2);
    double radius  = (gridWidth < gridHeight)?
                     (gridWidth / 2) + MARGIN :
                     (gridHeight / 2) + MARGIN;
    radius /= 4000.;

    QPen pen(Qt::green);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2);
    painter.setPen(pen);

    double rad { 0 };
    double rad2 { 0 };
    for(int i{ 49 }; i > -50; --i)
    {
       rad  = (PI * (i-1)) / 50.;
       rad2 = (PI * i) / 50.;

    /* ******************* chart ************************ */
        painter.drawLine(middleX + (radius * data[i + 50] * sin(rad2)),
                middleY + (radius * data[i + 50] * cos(rad2)),
                middleX + (radius * data[i-1 + 50] * sin(rad)),
                middleY + (radius * data[i-1 + 50] * cos(rad)));
    }
    /* ******************* last to first ************************ */
    painter.drawLine(middleX + (radius * data[99] * sin((PI * 49) / 50.)),
            middleY + (radius * data[99] * cos((PI * 49) / 50.)),
            middleX + (radius * data[0] * sin((PI * -50) / 50.)),
            middleY + (radius * data[0] * cos((PI * -50) / 50.)));

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
    painter.drawLine(QLine(middleX, middleY,
                            middleX + radius * 4000 * sin(rad),
                            middleY + radius * 4000 * cos(rad)));

}

