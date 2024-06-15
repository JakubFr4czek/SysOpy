#define _GNU_SOURCE    
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define KEY  "./queuekey"

struct my_msgbuf {
    long mtype;
    int value;
};

int main() {
        sleep(1);
        int val = 0;


	/**********************************
	Otworz kolejke systemu V "reprezentowana" przez KEY
	**********************************/

	key_t k = ftok(KEY, 1);
	int q = msgget(k, 0664);

	struct my_msgbuf buf;

	while(1)
 	{	
	    /**********************************
	    Odczytaj zapisane w kolejce wartosci i przypisz je do zmiennej val
	    obowiazuja funkcje systemu V
	    ************************************/
	   	msgrcv(q, (void*)&buf, sizeof(buf.value), 0, 0);

		val = buf.value;

        printf("%d square is: %d\n", val, val*val);

		break;
 
	}

	/*******************************
	posprzataj
	********************************/
	msgctl(q, IPC_RMID, NULL);

     return 0;
   }
