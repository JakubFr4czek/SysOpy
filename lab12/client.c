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
#include<pthread.h>
#include"server_config.h"

#define INPUT_BUFFER 1024

bool connected = false;
int server_fd;
char* client_name;

// Przygotowanie struktury przechowującej adres ip i port
struct sockaddr_in socket_addr_in;

// Zmienna przechowująca rozmiar powyższej struktury
socklen_t socket_addr_len;

// Wątek odpowiedzialny za wysyłanie ALIVE
pthread_t alive_thread;

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

    message_t stop_message;
    stop_message.type = STOP;
    strcpy(stop_message.id, client_name);
    
    if(sendto(server_fd, &stop_message, sizeof(stop_message), 0,
                    (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) < 0) perror("send\n");


    close(server_fd);

    exit(0);

}

// ALIVE jest wysyłane co 2 sekundy, a timeout następuje po 10
void* send_alive(){

    // Czas od ostatniego wysłania wiadomości ALIVE
    clock_t prev_time = clock();

    while(true){

        if((clock() - prev_time) / CLOCKS_PER_SEC > 2){

            message_t alive_message;
            alive_message.type = ALIVE;
            strcpy(alive_message.id, client_name);

            if(sendto(server_fd, &alive_message, sizeof(alive_message), 0,
                        (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) < 0) perror("send\n");

            prev_time = clock();

        }
    
    }

    pthread_exit(0);

    return NULL;
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
    if(!str_to_uint16(argv[2], &port)) perror("str_to_uint16\n");

    client_name = argv[3];

    // Uzupełnienie struktury przechowującej adres ip i port
    socket_addr_in.sin_family = AF_INET;
    socket_addr_in.sin_port = port;
    socket_addr_in.sin_addr = socket_in_addr;

    // Uzupełnienie zmiennej przechowująca rozmiar powyższej struktury
    socket_addr_len = sizeof(socket_addr_in);

    // Tworzenie socketu
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    // Wysyłanie inita
    message_t init_message;
    init_message.type = INIT;
    strcpy(init_message.id, client_name);

    if(sendto(server_fd, &init_message, sizeof(init_message), 0,
          (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) < 0) perror("send\n");

    // Tworzenie wątku odpowiedzialnego za wysyłanie ALIVE
    pthread_create(&alive_thread, NULL, send_alive, NULL);

    // Tworzenie procesu potomnego
    pid_t process_pid = fork();

    // Główna pętla
    while(true){

        // bufor na komendy
        char input[INPUT_BUFFER];

        if(process_pid == 0){ // child - wysyłanie

            // Odczytywanie komendy
            scanf("%s", input);
            
            if(strcmp(input, "LIST") == 0){
                
                message_t list_message;
                list_message.type = LIST;
                strcpy(list_message.id, client_name);
                
                if(sendto(server_fd, &list_message, sizeof(list_message), 0,
                    (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) < 0) perror("send\n");

            }else if(strcmp(input, "2ALL") == 0){
                
                // Bufor na treść wiadomości
                char toallmessage[INPUT_BUFFER];
                scanf("%s", toallmessage);

                message_t toall_message;
                toall_message.type = TOALL;
                strcpy(toall_message.id, client_name);
                strcpy(toall_message.data.msg.message, toallmessage);

                if(sendto(server_fd, &toall_message, sizeof(toall_message), 0,
                    (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) < 0) perror("send\n");

            }else if(strcmp(input, "2ONE") == 0){
                
                // Bufor na adresata i treść wiadomości
                char receiver_id[ID_LENGTH];
                char msg[INPUT_BUFFER];
                scanf("%s %s", receiver_id, msg);

                message_t toone_message;
                toone_message.type = TOONE;
                strcpy(toone_message.id, client_name);
                strcpy(toone_message.data.private_msg.receiver_id, receiver_id);
                strcpy(toone_message.data.private_msg.message, msg);

                if(sendto(server_fd, &toone_message, sizeof(toone_message), 0,
                    (struct sockaddr*)&socket_addr_in, sizeof(socket_addr_in)) < 0) perror("send\n");
            }

        }else{ // parent - odbieranie

            message_t message;

            if(recvfrom(server_fd, &message, sizeof(message), MSG_DONTWAIT, (struct sockaddr*)&socket_addr_in, &socket_addr_len) > 0){

                switch(message.type){

                    case LIST:

                        for(int i = 0; i < MAX_CLIENTS; i += 1){

                            if(message.data.list.ids[i][0] != '\0')
                                printf("~ %s\n", message.data.list.ids[i]);

                        }

                        break;

                    case TOALL:
                        
                        printf("%s | 2ALL | %s: %s\n", message.data.msg.send_time, message.id, message.data.msg.message);

                        break;

                    case TOONE:
                        
                        printf("%s | 2ONE | %s: %s\n", message.data.private_msg.send_time, message.id, message.data.private_msg.message);

                        break;

                    case STOP:
                        kill(getpid(), SIGINT);
                        break;

                    default:
                        printf("Unknown command!\n");
                        break;

                }

            }

        }

    }

}