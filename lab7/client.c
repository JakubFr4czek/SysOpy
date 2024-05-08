#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

#define QUEUE_NAME "/server_queue"
#define BUFFER_SIZE 256

int main(){

    // Possible types of messages,
    // INIT - contains IPC key
    // ID - id of a client
    // DATA - contains any data
    typedef enum {
        INIT,
        ID,
        DATA
    }Msg_type;

    // Object that is being sent during communication
    typedef struct{
        
        Msg_type msg_type;
        int id;
        char buffer[BUFFER_SIZE];

    }Message;

    // Starting parameters of the server queue
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(Message),
        .mq_maxmsg = 10
    };

    //Opening the server queue
     mqd_t server_descriptor = mq_open(QUEUE_NAME, O_RDWR, S_IRUSR | S_IWUSR, &attr);

    if(server_descriptor == -1){
        perror("Error while opening queue\n");
        return 1;
    }
    
    char client_queue[BUFFER_SIZE];
    sprintf(client_queue, "/client_queue_%d", getpid());

    // Opening client queue
    mqd_t client_descriptor = mq_open(client_queue, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);

    // Constructing INIT message
    Message init_message;
    init_message.id = -1; // Client has no id yet
    init_message.msg_type = INIT;
    memcpy(init_message.buffer, client_queue, sizeof(client_queue)); // It needs to be copied manually, otherwise it results in segementation fault

    // Sending init to the server
    if(mq_send(server_descriptor, (char*)&init_message, sizeof(init_message), 0) == -1){
        perror("Error while sending init to server\n");
        return 1;
    }

    // Receiving an id from the server
    Message id_message;

    ssize_t bytes_read = mq_receive(client_descriptor, (char*)&id_message, sizeof(id_message), 0);

    if (bytes_read == -1) {

        perror("mq_receive\n");
            
    }

    int clientID = id_message.id;

    pid_t pid = fork();

    while(1){

        if(pid == 0){ // Receiving messages from the server

            Message message;

            ssize_t bytes_read = mq_receive(client_descriptor, (char*)&message, sizeof(message), 0);

            if (bytes_read == -1) {

                perror("mq_receive\n");
                    
            }

            switch(message.msg_type){

                case DATA:
                    printf("Message from client %d: %s\n", message.id, message.buffer);
                    break;
                default:
                    perror("Wrong message type!");
                    break;

            }

            
        }else{ // Sending messages to the server

            char input[BUFFER_SIZE];
            scanf("%256[^\n]%*c", input);

            Message message;
            message.msg_type = DATA;
            message.id = clientID;
            memcpy(message.buffer, input, sizeof(input));

            if(mq_send(server_descriptor, (char*)&message, sizeof(message), 0) == -1){
                perror("Error while sending init to server\n");
                return 1;
            }

        }


    }
    
    





}