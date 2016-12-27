#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>

#include "SocketServ.h"
#include "SocketChat.h"

int main(void)
{
    SocketServ socketServ;

    SocketChat socketChat;
    socketServ.SetOperator(&socketChat);

    u_short port = 4000;
    socketServ.StartUp(&port);

    while (1)
    {
        //printf("Wait request ...\n");
        if(socketServ.Accept() == 0)
        {
            //printf("Get request:\n");
            socketServ.AcceptRequest();
        }
    }

    socketServ.Stop();

    return 0;
}
