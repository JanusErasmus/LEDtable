#include <QTimer>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mTable = new LEDtable(16, 16);
    mClock = new Clock(mTable);

    QTimer * trigger = new QTimer(this);
    connect(trigger, SIGNAL(timeout()), this, SLOT(animate()));
    trigger->start(2500);
}

void MainWindow::animate()
{
    mClock->update();

    repaint();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    mTable->paint(&painter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
