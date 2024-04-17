#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

int main (int l_param, char * wparam[]){
  int i;
  int tab[20]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};

  void *handle = dlopen("bibl.so", RTLD_LAZY);
  
  if(!handle){
    perror("handle");
    exit(EXIT_FAILURE);
  }

  int (*f1) (int*, int) = dlsym(handle, "sumuj");
  double (*f2) (int, int) = dlsym(handle, "dziel");
/*
1) otworz biblioteke
2) przypisz wskaznikom f1 i f2 adresy funkcji sumuj i dziel z biblioteki
3) stworz Makefile kompilujacy biblioteke 'bibl1' ladowana dynamicznie
   oraz kompilujacy ten program
*/

  printf("%d\n", f1(tab, 20));
  printf("%f\n", f2(10.0, 2.0));
  dlclose(handle);
  return 0;
}
