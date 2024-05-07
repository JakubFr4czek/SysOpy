#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#define QUEUE_NAME "/server_queue"
#define BUFFER_SIZE 1024

int main() {
    
    // Possible types of messages,
    // INIT - contains IPC key
    // DATA - contains any data
    typedef enum {
        INIT,
        DATA
    }Msg_type;

    // Object that is being sent during communication
    typedef struct{
        
        Msg_type msg_type;
        char* buffer;

    }Message;

    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(Message),
        .mq_maxmsg = 10
    };

    mqd_t descriptor = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);

    if (descriptor == -1) {
        perror("mq_open\n");
        return 1;
    }

    Message receivedMessage;

    while(1) {

        ssize_t bytes_read = mq_receive(descriptor, (char*)&receivedMessage, sizeof(Message), 0);

        if (bytes_read == -1) {
            perror("mq_receive\n");
            continue;
        }

        switch(receivedMessage.msg_type){

            case INIT:
                printf("INIT\n");
                printf("%s\n",receivedMessage.buffer);
                break;
            case DATA:
                printf("DATA\n");
                break;
            default:
                perror("Wrong message type\n");
                break;

        }

    }

    if(mq_close(descriptor) == -1) {
        perror("mq_close\n");
        return 1;
    }

    if(mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink\n");
        return 1;
    }
    
    return 0;
}
