#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent), ui(new Ui::MainWindow)
{
    sample_nth = (DATA_SIZE / 2) - 1;
    distanceData.resize(DATA_SIZE/2);
    distanceData.fill(0);

    ui->setupUi(this);
    connect(&thread, SIGNAL(tick()), this, SLOT(externalThreadTick()));
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(sendCommand()));

    ui->statusBar->showMessage("No device");
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : infos)
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

void MainWindow::externalThreadTick()
{
    if (serial.size() >= 2)
    {
        readData = serial.readAll();
        uint16_t* sample=reinterpret_cast<uint16_t*>(readData.data());
        distanceData[sample_nth] = static_cast<double>(*sample);
        --sample_nth;
        if(sample_nth < 0)
        {
            sample_nth = (DATA_SIZE / 2) - 1;
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
    sendData.clear();
    sendData[0]=static_cast<char>(128);
    serial.write(sendData);
}

std::random_device rd;
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    /* ******************** visualization without distance reader ********************** */
    std::uniform_real_distribution<double> randomNumber(0, 4000);
    distanceData[sample_nth] = randomNumber(rd);
    --sample_nth;
    if(sample_nth<0)
    {
        sample_nth = (DATA_SIZE / 2) - 1;
    }
    /* ******************** visualization without distance reader ********************** */
    update();
    chart.drawRadar(painter, centralWidget()->geometry());
    chart.drawRadarData(painter, distanceData);
}
