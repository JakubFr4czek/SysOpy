#ifndef SERVER_CONFIG_H
#define server_CONFIG_H

#define MAX_CLIENTS 10
#define ID_LENGTH 32
#define MAX_TIME_LENGTH 32
#define MESSAGE_LENGTH 1024

typedef enum{

    LIST,
    TOALL,
    TOONE,
    STOP,
    ALIVE,
    INIT

} message_type_t;

typedef struct{

    message_type_t type;
    char id[ID_LENGTH];

    union{

        struct{

            char ids[MAX_CLIENTS][ID_LENGTH];

        } list;

        struct{

            char message[MESSAGE_LENGTH];
            char send_time[MAX_TIME_LENGTH];

        } msg;

    } data;

}message_t;

#endif
