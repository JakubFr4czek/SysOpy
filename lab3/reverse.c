#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

/* 
    How to use:

    gcc "reverse.c" -o reverse

    reverse <input_filename> <outptu_filename> <buffer_size>

*/

void reverse_array(void *array, int size){

    if(size < 2) return;

    char* arr = (char*)array; //char has size of 1 byte

    for(int i = 0; i < size / 2; i += 1){

        //printf("%d %c %c\n",i, arr[i], arr[size - 1 - i]);

        int temp = arr[size - 1 - i];
        arr[size - 1 - i] = arr[i];
        arr[i] = temp;

    }

}

int main(int argc, char **argv){

    clock_t begin = clock();

    //argv -> program_name, input_filename, output_filename, file_size

    int buffer_size = atoi(argv[3]);

    if(argc != 4){

        printf("%d is a worng number of arguments!", argc);
        return 1;

    }

    //input file

    FILE *input = fopen(argv[1], "r");

    if(!input){

        printf("Input file could not be opened!\n");
        return 2;

    }; 

    //output file

    FILE *output = fopen(argv[2], "a");

    if(!output){

        printf("Output file could not be opened!\n");
        return 3;

    }

    //Estimating file size
    fseek(input, 0, SEEK_END);
    int to_be_read = ftell(input); //variable will be use to tell how much bytes are left to be read

    //Allocating memory for the buffer
    void* buffer = (void*)malloc(buffer_size);

    //Determining if input file is bigger than the buffer
    int actual_buffer_size = buffer_size <= to_be_read?buffer_size:to_be_read;
    int pos_in_file = -actual_buffer_size; //variable will be used to determine pointer position in file

    while(to_be_read > 0){

        //moveing pointer to desired position
        int fseek_status = fseek(input, pos_in_file, SEEK_END);

        if(fseek_status != 0){

            printf("Wrong file pointer position!");
            return 4;

        }

        //reading bytes to a buffer
        int bytes_read = fread(buffer, 1, actual_buffer_size, input);

        if(bytes_read != actual_buffer_size){

            printf("Bytes read does not match actuall buffer size!");
            return 5;

        }

        //updating how many bytes are left to be read
        to_be_read -= bytes_read;

        //reversing buffer
        reverse_array(buffer, bytes_read);

        //writing reversed buffer to an output file
        int fwrite_status = fwrite(buffer, 1, bytes_read, output);

        if(fwrite_status != actual_buffer_size){

            printf("Difference between read and write!");
            return 6;

        }

        //Determining if whats left to be read is bigger than buffer
        actual_buffer_size = buffer_size <= to_be_read?buffer_size:to_be_read;         
        pos_in_file -= actual_buffer_size;

    }

    //closing files
    fclose(input);
    fclose(output);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%s took %f seconds to execute\n", argv[0], time_spent);

}