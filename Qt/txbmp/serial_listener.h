#ifndef SERIAL_LISTENER_H
#define SERIAL_LISTENER_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "hdlc_sender.h"

class SerialListener : public QThread
{
    Q_OBJECT

    static SerialListener *__instance;

    int mSendBufferLength;
    uint8_t *mSendBuffer;
    int mSendLength;
    bool mSendFlag;
    QMutex mSendMutex;
    QWaitCondition mSendWait;

    HDLCsender *mSender;

    void run();

    void waitSend();

public:
    SerialListener(HDLCsender *sender);
    ~SerialListener();

public slots:
    void sendData(uint8_t *buffer, int len);
};

#endif // SERIAL_LISTENER_H
