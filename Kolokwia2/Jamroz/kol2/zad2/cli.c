#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<string.h>
#include<errno.h>
#include<inttypes.h>
#include<stdbool.h>

#define L_PYTAN 10

static bool str_to_uint16(const char *str, uint16_t *res){

    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *res = (uint16_t) val;
    return true;

}

int main(void){
 int status,gniazdo,i;
 struct sockaddr_in ser,cli;
 char buf[200];
 char pytanie[]="abccbahhff";

 gniazdo = socket(AF_INET,SOCK_STREAM,0);
 if (gniazdo==-1) {printf("blad socket\n"); return 0;}

 memset(&ser, 0, sizeof(ser));
 //nawiaz polaczenie z IP 127.0.0.1 i usluga na porcie takim samym jak ustaliles w serwerze, zwroc status
 //...

    struct in_addr ipv4;

  inet_pton(AF_INET, "127.0.0.1", &ipv4);

  uint16_t port;
  str_to_uint16("8000", &port);

  ser.sin_family = AF_INET;
  ser.sin_port = port;
  ser.sin_addr = ipv4;

    status = connect(gniazdo, (struct sockaddr*)&ser, sizeof(ser));

 if (status<0) {printf("blad 01\n"); return 0;}
 for (i=0; i<L_PYTAN; i++){
  status = write(gniazdo, pytanie+i, 1);
  //odczytaj dane otrzymane z sieci, wpisz do tablicy 'buf' i wyświetl na standardowym wyjściu (ekranie)
  //...
  recv(gniazdo, &buf, sizeof(buf), 0);
  printf("%s\n", buf);

 }
 printf ("\n");

close(gniazdo);  
printf("KONIEC DZIALANIA KLIENTA\n");
return 0;
}

