#include <sys/socket.h>
#include <netdb.h>        // gethostbyname
#include <stdio.h>
// #include <sys/types.h>
// #include <netinet/in.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STREAM_SOCKET_IP_ADDRESS  "127.0.0.1"
#define STREAM_SOCKET_PORT_NUMBER 65515

#define TRUE     1
#define BUF_SIZE 1024

#define MSG_RAIN  "Rain"
#define MSG_COVID "Covid"

#define MSG_RAIN_RESPONSE  "Rain, rain, go away and come back another day"
#define MSG_COVID_RESPONSE "Covid, covid, go disappear and disappear forever"
