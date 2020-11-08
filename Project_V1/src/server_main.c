#include <stdio.h>
#include <sys/socket.h>

#include "../include/server.h"
#include "../include/chatRoom.h"

// Main
int main()
{
    // Start the chat room.
    int server_socket = startChatRoom(SERVER_PORT);
    if (server_socket == -1)
    {
        printf("Fail to start the chat room!\n");
        return -1;
    }

    // Keep the server running
    while (1)
    {
        printf("Server running on port %d.  Waiting for a client to connect...\n", SERVER_PORT);

        // Accept connection from a client
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1)
        {
            printf("Fail to accept connection from client!\n");
            continue;
        }
        printf("> A client connection established.\n");

        // Set current user
        User *currentUser = (User *)malloc(sizeof(User));
        if (!currentUser)
        {
            printf("Fail to initialize current user!\n");
            return -1;
        }
        currentUser->isLogedIn = false; /* The user is not loged in at first. */

        // Send initial message
        char *initialMessage = "You have successfully jointed in the server!";
        if (send(client_socket, initialMessage, sizeof(initialMessage), 0) == -1)
        {
            printf("Fail to send the initial message!\n");
            return -1;
        }

        // Handle user requests
        client_handler(client_socket, currentUser);

        // Prevent memory leak
        free(currentUser);
    }

    return 0;
}