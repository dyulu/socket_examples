#include "stream-socket-inet.h"

int  send_msg(int sock, char* msg);
void wait_for_response(int msgsock);
 
int main(int argc, char* argv[])
{
    int    sock;
    struct sockaddr_in server;
    struct hostent* hostentry;
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("opening stream socket");
        exit(1);
    }

    server.sin_family = AF_INET;

    hostentry = gethostbyname(STREAM_SOCKET_IP_ADDRESS);
    if (hostentry == (struct hostent *) 0) {
        fprintf(stderr, "%s: unknown host\n", STREAM_SOCKET_IP_ADDRESS);
        exit(1);
    }

    memcpy((char *)&server.sin_addr, (char *)hostentry->h_addr, hostentry->h_length);
    server.sin_port = htons(STREAM_SOCKET_PORT_NUMBER);
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("connecting stream socket, closing the socket ");
        close(sock);
        exit(1);
    }

    if (send_msg(sock, MSG_RAIN) == -1) {
        close(sock);
        exit(1);
    }

    wait_for_response(sock);

    if (send_msg(sock, MSG_COVID) == -1) {
        close(sock);
        exit(1);
    }

    wait_for_response(sock);

    close(sock);
    exit(0);
}

int send_msg(int sock, char* msg)
{
    printf(">>>> Sending msg: %s\n", msg);
    if (write(sock, msg, sizeof(msg)) == -1) {
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

