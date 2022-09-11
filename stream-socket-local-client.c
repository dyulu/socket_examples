#include "stream-socket-local.h"

int  send_msg(int sock, struct message_t*  msg);
void wait_for_response(int msgsock);
 
int main()
{
    int    sock;
    struct sockaddr_un server;
    struct message_t   msg;
 
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("opening stream socket");
        exit(1);
    }

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, STREAM_SOCKET_SERVER_PATH);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("connecting stream socket, closing the socket ");
        close(sock);
        exit(1);
    }

    memset(&msg, 0, sizeof(msg));
    msg.msg_id = MSG_ID_RAIN;
    strcpy(msg.msg, MSG_RAIN); 
    if (send_msg(sock, &msg) == -1) {
        close(sock);
        exit(1);
    }

    memset(&msg, 0, sizeof(msg));
    msg.msg_id = MSG_ID_COVID;
    strcpy(msg.msg, MSG_COVID);
    wait_for_response(sock);


    if (send_msg(sock, &msg) == -1) {
        close(sock);
        exit(1);
    }

    wait_for_response(sock);

    close(sock);
    exit(0);
}

int send_msg(int sock, struct message_t* msg)
{
    printf(">>>> Sending msg: %s\n", msg->msg);
    if (write(sock, msg, sizeof(struct message_t)) == -1) {
        perror("writing on stream socket ");
        return -1;
    }

    return 0;
}

void wait_for_response(int msgsock)
{
    char buf[BUF_SIZE];
    int  rval;

    memset(buf, 0, sizeof(buf));
    if ((rval = read(msgsock, buf, BUF_SIZE)) == -1)  // With blocking socket, blocking till receives some msg
        perror("reading stream message ");

    printf("<<<< Received: %s\n", buf);
}

