#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("Para %d : %s\n", i, argv[i]);
    }

    char pBuf[100];

    cin >> pBuf;

    printf("%s\n", pBuf);

    return 0;
}


