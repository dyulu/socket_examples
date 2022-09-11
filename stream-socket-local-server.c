#include <signal.h>

#include "stream-socket-local.h"

void int_handler(int sig);
void get_and_process_msg(int msgsock);

int main()
{
    int                sock;
    int                msgsock;
    int                length;
    struct sockaddr_un server;

    signal(SIGINT, int_handler);
 
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("opening stream socket ");
        exit(1);
    }

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, STREAM_SOCKET_SERVER_PATH);

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

void int_handler(int sig)
{
    // Remove the path name from the file system
    printf("Remove %s\n", STREAM_SOCKET_SERVER_PATH); 
    unlink(STREAM_SOCKET_SERVER_PATH);

    // Exit the application
    exit(0);
}

void get_and_process_msg(int msgsock)
{
    struct message_t msg;
    int              rval;

    do {
        memset(&msg, 0, sizeof(struct message_t));
        if ((rval = read(msgsock, &msg, sizeof(struct message_t))) == -1)  // Blocks until there is something read, i.e. after the client has executed a write()
            perror("reading stream message ");

        if (rval == 0)    // Client closes the socket
            printf("Ending connection\n");
        else {
            printf("<<<< Received: %s\n", msg.msg);
            switch(msg.msg_id) {
                case MSG_ID_RAIN:
                    printf(">>>> Response: %s\n", MSG_RAIN_RESPONSE);
                    if (write(msgsock, MSG_RAIN_RESPONSE, sizeof(MSG_RAIN_RESPONSE)) == -1) {
                        perror("writing on stream socket ");
                    }
                    break;

                case MSG_ID_COVID:
                    printf(">>>> Response: %s\n", MSG_COVID_RESPONSE);
                    if (write(msgsock, MSG_COVID_RESPONSE, sizeof(MSG_COVID_RESPONSE)) == -1) {
                        perror("writing on stream socket ");
                    }
            }
        }
    } while (rval > 0);
}
