#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

using namespace std;

enum HTTPType{GET, POST};

#define GET_HEADER_STR "GET /%s?%s HTTP/1.1\r\nHost: %s:%s\r\n User-Agent: Mozilla/5.0 Firefox/38.0\r\n \
                        Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n \
                        Accept-Language: en-US,en;q=0.5\r\n \
                        Accept-Encoding: gzip, deflate\r\n \
                        Connection: keep-alive\r\n \
                        Cache-Control: max-age=0\r\n"
#define POST_HEADER_STR "POST /%s HTTP/1.1\r\nHost: %s:%s\
                        User-Agent: Mozilla/5.0 Firefox/38.0\r\n \
                        Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n \
                        Accept-Language: en-US,en;q=0.5\r\n \
                        Accept-Encoding: gzip, deflate\r\n \
                        Referer: http://%s:%s/\r\n \
                        Connection: keep-alive\r\n \
                        Content-Type: application/x-www-form-urlencoded\r\n \
                        Content-Length: %d\r\n\r\n%s"

char cmd[1024] = {0};
string MakeRequestInfo(string strServerIP, string strServerPort, HTTPType httpType,
                       string strPageName, string strParams)
{
    string strRet;
    switch(httpType)
    {
    case GET:
        sprintf(cmd, GET_HEADER_STR, strPageName.c_str(), strParams.c_str(), strServerIP.c_str(), strServerPort.c_str());
        strRet = cmd;
        break;
    case POST:
        sprintf(cmd, POST_HEADER_STR, strPageName.c_str(), strServerIP.c_str(), strServerPort.c_str(),
                strServerIP.c_str(), strServerPort.c_str(), (int)strParams.length(), strParams.c_str());
        strRet = cmd;
        break;
    default:
        break;
    }

    return strRet;
}

int main()
{
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(4000);
    int len = sizeof(address);

    int result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1)
    {
        perror("Socket Cient");
        exit(1);
    }

    /* Start communicate */

    // Get
    //string str = MakeRequestInfo("127.0.0.1", "4000", GET, "tree", "a=1&b=2&c=3");
    // Post
    string str = MakeRequestInfo("127.0.0.1", "4000", POST, "tree", "a=1&b=2&c=3");
    // Send request
    send(sockfd, str.c_str(), str.length(), 0);

    // Get response
    printf("msg from server = ");
    char ch;
    while (recv(sockfd, &ch, 1, 0) > 0)
    {
        printf("%c", ch);
    }

    printf("\n");


    close(sockfd);
    exit(0);
}


