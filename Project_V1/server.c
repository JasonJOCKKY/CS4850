#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 9999
#define MAX_QUEUE 5
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

    // Bind the socket to the specific IP and SERVER_PORT
    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        printf("Fail to bind!\n");
        return -1;
    }

    // Listen for connections
    if (listen(sockfd, MAX_QUEUE) == -1) {
        printf("Fail to start listening!\n");
        return -1;
    }

    // Accept connection from a client
    int client_socket = accept(sockfd, NULL, NULL);
    if (client_socket == -1) {
        printf("Fail to accept connection from client!\n");
        return -1;
    }

    // Send data
    char message[MAX_LINE] = "Hello from Server!";
    send(client_socket, message, sizeof(message), 0);
    
    return 0;
}