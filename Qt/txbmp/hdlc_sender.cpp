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

    unsigned char txData[2048];
    int txLen = 0;
    for(int k = 0; k < len; k++)
    {
        if(data[k] == '\n') //insert escape
        {
            txData[txLen++] = 0x7D;
            txData[txLen++] = '\n' ^ 0x20;
        }
        else if(data[k] == '\r') //insert escape
        {
            txData[txLen++] = 0x7D;
            txData[txLen++] = '\r' ^ 0x20;
        }
        else if(data[k] == 8) //insert escape
        {
            txData[txLen++] = 0x7D;
            txData[txLen++] = 8 ^ 0x20;
        }
        else if(data[k] == 0) //insert escape
        {
            txData[txLen++] = 0x7D;
            txData[txLen++] = 0x20;
        }
        else if(data[k] == 127) //insert escape
        {
            txData[txLen++] = 0x7D;
            txData[txLen++] = 127 ^ 0x20;
        }
        else if(data[k] == '\b') //insert escape
        {
            txData[txLen++] = 0x7D;
            txData[txLen++] ='\b' ^ 0x20;
        }
        else
        {
            txData[txLen++] = data[k];
        }
    }
    qInfo("Sending %d", txLen);
    unsigned char txFrameData[4098];
    int txFrameLen = 4098;
    HDLC::HDLC_Frame(txData, txLen, txFrameData, &txFrameLen);    
    qInfo("frame  %d", txFrameLen);

    int transmitIndex = 0;
    int transmit = txFrameLen;
    while(transmit)
    {
        int sent = transmit;
        if(sent > 60)
            sent = 60;

        unsigned char frame[120];
        int frameLen = 120;
        HDLC::HDLC_Frame(&txFrameData[transmitIndex], sent, frame, &frameLen);
        frame[frameLen++] = '\r';

        if(mSerial->isOpen())
        {
            mSerial->write((const char*)frame, frameLen);
            if(!mSerial->waitForBytesWritten(2000))
            {
                qCritical("Send failure");
                return;
            }
            else
                qInfo() << "TX OK" << frameLen;

            QThread::msleep(200);
        }
        else
        {
            qCritical("Port not open");
            return;
        }

        transmit -= sent;
        transmitIndex += sent;
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
                for(int k = 0; k < mBuffer.length(); k)
                {
                    char line[128];
                    line[0] = 0;

                    int len = mBuffer.length() - k;
                    if(len > 16)
                        len = 16;
                    for(int i = 0; i < len; i++)
                    {
                        char tempNumber[16];
                        sprintf(tempNumber, "0x%02X ", mBuffer.at(k + i));
                        strcat(line, tempNumber);
                    }
                    k += len;

                    qInfo() << line;
                }
//                QString code = QString(mBuffer).trimmed();
//                code.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?")));
//                qDebug() << "RX: " << code;

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
