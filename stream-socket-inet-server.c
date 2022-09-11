#include <arpa/inet.h>     // inet_addr

#include "stream-socket-inet.h"
 
void get_and_process_msg(int msgsock);

int main()
{
    int                sock;
    int                msgsock;
    int                length;
    struct sockaddr_in server;
    struct hostent*    hostentry;
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("opening stream socket ");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(STREAM_SOCKET_IP_ADDRESS);
    /*
    hostentry = gethostbyname(STREAM_SOCKET_IP_ADDRESS);
    if (hostentry == (struct hostent *) 0) {
        fprintf(stderr, "%s: unknown host\n", STREAM_SOCKET_IP_ADDRESS);
        exit(1);
    }
    memcpy((char *)&server.sin_addr, (char *)hostentry->h_addr, hostentry->h_length);
    */
    server.sin_port = htons(STREAM_SOCKET_PORT_NUMBER);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("binding stream socket ");
        exit(1);
    }

    length = sizeof(server);
    if (getsockname(sock, (struct sockaddr *)&server, &length) == -1) {
        perror("getting socket name ");
        exit(1);
    }

    listen(sock, 5);  // 5: the number of connections that can be waiting while the process is handling a particular connection
    while (TRUE) {
        printf("Waiting for client to connect ....\n");
        msgsock = accept(sock, (struct sockaddr *)0, (int *)0);  // Blocks until a client connects
        if (msgsock == -1)
            perror("accept ");
        else {
            if (fork() == 0) {        // Child process to handle the client connection,
                                      // so parent process can accept other connections
                printf("Get and processs msg ...\n");
                close(sock);
                get_and_process_msg(msgsock);
                exit(0);
            }
        }

        printf("Done with this client connection, so closing\n");
        close(msgsock);
    }

    // All sockets will be closed automatically when the process is killed or terminates normally
    exit(0);
}

void get_and_process_msg(int msgsock)
{
    char buf[BUF_SIZE];
    int  rval;

    do {
        memset(buf, 0, sizeof(buf));
        if ((rval = read(msgsock, buf, BUF_SIZE)) == -1)  // Blocks until there is something read, i.e. after the client has executed a write()
            perror("reading stream message ");

        if (rval == 0)    // Client closes the socket
            printf("Ending connection\n");
        else {
            printf("<<<< Received: %s\n", buf);
            if (strcmp(buf, MSG_RAIN) == 0) {
                printf(">>>> Response: %s\n", MSG_RAIN_RESPONSE);
                if (write(msgsock, MSG_RAIN_RESPONSE, sizeof(MSG_RAIN_RESPONSE)) == -1) {
                    perror("writing on stream socket ");
                }
            }
            if (strcmp(buf, MSG_COVID) == 0) {
                printf(">>>> Response: %s\n", MSG_COVID_RESPONSE);
                if (write(msgsock, MSG_COVID_RESPONSE, sizeof(MSG_COVID_RESPONSE)) == -1) {
                    perror("writing on stream socket ");
                }
            }
        }
    } while (rval > 0);
}
