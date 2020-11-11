#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#include "../include/server.h"

#define delim " \n"

void *client_handler(void *params_v)
{
    param_t *params = (param_t *)params_v;

    // Validate Parameters
    if (!params || !params->client_list || !params->current_client || !params->lock)
    {
        printf("(Client_handler) Parameters Invalid.\n");
        return NULL;
    }

    // Extract Parameters
    ClientList *client_list = params->client_list;
    Client *current_client = params->current_client;
    pthread_mutex_t *lock = params->lock;
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
            break;
        }
        else if (recv_result == -1)
        { /* Error receiving message */
            printf("> (Socket %d) Fail to receieve message from the client!  Closing connection...\n", current_client->socket);
            break;
        }
        printf("> Message received: %s. (Current user: %s)\n", buf, current_client->userID);

    } while (command_processer(client_list, current_client, buf) != -1); /* Proccess client message. */

    // Remove client connection
    close(current_client->socket);
    remove_client(client_list, current_client);

    return NULL;
}

int command_processer(ClientList *client_list, Client *current_client, char *input_string)
{
    // Validate Parameters
    if (!client_list || !current_client || !input_string)
    {
        printf("Command Processer error.\n");
        return -1;
    }

    // Copy the input string.
    char input_copy[MAX_LINE];
    strcpy(input_copy, input_string);

    // Parameters for tokenize the input.
    char *saveptr;
    char *token = strtok_r(input_copy, delim, &saveptr); /* The first word is the command. */

    // Make different response with command.
    if (strcmp(token, "login") == 0)
    { /* Log in.  login [userID] [password] */
        return login_handler(current_client, saveptr);
    }
    else if (strcmp(token, "newuser") == 0)
    { /* Create new user. newuser [userID] [password] */
        return newuser_handler(current_client, saveptr);
    }
    else if (strcmp(token, "send") == 0)
    {
        /* Send a message.
        send all [message] OR
        send [userID] [message] */
        return send_handler(client_list, current_client, saveptr);
    }
    else if (strcmp(token, "who") == 0)
    { /* who */
        return who_handler(client_list, current_client);
    }
    else if (strcmp(token, "logout") == 0)
    { /* Log out. logout */
        return -1;
    }
    else
    { /* Invalid command. */
        return invalid_handler(current_client, input_string);
    } /* End Make Request. */
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
    if (listen(server_socket, MAX_CLIENTS) == -1)
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

    newList->size = 0;

    return newList;
}

