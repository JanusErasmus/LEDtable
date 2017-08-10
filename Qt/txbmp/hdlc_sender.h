#ifndef BMP_SENDER_H
#define BMP_SENDER_H
#include <QtSerialPort/QSerialPort>
#include <QObject>

class HDLCsender : public QObject
{
    Q_OBJECT

    QByteArray mBuffer;
    QSerialPort *mSerial;

    QSerialPort *openPort(QString portName);
public:
    HDLCsender(QString portName);
    ~HDLCsender();

    void send(unsigned char *data, qint64 len);
    void wait();

private slots:
    void readData();
    void error(QSerialPort::SerialPortError);
};

#endif // BMP_SENDER_H
