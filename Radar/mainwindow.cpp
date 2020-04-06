#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent), ui(new Ui::MainWindow)
{
    N = DSIZE2;
    n = N-1;
    distanceData.resize(N);
    distanceData.fill(0);

    ui->setupUi(this);
    connect(&thread, SIGNAL(tick()), this, SLOT(externalThread_tick()));
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(sendCommand()));

    ui->statusBar->showMessage("No device");
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        if(info.serialNumber()=="NXP-77")
        {
            auto portname=info.portName();
            serial.setPortName(portname);
            if (serial.open(QIODevice::ReadWrite))
            {
                //serial.setBaudRate( serial.Baud115200,  serial.AllDirections);
                ui->statusBar->showMessage(tr("Device: %1").arg(info.serialNumber()));
                serial.clear();
                thread.start(thread.HighestPriority);
            }
            else
            {
                ui->statusBar->showMessage(tr("Can't open %1, error code %2") .arg(serial.portName()).arg(serial.error()));
                return;
            }
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    thread.terminate();
    thread.wait();
    serial.close();
    delete ui;
}

void MainWindow::externalThread_tick()
{
    if (serial.size() >= 2)
    {
        readdata = serial.readAll();
        uint16_t* sample=reinterpret_cast<uint16_t*>(readdata.data());
        distanceData[n] = static_cast<double>(*sample);
        --n;
        if(n<0)
        {
            n=N-1;
        }
        update();
        if(ui->actionRun->isChecked())
        {
            sendCommand();
        }
    }
}

void MainWindow::sendCommand()
{
    senddata.clear();
    senddata[0]=static_cast<char>(128);
    serial.write(senddata);
}

std::random_device rd;
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    /* ******************** visualization without distance reader ********************** */
    //std::uniform_real_distribution<double> randomNumber(0, 4000);
    //distanceData[n] = randomNumber(rd);
    //--n;
    //if(n<0){
    //    n=N-1;
    //}
    /* ******************** visualization without distance reader ********************** */
    update();
    chart.drawRadar(painter, centralWidget()->geometry());
    chart.drawRadarData(painter, distanceData);
}
