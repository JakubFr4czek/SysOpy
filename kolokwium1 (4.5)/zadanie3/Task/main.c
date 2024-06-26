#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char* argv[])
{

 if(argc !=2){
    printf("Not a suitable number of program parameters\n");
    exit(1);
 }

 int toChildFD[2];
 int toParentFD[2];

 pipe(toChildFD);
 pipe(toParentFD);

 int val1,val2,val3 = 0;

 pid_t pid;

   if((pid = fork()) == 0) {

    //odczytaj z potoku nienazwanego wartosc przekazana przez proces macierzysty i zapisz w zmiennej val2
   if(close(toChildFD[1]) == -1){ //Zamykam zapis dla potomka
      
      perror("Close error");
      exit(EXIT_FAILURE);

   }


   if(read(toChildFD[0], &val2, sizeof(val2)) == -1){

      perror("Read error");
      exit(EXIT_FAILURE);

   }
   
   val2 = val2 * val2;

   if(close(toChildFD[0]) == -1){
         
      perror("Close error");
      exit(EXIT_FAILURE);

   }

    //wyslij potokiem nienazwanym val2 do procesu macierzysego

   if(close(toParentFD[0]) == -1){ //Zamykam odczyt dla potomka   

      perror("Close error");
      exit(EXIT_FAILURE);

   }

   if(write(toParentFD[1], &val2, sizeof(val2)) == -1){

      perror("Write error");
      exit(EXIT_FAILURE);  

   }

   close(toParentFD[1]);

   }else {

     val1 = atoi(argv[1]);

    //wyslij val1 potokiem nienazwanym do procesu potomnego

   if(close(toChildFD[0]) == -1){ //Zamykam odczyt dla macierzystego

      perror("Close error");
      exit(EXIT_FAILURE);

   }
   if(write(toChildFD[1], &val1, sizeof(val1)) == -1){

      perror("Write error");
      exit(EXIT_FAILURE);

   }

   if(close(toChildFD[1]) == -1){    

      perror("Close error");
      exit(EXIT_FAILURE); 

   } 

   sleep(1);

   //odczytaj z potoku nienazwanego wartosc przekazana przez proces potomny i zapisz w zmiennej val3

   close(toParentFD[1]); //Zamykam zapis dla macierzystego

   if(read(toParentFD[0], &val3, sizeof(val3)) == -1){

      perror("Read error");
      exit(EXIT_FAILURE);

   }

   if(close(toParentFD[0]) == -1){
         
      perror("Close error");
      exit(EXIT_FAILURE); 
   }

   printf("%d square is: %d\n",val1, val3);
 }
 return 0;
}
