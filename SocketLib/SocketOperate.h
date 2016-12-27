#ifndef __SOCKET_OPERATE_H__
#define __SOCKET_OPERATE_H__

#include <string>

using namespace std;

class SocketOperate
{
public:
    virtual string Response(string strRequest) = 0;
};

#endif


