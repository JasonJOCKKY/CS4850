#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/client.h"

#define delim " \n"

int join_chatRoom(int serverPort)
{
    // Validate parameters
    if (serverPort < 0)
    {
        return -1;
    }

    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        // Fail to create client socket.
        return -1;
    }

    // Structure of socket address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(serverPort);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connect to socket
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        // Connection error
        return -1;
    }

    return client_socket;
}

int get_input(char *buffer)
{
    // Validate Parameter
    if (!buffer)
    {
        return -1;
    }

    if (fgets(buffer, MAX_LINE, stdin) == NULL)
    { /* Error reading user input. */
        return -1;
    }

    // Trim the newline character at the end
    if (buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = 0;
    }

    return 0;
}

int command_processer(int socket, char *input_string)
{
    // Validate Parameters
    if (!input_string)
    {
        return -1;
    }

    // Copy the input string.
    char input_copy[MAX_LINE];
    strcpy(input_copy, input_string);

    // Parameters for tokenize the input.
    char *saveptr;
    char *token = __strtok_r(input_copy, delim, &saveptr); /* The first word entered by a user is the command. */

    // Make different requests with command.
    if (strcmp(token, "login") == 0)
    { /* Log in.  login [userID] [password] */
        return login_request(socket, saveptr);
    }
    else if (strcmp(token, "newuser") == 0)
    { /* Create new user. newuser [userID] [password] */
        return newuser_request(socket, saveptr);
    }
    else if (strcmp(token, "send") == 0)
    {
        /* Send a message.
        send all [message] OR
        send [userID] [message] */
        return send_request(socket, saveptr);
    }
    else if (strcmp(token, "who") == 0)
    { /* who */
        return who_request(socket);
    }
    else if (strcmp(token, "logout") == 0)
    { /* Log out. logout */
        if (logout_request(socket) == -1)
        {
            return -1;
        }

        // Logging out
        return 1;
    }
    else
    { /* Invalid command. */
        return -2;
    } /* End Make Request. */
}

int receive_message(int socket, char *buffer)
{
    switch (recv(socket, buffer, MAX_LINE, 0))
    {
    case 0: /* Lost connection. */
        return -2;
    case -1: /* Error. */
        return -1;
    default: /* Success. */
        return 0;
    }
}

/* Request Functions */
int login_request(int socket, char *saveptr)
{
    // Validate parameters
    if (socket < 0 || !saveptr)
    {
        return -1;
    }

    // Get userID and password
    char *userID = __strtok_r(NULL, delim, &saveptr);
    char *password = __strtok_r(NULL, delim, &saveptr);
    if (!userID || !password)
    {
        return -2;
    }

    // Construct the request message to send to the server
    char buf[MAX_LINE] = "login ";
    strcat(buf, userID);
    strcat(buf, " ");
    strcat(buf, password);

    // Send the message
    return send(socket, buf, sizeof(buf), 0);
}

int newuser_request(int socket, char *saveptr)
{
    // Validate parameters
    if (socket < 0 || !saveptr)
    {
        return -1;
    }

    // Get userID and password
    char *newUserID = __strtok_r(NULL, delim, &saveptr);
    char *newPassword = __strtok_r(NULL, delim, &saveptr);
    if (!newUserID || !newPassword)
    {
        return -2;
    }

    // Validate userID and password
    size_t idLength = strlen(newUserID);
    size_t pwLength = strlen(newPassword);
    if (idLength <= 0 || idLength > MAX_ID_LENGTH || pwLength < MIN_PASSWORD_LENGTH || pwLength > MAX_PASSWORD_LENGTH)
    {
        return -2;
    }

    // Construct the request message to send to the server
    char buf[MAX_LINE] = "newuser ";
    strcat(buf, newUserID);
    strcat(buf, " ");
    strcat(buf, newPassword);

    // Send the message
    return send(socket, buf, sizeof(buf), 0);
}

int send_request(int socket, char *saveptr)
{
    // Validate parameters
    if (socket < 0 || !saveptr)
    {
        return -1;
    }

    // Get scope and message
    char *scope = __strtok_r(NULL, delim, &saveptr); /* Either "all" or userID */
    char *message = __strtok_r(NULL, "", &saveptr);

    if (!scope || !message)
    {
        return -2;
    }

    // Construct the request message to send to the server
    char buf[MAX_LINE] = "send ";
    strcat(buf, scope);
    strcat(buf, " ");
    strcat(buf, message);

    // Send the message
    return send(socket, buf, sizeof(buf), 0);
}

int who_request(int socket)
{
    // Validate parameters
    if (socket < 0)
    {
        return -1;
    }

    // Construct the request message to send to the server
    char buf[MAX_LINE] = "who";

    // Send the message
    return send(socket, buf, sizeof(buf), 0);
}

int logout_request(int socket)
{
    // Validate parameters
    if (socket < 0)
    {
        return -1;
    }

    // Construct the request message to send to the server
    char buf[MAX_LINE] = "logout";

    // Send the message
    return send(socket, buf, sizeof(buf), 0);
}

/* Utility Functinos */
int print_error(char *error)
{
    printf("<<Error>> %s\n", error);
    return 0;
}

int print_server(char *message)
{
    printf("<Server> %s\n", message);
    return 0;
}

int print_prompt()
{
    return printf("> ");
}
