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

    mTetris = new tetris();

    // Null out the pointers contained in the array:
    for (int x = 0; x < 16; ++x)
    {
      for (int y = 0; y < 16; ++y)
      {
          mGrid[x][y] = 0;
      }
    }

    //mGrid[0][0] = new RGB(100, 0, 0);

    animate();

    QTimer * trigger = new QTimer(this);
    connect(trigger, SIGNAL(timeout()), this, SLOT(animate()));
    trigger->start(100);

}

void MainWindow::animate()
{
    mTable->clear();

    mTetris->getGrid(&mGrid[0][0]);
    for(uint8_t x = 0; x < 16; x++)
    {
        for(uint8_t y = 0; y < 16; y++)
        {
            RGB *p = mGrid[x][y];
            if(p)
            {
                mTable->setPixel(x, y, QColor(p->red, p->green, p->blue));
            }
        }
    }

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
