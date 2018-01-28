#include "serial_listener.h"

SerialListener *SerialListener::__instance = 0;

SerialListener::SerialListener(HDLCsender *sender) : QThread(0)
{
    mSender = sender;

    if(__instance)
        delete __instance;

    __instance = this;

    mSendBufferLength = 1024;
    mSendBuffer = (uint8_t*) malloc(mSendBufferLength);
}

void SerialListener::run()
{
    while(1)
    {
    __instance->waitSend();
    }
}

void SerialListener::waitSend()
{
    mSendMutex.lock();
    mSendFlag = true;
    if(mSendWait.wait(&mSendMutex))
    {
        mSendFlag = false;
        qInfo("Listener: Send %d", mSendLength);
        mSender->send(mSendBuffer, mSendLength);

    }
    mSendMutex.unlock();

}

void SerialListener::sendData(uint8_t *buffer, int len)
{
    mSendMutex.lock();
    if(mSendFlag)
    {
        mSendWait.wakeAll();
        if(len > mSendBufferLength)
            len = mSendBufferLength;

        mSendLength = len;
        memcpy(mSendBuffer, buffer, len);
    }
    else
        qInfo("Serial: no one cares");
    mSendMutex.unlock();
}


SerialListener::~SerialListener()
{
    free(mSendBuffer);
}
