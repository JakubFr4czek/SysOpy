#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE_NAME "/server_queue"
#define BUFFER_SIZE 1024

int main(){

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

    // Starting parameters of the server queue
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(Message),
        .mq_maxmsg = 10
    };

    //Opening the server queue
     mqd_t descriptor = mq_open(QUEUE_NAME, O_RDWR, S_IRUSR | S_IWUSR, &attr);

    if(descriptor == -1){
        perror("Error while opening queue\n");
        return 1;
    }
    
    char client_queue[BUFFER_SIZE];
    sprintf(client_queue, "/client_queue_%d", getpid());

    Message init_message;
    init_message.msg_type = INIT;
    init_message.buffer = client_queue;

    //mqd_t client_descriptor = mq_open(client_queue, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);

    // Sending init to server
    if(mq_send(descriptor, (char*)&init_message, sizeof(Message), 0) == -1){
        perror("Error while sending init to server\n");
        return 1;
    }

    Message receivedMessage;

    // Waiting for data
    while(1) {

        ssize_t bytes_read = mq_receive(descriptor, (char*)&receivedMessage, sizeof(Message), 0);

        if (bytes_read == -1) {
            perror("mq_receive\n");
            continue;
        }

        printf("Hooray!\n");

    }



}