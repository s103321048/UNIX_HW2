#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if(2 != argc)
    {
        printf("Usage :  \n");
        return 1;
    }

    errno = 0;
    fd = open(argv[1],O_RDONLY|O_CREAT,S_IRWXU);

    if(-1 == fd)
    {
        printf("open() failed with error [%s]\n",strerror(errno));
        return 1;
    }
    else
    {
        printf("open() Successful.\n");
    }

    return 0;
}
