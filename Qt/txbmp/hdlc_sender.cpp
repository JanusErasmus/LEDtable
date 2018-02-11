#include <QDebug>
#include <QThread>

#include "hdlc_sender.h"
#include "hdlc.h"

HDLCsender::HDLCsender(QString portName)
{
    qInfo() << "HDLCsender created";
    mSerial = openPort(portName);
    mBuffer.clear();

    if(mSerial)
    {
        connect(mSerial, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(mSerial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(error(QSerialPort::SerialPortError)));
    }
}

QSerialPort *HDLCsender::openPort(QString portName)
{
    QSerialPort *serial = new QSerialPort(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if(serial->open(QIODevice::ReadWrite))
    {
        qInfo() << "Opened serial port " << serial->portName();
    }
    else
    {
        qInfo() << "Could not open serial port " << serial->portName();
        delete serial;
        serial = 0;
    }

    return serial;
}

void HDLCsender::send(unsigned char *data, qint64 len)
{
    if(!mSerial)
        return;

    unsigned char frame[2048];
    int frameLen = 2048;
    HDLC::HDLC_Frame(data, len, frame, &frameLen);

    qInfo() << "Send " << frameLen << " bytes";
    int sentLen = 0;
    int index = 0;
    while(sentLen < frameLen)
    {
        char string[128];
        memset(string, 0, 128);
        string[0] = '~';

        int txLen = (frameLen - sentLen);
        if(txLen > 60)
            txLen = 60;

        for(int k = 0; k < txLen; k++)
        {
            char numberString[8];
            sprintf(numberString, "%02X", frame[k + index]);
            strcat(string, numberString);
        }
        index += txLen;

//        char numberString[8];
//        sprintf(numberString, "%02X", 0x55);
//        strcat(string, numberString);

        strcat(string, "\r");
        int stringLen = (int)strlen(string);

        if(mSerial->isOpen())
        {
            mSerial->write((const char*)string, stringLen);
            if(!mSerial->waitForBytesWritten(2000))
            {
                qCritical("Send failure");
                return;
            }
            else
                qInfo() << "TX OK" << stringLen;

            QThread::msleep(20);
        }
        else
        {
            qCritical("Port not open");
            return;
        }

        sentLen += txLen;
    }
}

void HDLCsender::wait()
{
    while(mSerial && mSerial->waitForReadyRead(2000))
    {
    }

    qInfo() << "No bytes";
}

void HDLCsender::readData()
{
    QByteArray data = mSerial->readAll();
    int len = data.length();
    if(len)
    {
        for(int k = 0; k < len; k++)
        {
            mBuffer.append(data.at(k));
            //        qDebug() << "Serial RX: " << len;
            //        qInfo() << data;
            if(mBuffer.endsWith('\r'))// || mBuffer.endsWith('\n'))
            {
//                for(int k = 0; k < mBuffer.length(); k)
//                {
//                    char line[128];
//                    line[0] = 0;

//                    int len = mBuffer.length() - k;
//                    if(len > 16)
//                        len = 16;
//                    for(int i = 0; i < len; i++)
//                    {
//                        char tempNumber[16];
//                        sprintf(tempNumber, "0x%02X ", mBuffer.at(k + i));
//                        strcat(line, tempNumber);
//                    }
//                    k += len;

//                    qInfo() << line;
//                }
                QString code = QString(mBuffer).trimmed();
                code.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?")));
                //qDebug() << "RX: " << code;

                mBuffer.clear();
            }
        }

    }
}

void HDLCsender::error(QSerialPort::SerialPortError e)
{
    if(e == QSerialPort::ResourceError)
    {
    qCritical() << "SerialPort: Disconnected" << e;
    }
}

HDLCsender::~HDLCsender()
{
    if(mSerial)
    {
        mSerial->close();
        delete mSerial;
    }
}
