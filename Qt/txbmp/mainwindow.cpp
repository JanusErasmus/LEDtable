#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bmp_reader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mSender = new HDLCsender("/dev/ttyACM0");

    mListener = new SerialListener(mSender);
    mListener->start();

    connect(ui->updateButton, SIGNAL(clicked(bool)), this, SLOT(buttonPress()));
}


void MainWindow::buttonPress()
{
    BMPreader reader("test.bmp");
    uint8_t buff[2048];
    int len = reader.getBuffer(buff, 2048);
    mListener->sendData(buff, len);
}

MainWindow::~MainWindow()
{
    delete ui;
}
