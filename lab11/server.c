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
#include<string.h>
#include"server_config.h"

#define TIME_BUFFER_SIZE 80

int server_fd;
int clients_fd[MAX_CLIENTS];
char clients_ids[MAX_CLIENTS][ID_LENGTH];
clock_t clients_times[MAX_CLIENTS]; 
int number_of_clients = 0;

// Bufor przechowujący czas;
char time_buffer[TIME_BUFFER_SIZE];

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


void prepare_clients_arr(){

    for(int i = 0; i < MAX_CLIENTS; i += 1){

        clients_fd[i] = -1;
        clients_ids[i][0] = '\0';
        clients_times[i] = -1;

    }

}

// Wysyłanie informacji o rozłączeniu do klienta
void disconnect_client(int client_number){

    if(clients_fd[client_number] != -1){

    message_t stop_message;
    stop_message.type = STOP;
    if(send(clients_fd[client_number], &stop_message, sizeof(stop_message), MSG_DONTWAIT) < 0) perror("send\n");

    shutdown(clients_fd[client_number], SHUT_RDWR);
    close(clients_fd[client_number]);

    }else perror("disconnect_client\n");

}

// Uwuwanie danych klienta
void remove_client(int client_number){

    if(clients_fd[client_number] != -1){

    strcpy(clients_ids[client_number], "\0"); // Usuwanie id klienta
    close(clients_fd[client_number]);
    clients_fd[client_number] = -1;
    number_of_clients -= 1;

    }else perror("remove_client\n");

}

void sigint_handler(int signo){

    // Powiadamianie klienta o rozłączeniu
    for(int i = 0; i < number_of_clients; i += 1){

        disconnect_client(i); // Musi być wykonane przed remove_client
        remove_client(i);

    }

    // Zamykanie serwera
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);

    exit(0);

}

void check_for_timeout(){

    for(int i = 0; i < MAX_CLIENTS; i += 1){

        if(clients_fd[i] != -1 && (clock() - clients_times[i]) / CLOCKS_PER_SEC > 10){

            printf("%s | Client %s timed out\n", time_buffer, clients_ids[i]);

            disconnect_client(i);
            remove_client(i);

        }

    }

}

void update_time(char* time_buffer){

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer, sizeof(char) * TIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", timeinfo);

}

int main(int argc, char *argv[]){

    if (argc != 3){
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }    

    // Przygotowanie tablicy przechowującej deskryptory klientów i ich id
    prepare_clients_arr();

    // Przechwycenie sygnału SIGINT
    signal(SIGINT, sigint_handler);

    // Przygotowanie struktury przechowującej adres ip
    struct in_addr socket_in_addr;

    // Rzutowanie adresu ip z postaci tekstowej na binarną
    if(inet_pton(AF_INET, argv[1], &socket_in_addr) < 0){
        perror("inet_pton\n");
        return 1;
    }

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
        return 1;
    }

    // Rozpoczynanie nasłuchiwania
    if(listen(server_fd, MAX_CLIENTS) < 0){
        perror("listen\n");
        return 1;
    }

    // Struktura przechowująca otrzymaną wiadomość
    message_t message;

    // Główna pętla serwera
    while(true){
        
        update_time(time_buffer);

        // Akceptowanie nowego klienta
        int client_fd = accept(server_fd, NULL, 0);

        // Jeśli klient się połączył
        if(client_fd > 0){
            
            if(number_of_clients < MAX_CLIENTS){

                // Wyszukiwanie wolnego slota
                for(int i = 0; i < MAX_CLIENTS; i += 1){

                    if(clients_fd[i] == -1){

                        clients_fd[i] = client_fd;
                        number_of_clients += 1;
                        clients_times[i] = clock();
                        break;

                    }

                }

            }else{
                printf("%s | Too many clients!\n", time_buffer);
            }

        }

        for(int i = 0; i < MAX_CLIENTS; i += 1){

            if(clients_fd[i] != -1 && recv(clients_fd[i], &message, sizeof(message_t), MSG_DONTWAIT) > 0){

            

                switch(message.type){
                    
                    // Klient po połączeniu się zawsze wysyła init, żeby przekzać swoje id (nazwę)
                    case INIT:
                    
                        printf("%s | Client %s has joined the server!\n", time_buffer, message.id);
                        strcpy(clients_ids[i], message.id); // Zapisanie id klienta
                        break;

                    case LIST:

                        printf("%s | Client %s requested LIST!\n", time_buffer, message.id);

                        message_t list_message;
                        list_message.type = LIST;
                        strcpy(list_message.id, "server");

                        for(int j = 0; j < MAX_CLIENTS; j += 1){
                            strcpy(list_message.data.list.ids[j], clients_ids[j]);
                        }

                        if(send(clients_fd[i], &list_message, sizeof(message_t), MSG_DONTWAIT) < 0) perror("send\n");

                        break;

                    case TOALL:

                        printf("%s | %s sent message to all clients\n", time_buffer, message.id);

                        strcpy(message.data.msg.send_time, time_buffer);

                        for(int j = 0; j < number_of_clients; j += 1){

                            if(clients_fd[j] != -1 && clients_fd[j] != clients_fd[i]){

                                if(send(clients_fd[j], &message, sizeof(message), MSG_DONTWAIT) < 0) perror("send\n");

                            }

                        }

                        break;

                    case TOONE:

                        printf("%s | %s sent private message to %s\n", time_buffer, message.id, message.data.private_msg.receiver_id);

                        strcpy(message.data.private_msg.send_time, time_buffer);

                        for(int j = 0; j < number_of_clients; j += 1){

                            if(clients_fd[j] != -1 && strcmp(clients_ids[j], message.data.private_msg.receiver_id) == 0){

                                if(send(clients_fd[j], &message, sizeof(message), MSG_DONTWAIT) < 0) perror("send\n");
                                break;

                            }

                        }

                        break;
                    // Klient wysyła stop, gdy chce się rozłączyć
                    case STOP:

                        printf("%s | Client %s has left the server\n", time_buffer, message.id);
                        //Klient wyszedł sam więc nie ma potrzeby wysyłać mu STOP, i tak go nie przyjmie
                        remove_client(i);
                        break;

                    case ALIVE:
                        
                        printf("%s | Client %s sent ALIVE message\n", time_buffer, message.id);
                        clients_times[i] = clock();

                        break;

                    default:
                        printf("Unknown message type\n");
                        break;

                }

            }

        }
        
        check_for_timeout();

    }

}