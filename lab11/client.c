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
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include"server_config.h"

#define INPUT_BUFFER 1024

bool connected = false;
int server_fd;
char* client_name;

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

    if(connected)
        close(server_fd);

    message_t stop_message;
    stop_message.type = STOP;
    strcpy(stop_message.id, client_name);
    
    exit(0);

}

int main(int argc, char *argv[]){

    if (argc != 4){
        fprintf(stderr, "Usage: %s <ip> <port> <client_name>\n", argv[0]);
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

    client_name = argv[3];

    // Przygotowanie struktury przechowującej adres ip i port
    struct sockaddr_in socket_addr_in;
    socket_addr_in.sin_family = AF_INET;
    socket_addr_in.sin_port = port;
    socket_addr_in.sin_addr = socket_in_addr;

    // Tworzenie socketu
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Łączenie z serwerem
    if(connect(server_fd, (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) > 0){
        connected = true;
    }else{
        perror("connect\n");
    }

    // Wysyłanie inita
    message_t init_message;
    init_message.type = INIT;
    strcpy(init_message.id, client_name);

    if(send(server_fd, &init_message, sizeof(init_message), MSG_DONTWAIT) < 0){
        perror("send\n");
    }

    pid_t process_pid = fork();

    while(true){

        char input[INPUT_BUFFER];

        if(process_pid == 0){ // child - wysyłanie

            scanf("%s", input);

            if(strncmp(input, "LIST", 4) == 0){
                printf("LISTUJEMY\n");
            }else if(strncmp(input, "TOALL", 5) == 0){
                printf("TOALLUJEMY\n");
            }else if(strncmp(input, "TOONE", 5) == 0){
                printf("TOONEUJEMY\n");
            }else if(strncmp(input, "STOP", 4) == 0){
                printf("STOPUJEMY\n");
            }

            

        }else{ // parent - odbieranie

            message_t message;
            if(recv(server_fd, &message, sizeof(message), MSG_DONTWAIT) > 0){

                switch(message.type){

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
                        printf("STOP\n");
                        break;

                    case ALIVE:
                        printf("ALIVE\n");
                        break;

                    default:
                        break;

                }

            }

        }

    }

}