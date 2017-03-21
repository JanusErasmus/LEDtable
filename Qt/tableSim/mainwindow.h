#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "led_table.h"
#include "clock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    LEDtable *mTable;
    Clock *mClock;

    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void animate();
};

#endif // MAINWINDOW_H
