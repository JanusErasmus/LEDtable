#include <QApplication>
#include <QDebug>
#include <QThread>

//#include "hdlc_sender.h"
//#include "bmp_reader.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    //qInfo("txbmp to LEDtable");

//    if(argc > 2)
//    {
//        //qInfo("Convert and send %s via %s", argv[1], argv[2]);

//        unsigned char pixels[1024];
//        BMPreader reader(argv[1]);
//        int len = reader.getBuffer(pixels, 1024);
//        HDLCsender *sender = new HDLCsender(argv[2]);
//        sender->send(pixels, len);
//        sender->wait();
//    }
//    else
//    {
//        qCritical("Please specify bitmap and serial port");
//    }
    //qInfo() << "DONE";
    return a.exec();
}

