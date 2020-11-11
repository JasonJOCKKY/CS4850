/* Functions for server
Jingsong Tan
Nov. 2020
*/

#include <stdio.h>
#include <stdbool.h>
#include "chatRoom.h"

/* Client List Functions 
*/

typedef struct client_t
{
    int socket;
    char userID[MAX_ID_LENGTH]; /* userID is empty if the user is not logged in. userID will be set when the user is logged in. */
} Client;

typedef struct clientList_t
{
    Client *list;
    size_t size;
} ClientList;

// Create a new client list.
// \return Pointer to the new list. NULL for error.
ClientList *create_clientList();

// Distroy a client list.
// \return -1 for error.
int destroy_clientList(ClientList *client_list);

// Add a client to the list.
// \return Pointer to the new client. NULL for error.
Client *add_client(ClientList *client_list, int client_socket);

// Remove a client from the list.
// \return -1 for error. -2 for target not in the list.
int remove_client(ClientList *client_list, Client *target_client);

// Check if a client is in the list.
// \return 0 for yes. 1 for no. -1 for error. 
int isInList(ClientList *client_list, Client *target_client);

/* Chatroom Functions 
*/

// Start the chatroom, listening.
// \return the socket descripter for the server. -1 on error.
int start_chatRoom(int serverPort);

// Discunnects a client. Remove the client from the client list.
// \return -1 for error.
int disconnect_client(Client *target_client, ClientList *client_list);

/* Thread functions
*/

// Runs a loop to handle one client.
// \param params param_t*
void *client_handler(void *params);
// Struct for parameter
typedef struct client_handler_param {
    Client *current_client;
    ClientList *client_list;
} param_t;

// Process client's input.
// \return -1 for shutting down connection.
int command_processer(ClientList *client_list, Client *current_client, char *input_string);

/* File Functions
*/

// Write a new user to the file;
// \return 0 successful. -1 for error. -2 for user already exists.
int write_newuser(char *userID, char *password);

// Check if userID and password matches
// \return 0 for yes. -1 for error. -2 for no.
int credential_doesMatch(char *userID, char *password);

/* Request Handlers
*/

// Handel the request to log the user in, current user will be set to the correct user.  Send awknoledgement back to the client.
// \return -1 on error.
int login_handler(int socket, char *saveptr);

// Handel the request to create a new user. Write the new user information onto the disk.
// \return -1 for error.
int newuser_handler(int socket, char *saveptr);

// Handle the request to broadcast a message.
// \return -1 for error.
int send_handler(int socket, char *saveptr);

// Handle the request to log the user out.
// \return -1 for error.
int logout_handler(int socket, char *saveptr);

// Handle a invalid command. Send error message to the client.
// \return -1 for error.
int invalid_handler(int socket, char *saveptr);