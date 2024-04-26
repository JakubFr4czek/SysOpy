#include <stdio.h>
#include <mqueue.h>

int main(){

    mqd_t decriptor = mq_open("/queue", O_RDONLY);

    if(decriptor == -1){
        perror("Error while opening queue");
        return 1;
    }

    ssize_t mq_receive(mqd_t mqdes, char *ptr, size_t len, unsigned int *priop);


    if(mq_close(decriptor) == -1){
        perror("Error while closing queue");
        return 1;
    }

    if(mq_unlink("/queue") == -1){
        perror("Error while unlinking queue");
        return 1;
    }
    
}