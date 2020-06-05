#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "chart.h"
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
    void externalThreadTick();
    void sendCommand();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);

    QByteArray sendData;
    QByteArray readData;
    QSerialPort serial;
    QVector<double> distanceData;
    Thread thread;

    Chart chart;
    int sample_nth;
};

#endif // MAINWINDOW_H
