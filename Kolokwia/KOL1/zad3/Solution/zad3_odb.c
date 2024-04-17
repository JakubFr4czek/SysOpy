#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


struct pak{
 int i;
 char lit;
} ob1;

int main (int lpar, char *tab[]){
 
// 1) utworzyc potok nazwany 'potok1'

  mkfifo("potok", 0666);

  

  while (1){

    int desc = open("potok", O_RDONLY);
    read(desc, &ob1, sizeof(ob1));
    printf("%d %c", ob1.i, ob1.lit); fflush(stdout);
    
    close(desc);
  }

  

  return 0;
}
