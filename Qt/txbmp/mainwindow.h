#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "serial_listener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    SerialListener *mListener;
    HDLCsender *mSender;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void buttonPress();
};

#endif // MAINWINDOW_H