Client *add_client(ClientList *client_list, int client_socket)
{
    // Validate Parameters
    if (!client_list || client_socket < 0)
    {
        return NULL;
    }

    // Check if the list is full
    if (client_list->size >= MAX_CLIENTS)
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

    client_list->list[client_list->size] = newClient;
    client_list->size++;

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
        if (client_list->list[i] == target_client)
        { /* Target in the list. Remove it. */
            // Copy the last client into the target location.
            client_list->list[i] = client_list->list[client_list->size - 1];

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

int isInList(ClientList *client_list, Client *target_client)
{
    // Validate parameters
    if (!client_list || !target_client)
    {
        return -1;
    }

    for (size_t i = 0; i < client_list->size; i++)
    {
        if (client_list->list[i] == target_client)
        { /* Target is in the list. */
            return 1;
        }
    }

    // Target is not in the list.
    return 0;
}

/* File Functions
*/

int write_newuser(char *userID, char *password)
{
    // Validate Parameters
    if (!userID || !password)
    {
        return -1;
    }

    // Open the file
    FILE *fp = fopen(USERLIST_FILE, "r+");
    if (!fp)
    { /* Fail to open the file. */
        return -1;
    }

    // Check if user already exists
    char idBuf[MAX_ID_LENGTH];
    while (fscanf(fp, "(%[^,], %*s\n", idBuf) == 1)
    {
        if (strcmp(idBuf, userID) == 0)
        { /* User already exists. */
            fclose(fp);
            return -2;
        }
    }

    // Write new user into the file
    if (fprintf(fp, "\n(%s, %s)", userID, password) < 0)
    { /* Fail to write into the file. */
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int authenticate(char *userID, char *password)
{
    // Validate Parameters
    if (!userID || !password)
    {
        return -1;
    }

    // Open the file
    FILE *fp = fopen(USERLIST_FILE, "r");
    if (!fp)
    { /* Fail to open the file. */
        return -1;
    }

    char idBuf[MAX_ID_LENGTH];
    char passwordBuf[MAX_PASSWORD_LENGTH];

    // Read from the file
    while (fscanf(fp, "(%[^,], %[^)])\n", idBuf, passwordBuf) == 2)
    {
        if (strcmp(idBuf, userID) == 0 && strcmp(passwordBuf, password) == 0)
        { /* Found a matching pair. */
            fclose(fp);
            return 1;
        }
    }

    // No matching credential found.
    fclose(fp);
    return 0;
}

/* Request Handlers
*/

int login_handler(Client *current_client, char *saveptr)
{
    // Validate Parameters
    if (!saveptr || !current_client)
    {
        return -1;
    }

    // Get userID and password
    char *userID = strtok_r(NULL, delim, &saveptr);
    char *password = strtok_r(NULL, delim, &saveptr);
    if (!userID || !password)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the message. */

    // Construct response message
    if (strcmp(current_client->userID, "") != 0)
    { /* User already logged in. */
        snprintf(buf, MAX_LINE, "Error: You have already logged in, %s!", current_client->userID);
    }
    else
    { /* Check if credentials match. */
        switch (authenticate(userID, password))
        {
        case 0: /* credential does not match */
            snprintf(buf, MAX_LINE, "Error: The userID and password does not match!");
            break;
        case 1: /* userID and password matches */
            snprintf(buf, MAX_LINE, "Welcome to the chat room, %s.", userID);
            strcpy(current_client->userID, userID);
            break;
        default: /* error when checking credential */
            return -1;
        }
    }

    // Send the response message to the client
    if (send(current_client->socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }

    // Successfully handeled the login request.
    return 0;
}

int newuser_handler(Client *current_client, char *saveptr)
{
    // Validate Parameters
    if (!saveptr || !current_client)
    {
        return -1;
    }

    // Get userID and password
    char *newUserID = strtok_r(NULL, delim, &saveptr);
    char *newPassword = strtok_r(NULL, delim, &saveptr);
    if (!newUserID || !newPassword)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the message. */

    // Construct response message
    if (strcmp(current_client->userID, "") != 0)
    { /* User already logged in. */
        snprintf(buf, MAX_LINE, "Error: You have already logged in, %s.", current_client->userID);
    }
    else
    { /* Check if user already exists. */
        switch (write_newuser(newUserID, newPassword))
        {
        case -2: /* new user already exists. */
            snprintf(buf, MAX_LINE, "Error: User %s already exists.", newUserID);
            break;
        case 0: /* successfully created a new user. */
            snprintf(buf, MAX_LINE, "Successfully created new user %s.", newUserID);
            break;
        default: /* error when creating new user. */
            return -1;
        }
    }

    // Send the response message to the client
    if (send(current_client->socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }

    // Successfully handeled the login request.
    return 0;
}

int send_handler(ClientList *client_list, Client *current_client, char *saveptr)
{
    // Validate Parameters
    if (!client_list || !current_client || !saveptr)
    {
        return -1;
    }

    // Get userID and password
    char *scope = strtok_r(NULL, delim, &saveptr);
    char *message = strtok_r(NULL, "", &saveptr);

    if (!scope || !message)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the reponse message. */

    if (strcmp(current_client->userID, "") == 0)
    { /* User have not logged in. */
        snprintf(buf, MAX_LINE, "Error: You have not logged in.");
        if (send(current_client->socket, buf, MAX_LINE, 0) == -1)
        { /* Fail to send response */
            return -1;
        }
        return 0;
    }

    if (strcmp(scope, "all") == 0)
    { /* User wants to broadcast a message. */
        snprintf(buf, MAX_LINE, "%s: %s", current_client->userID, message);
        for (size_t i = 0; i < client_list->size; i++)
        {
            if (send(client_list->list[i]->socket, buf, MAX_LINE, 0) == -1)
            { /* Fail to send response */
                return -1;
            }
        }
        return 0;
    }

    /* User wants to send a private message. 
        Target user == scope
    */
    for (size_t i = 0; i < client_list->size; i++)
    {
        if (strcmp(client_list->list[i]->userID, scope) == 0)
        { /* Found Matching target */
            snprintf(buf, MAX_LINE, "(Private) %s: %s", current_client->userID, message);
            if (send(client_list->list[i]->socket, buf, MAX_LINE, 0) == -1)
            { /* Fail to send response */
                return -1;
            }
            return 0;
        }
    }

    // No matching trget found in the list.
    snprintf(buf, MAX_LINE, "Error: There is no user '%s'", scope);
    if (send(current_client->socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }
    return 0;
}

int who_handler(ClientList *client_list, Client *current_client)
{
    // Validate Parameters
    if (!client_list || !current_client)
    {
        return -1;
    }

    // Construct response message
    char buf[MAX_LINE] = "In Chat: ( "; /* Buffer for the reponse message. */
    for (size_t i = 0; i < client_list->size; i++)
    {
        if (strcmp(client_list->list[i]->userID, "") != 0)
        { /* This is a logged-in user. */
            strcat(buf, client_list->list[i]->userID);
            strcat(buf, ", ");
        }
    }
    strcat(buf, ")");

    // Send response
    if (send(current_client->socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }
    return 0;
}

int invalid_handler(Client *current_client, char *input)
{
    // Validate Parameters
    if (!current_client || !input)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the reponse message. */
    snprintf(buf, MAX_LINE, "Error: Invalid command '%s'.", input);

    // Send response
    if (send(current_client->socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }
    return 0;
}