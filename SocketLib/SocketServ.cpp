#include "SocketServ.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void SocketServ::SetOperator(SocketOperate* pSocketOperate)
{
    m_pSocketOperate = pSocketOperate;
}

int SocketServ::StartUp(u_short *port)
{
    m_hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (m_hSocket == -1)
    {
        error_die("StartUp()");
    }

    memset(&m_name, 0, sizeof(m_name));
    m_name.sin_family = AF_INET;
    m_name.sin_port = htons(*port);
    m_name.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_hSocket, (struct sockaddr*)&m_name, sizeof(m_name)) < 0)
    {
        error_die("bind()");

        return -1;
    }

    if (*port == 0)  /* if dynamically allocating a port */
    {
        socklen_t namelen = sizeof(m_name);
        if (getsockname(m_hSocket, (struct sockaddr*)&m_name, &namelen) == -1)
        {
            error_die("getsockname()");
            return -1;
        }
        *port = ntohs(m_name.sin_port);
    }

    if (listen(m_hSocket, 5) < 0)
    {
        error_die("listen");
    }

    printf("Socket server is running on port %d\n", (int)*port);

    return 0;
}

void SocketServ::Stop()
{
    close(m_hSocket);
}

int SocketServ::Accept()
{
#ifdef _SOCKET_DEBUG_
    printf("\nstart waiting request ...\n");
#endif

    m_hAcceptSocket = accept(m_hSocket,
            (struct sockaddr *)&m_acceptName,
            &m_acceptNameLen);

    if (m_hAcceptSocket == -1)
    {
        error_die("Accept");
        return -1;
    }

#ifdef _SOCKET_DEBUG_
    printf("got a request\n");
#endif

    return 0;
}

void SocketServ::AcceptRequest()
{
    if (m_pSocketOperate == NULL)
    {
        printf("Socket Operator is not defination.");
        return;
    }

#ifdef _SOCKET_DEBUG_
    printf("to get request content\n");
#endif

    char buf[1024];

    int numchars = GetLine(buf, sizeof(buf));

#ifdef _SOCKET_DEBUG_
    printf("request : %s\n", buf);
#endif

    string strResponse = m_pSocketOperate->Response(buf);

#ifdef _SOCKET_DEBUG_
    printf("response : %s\n", strResponse.c_str());
#endif

    int ret = WriteLine(strResponse.c_str(), strResponse.length());

    if (ret == -1)
    {
#ifdef _SOCKET_DEBUG_
        printf("send response failed\n");
#endif
    }
    else
    {
#ifdef _SOCKET_DEBUG_
        printf("send response successful\n");
#endif
    }

    close(m_hAcceptSocket);
}

void SocketServ::error_die(const char *sc)
{
    perror(sc);
}

int SocketServ::GetLine(char* buf, int size)
{
    int n = recv(m_hAcceptSocket, buf, 1024, 0);
    if (n == -1)
    {
        error_die("recv()");
        return -1;
    }

    if (buf[n - 1] == '\0')
    {
        return n;
    }
    else
    {
        buf[n] = '\0';
        return n + 1;
    }
}

int SocketServ::WriteLine(const char* buf, int size)
{
    int ret = send(m_hAcceptSocket, buf, size, 0);
    if (ret == -1)
    {
         error_die("send()");
    }

    return ret;
}
