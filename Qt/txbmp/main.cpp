#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include "hdlc_sender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInfo("txbmp to LEDtable");

    if(argc > 2)
    {
        qInfo("Convert and send %s via %s", argv[1], argv[2]);

        HDLCsender *sender = new HDLCsender(argv[2]);


        char buff[1024];
        memset(buff, 0x55, 1024);
        memset(buff, '\n', 10);
        for(int k = 2; k < 255; k++)
            buff[k] = k;

        buff[0] = 'J';
        buff[1] = 'A';
        buff[798] = 'J';
        buff[799] = 'A';
        sender->send(buff, 800);
        sender->wait();
    }
    else
    {
        qCritical("Please specify bitmap and serial port");
    }
    qInfo() << "DONE";
//    a.exec();

    return 0;
}

