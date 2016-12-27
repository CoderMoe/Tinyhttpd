#ifndef __SOCKET_CHAT_H__
#define __SOCKET_CHAT_H__

#include "SocketOperate.h"

class SocketChat : public SocketOperate
{
public:
    string Response(string strRequest);

};

#endif


