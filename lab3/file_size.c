#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


int main(int argc, char **argv) {

    if(argc != 2){
        printf("Wrong number of arguments!\n");
        return 1;
    }

    const char* path = argv[1];

    printf("\n");

    DIR *dir = opendir(path);

    if(dir == NULL){

        printf("Wrong directory path!\n");
        return 2;

    }

    long long files_size = 0;

    struct dirent *file;

    while( (file = readdir(dir)) != NULL){

        struct stat file_stat;

        stat(file->d_name, &file_stat);

        if(!S_ISDIR(file_stat.st_mode)) {

            printf("%-20s %ld bytes\n", file->d_name, (long)file_stat.st_size);
            files_size += file_stat.st_size;

        }

    }

    printf("\nAll files size: %lld bytes\n", files_size);

    closedir(dir);

    return 0;
}
