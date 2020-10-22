#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 9999
#define MAX_LINE 256

int main(int argc, char **argv) {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Fail to create a socket!\n");
        return -1;
    }

    // Structure for socket address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connect to socket
    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        printf("Connection error!\n");
        return -1;
    }

    // Send and receieve data
    char buf[MAX_LINE];       

    recv(sockfd, buf, MAX_LINE, 0); 
    printf("Server says: %s\n", buf);
    
    return 0;
}