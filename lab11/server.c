#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<inttypes.h> /* strtoimax */
#include<stdbool.h>
#include<errno.h>
#include<stddef.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<stdlib.h>
#include"server_config.h"

int server_fd;
int clients_fd[MAX_CLIENTS];
char clients_ids[MAX_CLIENTS][ID_LENGTH];
int number_of_clients = 0;

// Funkcja pożyczona z internetu
// https://stackoverflow.com/questions/20019786/safe-and-portable-way-to-convert-a-char-to-uint16-t
static bool str_to_uint16(const char *str, uint16_t *res){

    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *res = (uint16_t) val;
    return true;

}


void sigint_handler(int signo){

    for(int i = 0; i < number_of_clients; i += 1){

        shutdown(clients_fd[i], SHUT_RDWR);
        close(clients_fd[i]);

    }

    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);

    exit(0);

}


int main(int argc, char *argv[]){

    if (argc != 3){
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }    

    signal(SIGINT, sigint_handler);

    // Przygotowanie struktury przechowującej adres ip
    struct in_addr socket_in_addr;

    // Rzutowanie adresu ip z postaci tekstowej na binarną
    inet_pton(AF_INET, argv[1], &socket_in_addr);

    // Rzutowanie portu z postaci tekstowej na binarną
    uint16_t port;
    if(!str_to_uint16(argv[2], &port)){
        perror("str_to_uint16\n");
        return 1;
    }

    // Przygotowanie struktury przechowującej adres ip i port
    struct sockaddr_in socket_addr_in;
    socket_addr_in.sin_family = AF_INET;
    socket_addr_in.sin_port = port;
    socket_addr_in.sin_addr = socket_in_addr;

    // Tworzenie socketu
    server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); // SOCK_NONBLOCK - żeby accept się nie blokował

    // Bindowanie socketu
    if(bind(server_fd, (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) < 0){
        perror("bind\n");
    }

    // Rozpoczynanie nasłuchiwania
    if(listen(server_fd, MAX_CLIENTS) < 0){
        perror("listen\n");
    }

    message_t message;

    while(true){

        int client_fd = accept(server_fd, NULL, 0);

        if(client_fd > 0){

            if(number_of_clients + 1 < MAX_CLIENTS){

                clients_fd[number_of_clients] = client_fd;
                number_of_clients++;

            }

            else{

                printf("Too many clients!\n");

            }

        }

        for(int i = 0; i < number_of_clients; i += 1){

            if(recv(clients_fd[i], &message, sizeof(message_t), MSG_DONTWAIT) > 0){

                switch(message.type){

                    case INIT:
                        printf("Client %s has joined the server!\n", message.id);
                        strcpy(clients_ids[i], message.id);
                        break;

                    case LIST:
                        printf("LIST\n");
                        break;

                    case TOALL:
                        printf("TOALL\n");
                        break;

                    case TOONE:
                        printf("TOONE\n");
                        break;

                    case STOP:
                        printf("Client %s has left the server\n", message.id);
                        break;

                    case ALIVE:
                        printf("ALIVE\n");
                        break;

                    default:
                        printf("Unknown message type\n");
                        break;

                }

            }

        }

    }

}