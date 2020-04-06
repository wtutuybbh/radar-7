#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QPainter>
#include "chart.h"
#include "parameters.h"
#include "thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void externalThread_tick();
    void sendCommand();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);

    QByteArray senddata;
    QByteArray readdata;
    QSerialPort serial;
    QVector<double> distanceData;
    Thread thread;

    Chart chart;
    int N;
    int n;
};

#endif // MAINWINDOW_H
