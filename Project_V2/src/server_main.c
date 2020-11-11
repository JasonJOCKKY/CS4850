#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>

#include "../include/server.h"

// Main
int main()
{
    // Start the chat room.
    int server_socket = start_chatRoom(SERVER_PORT);
    if (server_socket == -1)
    {
        printf("Fail to start the chat room!\n");
        return -1;
    }

    // Keep track of all connected clients.
    ClientList *client_list = create_clientList();
    if (!client_list)
    {
        printf("Fail to create client list.\n");
        return -1;
    }

    printf("Server running on port %d.  Waiting for clients to connect...\n", SERVER_PORT);

    // Keep the server running
    while (1)
    {
        // Accept connection from a client
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1)
        {
            printf("Fail to accept connection from client!\n");
            continue;
        }

        // Add client to the list.
        Client *current_client = add_client(client_list, client_socket);
        if (!current_client)
        {
            printf("Fail to create new client.\n");
            continue;
        }

        printf("> (Socket %d) A client connection established.\n", client_socket);

        // Handle user requests
        pthread_t tid;
        param_t *param = (param_t *)malloc(sizeof(param_t));
        if (!param)
        {
            printf("Fail to allocate params.\n");
            return -1;
        }
        param->client_list = client_list;
        param->current_client = current_client;
        pthread_create(&tid, NULL, &client_handler, param);
    }

    return 0;
}