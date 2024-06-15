#define _GNU_SOURCE    
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define KEY "./queuekey"

struct my_msgbuf {
    long mtype;
    int value;
};

int main(int argc, char* argv[])
{

 if(argc !=2){
   printf("Not a suitable number of program parameters\n");
   return(1);
 }

 /******************************************************
 Utworz kolejke komunikatow systemu V "reprezentowana" przez KEY
 Wyslij do niej wartosc przekazana jako parametr wywolania programu
 Obowiazuja funkcje systemu V
 ******************************************************/
  key_t k = ftok(KEY, 1);
    int q = msgget(k, IPC_CREAT | 0644);

    int val = atoi(argv[1]);
    
    struct my_msgbuf buf = {
      .mtype = 1,
      .value = val
    };
    
    if(msgsnd(q, (const void*)&buf, sizeof(int), 0) < 0){
        perror("msgsnd");
    }

  return 0;
}



