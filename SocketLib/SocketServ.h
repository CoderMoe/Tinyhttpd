#ifndef __SOCKET_SERV_H__
#define __SOCKET_SERV_H__

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "SocketOperate.h"

#define _SOCKET_DEBUG_

class SocketServ
{
public:
    SocketServ()
    {
        m_hSocket = -1;
        m_hAcceptSocket = -1;

        m_acceptNameLen = sizeof(m_acceptName);

        m_pSocketOperate = NULL;
    }

    ~SocketServ(){}

    void SetOperator(SocketOperate* pSocketOperate);

    int StartUp(u_short *port);
    void Stop();

    int Accept();
    void AcceptRequest();

private:
    int GetLine(char* buf, int size);

    int WriteLine(const char* buf, int size);

    void error_die(const char* sc);

    int m_hSocket;
    int m_hAcceptSocket;

    struct sockaddr_in m_name;

    struct sockaddr_in m_acceptName;
    socklen_t m_acceptNameLen;

    SocketOperate* m_pSocketOperate;
};

#endif


