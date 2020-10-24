#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/server.h"
#include "../include/chatRoom.h"

int main()
{
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("Fail to create a socket!\n");
        return -1;
    }

    // Structure for socket address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specific IP and SERVER_PORT
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        printf("Fail to bind!\n");
        return -1;
    }

    // Listen for connections
    if (listen(server_socket, MAX_QUEUE) == -1)
    {
        printf("Fail to start listening!\n");
        return -1;
    }

    printf("Server running on port %d.  Waiting for a client to connet...\n", SERVER_PORT);

    // Keep the server running
    while (1) {
        // Accept connection from a client
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            printf("Fail to accept connection from client!\n");
            return -1;
        }

        printf(">A client connection established.\n");

        // Send data
        char buf[MAX_LINE] = "You have successfully jointed in the server!";
        if (send(client_socket, buf, sizeof(buf), 0) == -1) {
            printf("Fail to send the initial message!\n");
            continue;
        }

        // Receive data
        ssize_t result = 1;
        while (1) {
            result = recv(client_socket, buf, sizeof(buf), 0);
            if (result  == 0) {
                printf("Lost connection to client.\n");
                break;
            }
            printf("Client says: %s\n", buf);
        }
    }

    return 0;
}