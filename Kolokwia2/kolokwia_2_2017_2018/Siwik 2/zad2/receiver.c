#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define UNIX_PATH_MAX 108
#define SOCK_PATH "sock_path"


int main() {
    int fd = -1;

    /*********************************************
    Utworz socket domeny unixowej typu datagramowego
    Utworz strukture adresowa ustawiajac adres/sciezke komunikacji na SOCK_PATH
    Zbinduj socket z adresem/sciezka SOCK_PATH
    **********************************************/

   fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
        .sun_path = SOCK_PATH
    };

    socklen_t addr_len = sizeof(addr);

    bind(fd, (struct sockaddr*)&addr, addr_len);

    char buf[20];
    if(read(fd, buf, 20) == -1)
        perror("Error receiving message");
    int val = atoi(buf);
    printf("%d square is: %d\n",val,val*val);

    
    /***************************
    Posprzataj po sockecie
    ****************************/

    shutdown(fd, SHUT_RDWR);
    close(fd);

    return 0;
}

