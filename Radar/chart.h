#ifndef CHART_H
#define CHART_H

#include <QPainter>
#include <cmath>
#include "parameters.h"

class Chart
{
public:
    Chart();
    void drawRadar(QPainter &painter, QRect geometry);
    void drawRadarData(QPainter &painter, QVector<double> &data);

    int gridNum { 10 };
    int handHead{ 49 };

    QColor backgroundColor  = QColor(77, 77, 77);
    QColor gridColor        = QColor(45, 45, 45);
    QColor textColor        = Qt::white;

private:
    int gx; // x starting point of a chart drawing area
    int gy; // y starting point of a chart drawing area
    int gw; // width of a chart drawing area
    int gh; // height of a chart drawing area
};

#endif // CHART_H
