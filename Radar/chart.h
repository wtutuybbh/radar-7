#ifndef CHART_H
#define CHART_H

#include "parameters.h"

class Chart
{
public:
    Chart();
    void drawRadar(QPainter &painter, QRect geometry);
    void drawRadarData(QPainter &painter, QVector<double> &data);

private:
    QPoint gridXY;
    int gridWidth;
    int gridHeight;

    int handHead{ 49 };
};

#endif // CHART_H
