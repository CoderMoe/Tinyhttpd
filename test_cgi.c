#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Running cgi with execlp\n");
    
    execlp("htdocs/color.cgi","htdocs/color.cgi", "color=red", NULL);
    
    printf("Done.\n");
    
    return 0;
}