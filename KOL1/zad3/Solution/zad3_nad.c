#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

struct pak{
 int i;
 char lit;
} ob1;
/*program przy kazdym uzyciu klawiszy ctrl-c (SIGINT) ma wyslac obiekt o1 przez
potok nazwany 'potok1'*/


void send_data(){

  int desc = open("potok", O_WRONLY);
  write(desc, &ob1, sizeof(ob1));
  close(desc);

}

int main (int lpar, char *tab[]){

  signal(SIGTSTP, send_data);

  ob1.i = 5;
  ob1.lit = 'Y';

  while(1){
    pause();
  }

  return 0;
}
