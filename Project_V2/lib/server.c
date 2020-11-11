#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/server.h"

void *client_handler(void *params_v)
{
    param_t *params = (param_t *)params_v;

    // Validate Parameters
    if (!params || !params->client_list || !params->current_client)
    {
        printf("(Client_handler) Parameters Invalid.\n");
        return NULL;
    }

    // Extract Parameters
    ClientList *client_list = params->client_list;
    Client *current_client = params->current_client;
    free(params);

    // Message buffer
    char buf[MAX_LINE];

    // Send initial message
    char *initialMessage = "You have successfully reached the chat room server!";
    if (send(current_client->socket, initialMessage, MAX_LINE, 0) == -1)
    {
        printf("(Socket %d) Fail to send the initial message!\n", current_client->socket);
        return NULL;
    }

    // Keep receieving and handling client message
    do
    {
        // Receive client message
        ssize_t recv_result = recv(current_client->socket, buf, sizeof(buf), 0);
        if (recv_result == 0)
        { /* Lost connection to the client. */
            printf("> (Socket %d) Lost connection to the client!\n", current_client->socket);
            return NULL;
        }
        else if (recv_result == -1)
        {
            printf("> (Socket %d) Fail to receieve message from the client!  Closing connection...\n", current_client->socket);
            return NULL;
        }
        printf("> Message received: %s. (Current user: %s)\n", buf, current_client->userID);

    } while (command_processer(client_list, current_client, buf) != -1); /* Proccess client message. */
}

int command_processer(ClientList *client_list, Client *current_client, char *input_string)
{
    // Mock sending
    char buf[MAX_LINE] = "Command Processer entered.";
    send(current_client->socket, buf, MAX_LINE, 0);
    return -1;
}

int start_chatRoom(int serverPort)
{
    // Validate Parameters
    if (serverPort < 0)
    {
        return -1;
    }

    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
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
        return -1;
    }

    // Listen for connections
    if (listen(server_socket, MAX_QUEUE) == -1)
    {
        return -1;
    }

    return server_socket;
}

/* Client List Functions. */

ClientList *create_clientList()
{
    ClientList *newList = (ClientList *)calloc(1, sizeof(ClientList));
    if (!newList)
    {
        return NULL;
    }

    newList->list = NULL;
    newList->size = 0;

    return newList;
}

int destroy_clientList(ClientList *client_list);

Client *add_client(ClientList *client_list, int client_socket)
{
    // Validate Parameters
    if(!client_list || client_socket < 0)
    {
        return NULL;
    }

    Client *newClient = (Client *)calloc(1, sizeof(Client));
    if (!newClient)
    {
        return NULL;
    }

    newClient->socket = client_socket;
    newClient->userID[0] = 0;

    client_list->size++;
    Client *newList = (Client *)realloc(client_list->list, sizeof(Client) * client_list->size);
    if (!newList)
    {
        free(newClient);
        return NULL;
    }

    client_list->list = newList;
    return newClient;
}

int remove_client(ClientList *client_list, Client *target_client)
{
    // Validate parameters
    if (!client_list || !target_client)
    {
        return -1;
    }

    for (size_t i = 0; i < client_list->size; i++)
    {
        if (client_list->list + i == target_client)
        { /* Target in the list. Remove it. */
            // Copy the last client into the target location.
            memcpy(
                client_list->list + i, 
                client_list->list + client_list->size - 1, 
                sizeof(Client *));
            
            // Reduce the list size by 1.
            client_list->size--;

            // Free target.
            free(target_client);

            return 0;
        }
    }

    // Target not in the list.
    return -2;
}

int isInList(ClientList *client_list, Client *target_client);

/* File Functions
*/

int write_newuser(char *userID, char *password);

int credential_doesMatch(char *userID, char *password);

/* Request Handlers
*/

int login_handler(int socket, char *saveptr);

int newuser_handler(int socket, char *saveptr);

int send_handler(int socket, char *saveptr);

int logout_handler(int socket, char *saveptr);

int invalid_handler(int socket, char *saveptr);