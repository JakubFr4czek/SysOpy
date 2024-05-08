#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_NAME "/server_queue"
#define BUFFER_SIZE 256
#define MAX_CLIENTS 15

int main() {
    
    printf("Server starting...\n");

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

    int registered_clients = 0;
    mqd_t client_queues[MAX_CLIENTS];

    // Setting attributes of the queue
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(Message),
        .mq_maxmsg = 10
    };

    // Opening server queue
    mqd_t server_descriptor = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);

    if (server_descriptor == -1) {
        perror("mq_open\n");
        return 1;
    }

    printf("Server started\n");

    // Holds a received message from the client
    Message receivedMessage;

    while(1) {
        
        // Receiving a message from the clients
        if(mq_receive(server_descriptor, (char*)&receivedMessage, sizeof(receivedMessage), 0) == -1){

            perror("mq_receive\n");
            continue;
        }

        switch(receivedMessage.msg_type){

            case INIT: //A new client wants to join the server

                printf("A new client joined the server\n");
                
                // Registering a client
                if(registered_clients < MAX_CLIENTS){
                    
                    // Adding client queue descriptior to the list
                    client_queues[registered_clients] = mq_open(receivedMessage.buffer, O_RDWR, S_IRUSR | S_IWUSR, &attr);

                    // Creating a message containing client's new id
                    Message clientID;
                    clientID.msg_type = ID;
                    clientID.id = registered_clients;
    
                    // Sending a client their id
                    if(mq_send(client_queues[registered_clients], (char*)&clientID, sizeof(clientID), 0) == -1){
                        perror("Error while sending id to the client\n");
                        return 1;
                    }

                    registered_clients += 1;

                }else{

                    perror("Maximum number of clients reached!");
                    continue;

                }
                break;

            case DATA: //A client sends data to the server

                // Sending data to all clients, but client with recivedMessage.id
                for(int i = 0; i < registered_clients; i += 1){

                    if(i != receivedMessage.id){

                        if(mq_send(client_queues[i], (char*)&receivedMessage, sizeof(receivedMessage), 0) == -1){
                        perror("Error while sending message to a client\n");
                        return 1;
                        }

                    }

                }

                break;

            default: // Wrong message type or no message type

                perror("Wrong message type\n");
                break;

        }

    }

    // Closing server queue
    if(mq_close(server_descriptor) == -1) {
        perror("mq_close\n");
        return 1;
    }

    // Destroying server queue
    if(mq_unlink(QUEUE_NAME) == -1) { // NEEDS TO BE CALLED AFTER CHANGING PARAMETER OF THE QUEUE!
        perror("mq_unlink\n");
        return 1;
    }
    
    return 0;
}
