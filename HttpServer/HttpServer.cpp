#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>

#include "SocketServ.h"
#include "SocketHttp.h"





/**********************************************************************/
/* Execute a CGI script.  Will need to set environment variables as
 * appropriate.
 * Parameters: client socket descriptor
 *             path to the CGI script */
/**********************************************************************/
//void execute_cgi(int client, const char *path,
//        const char *method, const char *query_string)
//{
//    char buf[1024];
//    int cgi_output[2];
//    int cgi_input[2];
//    pid_t pid;
//    int status;
//    int i;
//    char c;
//    int numchars = 1;
//    int content_length = -1;

//    printf("\n----> Start execute_cgi()\n");
//    printf("path : %s\n", path);
//    printf("method : %s\n", method);
//    printf("query_string : %s\n", query_string);

//    buf[0] = 'A';
//    buf[1] = '\0';
//    if (strcasecmp(method, "GET") == 0)
//    {
//        /*把所有的 HTTP header 读取并丢弃*/
//        while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
//        {
//            numchars = get_line(client, buf, sizeof(buf));
//        }
//    }
//    else if (strcasecmp(method, "POST") == 0) /*POST*/
//    {
//        /* 对 POST 的 HTTP 请求中找出 content_length */
//        numchars = get_line(client, buf, sizeof(buf));
//        while ((numchars > 0) && strcmp("\n", buf))
//        {
//            buf[15] = '\0';
//            if (strcasecmp(buf, "Content-Length:") == 0)
//                content_length = atoi(&(buf[16]));
//            numchars = get_line(client, buf, sizeof(buf));
//        }

//        if (content_length == -1)
//        {
//             /*错误请求*/
//            bad_request(client);
//            return;
//        }
//    }
//    else/*HEAD or other*/
//    {
//        // Do nothing
//    }

//    /* 建立管道*/
//    printf("----> create pipe\n");
//    if (pipe(cgi_output) < 0)
//    {
//        /*错误处理*/
//        cannot_execute(client);
//        return;
//    }

//    /*建立管道*/
//    if (pipe(cgi_input) < 0)
//    {
//        /*错误处理*/
//        cannot_execute(client);
//        return;
//    }

//    if ((pid = fork()) < 0 )
//    {
//        /*错误处理*/
//        cannot_execute(client);
//        return;
//    }

//    /* HTTP 状态码 200 */
//    //printf("----> return html page\n");
//    sprintf(buf, "HTTP/1.0 200 OK\r\n");
//    send(client, buf, strlen(buf), 0);
//    if (pid == 0)  /* child: CGI script */
//    {
//        printf("--0--> This is child process\n");
        
//        char meth_env[255];
//        char query_env[255];
//        char length_env[255];

//        printf("--0--> start redirect\n");
        
//        /* 把 STDOUT 重定向到 cgi_output 的写入端 */
//        dup2(cgi_output[1], STDOUT);
        
//        /* 把 STDIN 重定向到 cgi_input 的读取端 */
//        dup2(cgi_input[0], STDIN);
        
//        close(cgi_output[0]);
//        close(cgi_input[1]);
        
//        /*设置 request_method 的环境变量*/
//        sprintf(meth_env, "REQUEST_METHOD=%s", method);
//        putenv(meth_env);
//        if (strcasecmp(method, "GET") == 0)
//        {
//            /*设置 query_string 的环境变量*/
//            sprintf(query_env, "QUERY_STRING=%s", query_string);
//            putenv(query_env);
//        }
//        else
//        {   /* POST */
//            /*设置 content_length 的环境变量*/
//            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
//            putenv(length_env);
//        }

//        /*用 execl 运行 cgi 程序*/
//        printf("--0--> exec path %s: \n", path);
//        int ret = execl(path, path, NULL);
        
//        exit(ret);
//    }
//    else
//    {
//        /* parent */
        
//         printf("--1--> This is parent process\n");
         
//        /* 关闭 cgi_input 的读取端 和 cgi_output 的写入端 */
//        close(cgi_output[1]);
//        close(cgi_input[0]);
        
//        if (strcasecmp(method, "POST") == 0)
//        {
//             /*接收 POST 过来的数据*/
//            for (i = 0; i < content_length; i++)
//            {
//                recv(client, &c, 1, 0);
//                /*把 POST 数据写入 cgi_input，现在重定向到 STDIN */
//                write(cgi_input[1], &c, 1);
//            }
//        }
        
//        /*读取 cgi_output 的管道输出到客户端，该管道输入是 STDOUT */
//        while (read(cgi_output[0], &c, 1) > 0)
//        {
//            send(client, &c, 1, 0);
//        }
        
//        /*关闭管道*/
//        close(cgi_output[0]);
//        close(cgi_input[1]);
        
//        /*等待子进程*/
//        printf("--1--> wait child process back\n");
//        waitpid(pid, &status, 0);
        
//        printf("--1--> child process exit code : %d\n", status);
//    }
    
//    printf("----> end execute_cgi()\n");
//}

/**********************************************************************/

int main(void)
{
    SocketServ socketServ;

    SocketHTTP socketHTTP;
    socketServ.SetOperator(&socketHTTP);

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
