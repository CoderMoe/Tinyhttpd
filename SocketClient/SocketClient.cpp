#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

using namespace std;

int main()
{
    string str = "Hello World!";

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


