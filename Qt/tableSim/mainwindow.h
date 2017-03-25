#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "led_table.h"
#include "tetris.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    RGB *mGrid[16][16];

    LEDtable *mTable;
    tetris *mTetris;

    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void animate();
};

#endif // MAINWINDOW_H
