#include <sys/socket.h>
#include <sys/un.h>        // sockaddr_un structure
#include <stdio.h>
// #include <sys/types.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STREAM_SOCKET_SERVER_PATH  "/tmp/my-server.socket"

#define TRUE     1
#define BUF_SIZE 1024

#define MSG_RAIN  "Rain"
#define MSG_COVID "Covid"

#define MSG_RAIN_RESPONSE  "Rain, rain, go away and come back another day"
#define MSG_COVID_RESPONSE "Covid, covid, go disappear and disappear forever"

enum MSG_ID {
    MSG_ID_RAIN = 0,
    MSG_ID_COVID
};

struct message_t {
    enum MSG_ID msg_id;
    char        msg[BUF_SIZE];
};
