#include "xtool.h"

XRecorder XRec;

XRecorder::XRecorder(void)
{
}

void XRecorder::Quant(void)
{
    frameCount ++;

    if (flags & XRC_PLAY_MODE) {
        if (!(flags & XRC_MESSAGE_READ)) {
            GetMessage();
        }

        while (flags & XRC_MESSAGE_READ && frameCount == nextMsg->Frame) {
            DispatchMessage();
            GetMessage();
        }
    }

    xtClearMessageQueue();
}

void XRecorder::Open(char* fname,int mode)
{
}

void XRecorder::Close(void)
{
}

void XRecorder::PutMessage(int msg,int sz,void* p)
{
}

void XRecorder::GetMessage(void)
{
}

void XRecorder::DispatchMessage(void)
{
}

int XRecorder::CheckMessage(int code)
{
    // TODO
    return 1;
}
