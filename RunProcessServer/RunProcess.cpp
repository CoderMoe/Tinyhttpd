#include "RunProcess.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define STDIN   0
#define STDOUT  1
#define STDERR  2

string RunProcess::Response(string strRequest)
{
    return Run();
}

string RunProcess::Error()
{
    return "Error Error ...";
}

string RunProcess::Run()
{
   /* 建立管道*/
   int run_output[2];
   int run_input[2];
   printf("----> create pipe\n");

   if (pipe(run_output) < 0)
   {
       return Error();
   }

   /*建立管道*/
   if (pipe(run_input) < 0)
   {
       return Error();
   }

   pid_t pid = fork();
   if (pid < 0)
   {
       perror("fork");
       return Error();
   }

   string strRet = "";

   if (pid == 0)  /* child */
   {
       printf("--0--> This is child process\n");

       //char query_env[255];
       //char length_env[255];

       printf("--0--> start redirect\n");

       /* 把 STDOUT 重定向到 cgi_output 的写入端 */
       dup2(run_output[1], STDOUT);

       /* 把 STDIN 重定向到 cgi_input 的读取端 */
       dup2(run_input[0], STDIN);

       close(run_output[0]);
       close(run_input[1]);

       /*用 execl 运行*/
       string strPath = "../Run/run";

       int ret = execlp(strPath.c_str(), strPath.c_str(), "ab", "bc",  NULL);

       exit(ret);
   }
   else
   {
        /* parent */

        printf("--1--> This is parent process\n");

        /* 关闭 cgi_input 的读取端 和 cgi_output 的写入端 */
        close(run_output[1]);
        close(run_input[0]);

        sprintf(m_pBuf, "abcd\nabc\nabcd\ndafdd\n", 20);
        m_pBuf[20] = '\0';

        printf("--1--> m_pBuf : %s\n", m_pBuf);

        for (int i = 0; i < 21; i++)
        {
            write(run_input[1], &m_pBuf[i], 1);
        }

        /*读取 cgi_output 的管道输出到客户端，该管道输入是 STDOUT */
        char c = 'A';
        m_pBuf[1] = 0;
        while (read(run_output[0], &c, 1) > 0)
        {
            //printf("--1--> c : %c\n", c);
            m_pBuf[0] = c;
            strRet.append(m_pBuf);
        }

        /*关闭管道*/
        close(run_output[0]);
        close(run_input[1]);

       /*等待子进程*/
       printf("--1--> wait child process back\n");

       int status;
       waitpid(pid, &status, 0);

       printf("--1--> child process exit code : %d\n", status);
   }

   //printf("----> strRet : %s\n", strRet.c_str());

   return strRet;
}
