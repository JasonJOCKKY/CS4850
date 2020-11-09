#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/server.h"

int login_handler(int socket, char *userID, char *password, User *currentUser)
{
    // Validate Parameters
    if (socket < 0 || !userID || !password || !currentUser)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the message. */

    // Construct response message
    if (currentUser->isLogedIn)
    { /* User already logged in. */
        snprintf(buf, MAX_LINE, "Error: You have already logged in, %s!", currentUser->userID);
    }
    else
    { /* Check if credentials match. */
        switch (credential_doesMatch(userID, password))
        {
        case -2: /* credential does not match */
            snprintf(buf, MAX_LINE, "Error: The userID and password does not match!");
            break;
        case 0: /* userID and password matches */
            snprintf(buf, MAX_LINE, "Welcome to the chat room, %s.", userID);
            strcpy(currentUser->userID, userID);
            currentUser->isLogedIn = true;
            break;
        default: /* error when checking credential */
            return -1;
        }
    }

    // Send the response message to the client
    if (send(socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }

    // Successfully handeled the login request.
    return 0;
}

int newuser_handler(int socket, char *newUserID, char *newPassword, User *currentUser)
{
    // Validate Parameters
    if (socket < 0 || !newUserID || !newPassword || !currentUser)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the message. */

    // Construct response message
    if (currentUser->isLogedIn)
    { /* User already logged in. */
        snprintf(buf, MAX_LINE, "Error: You have already logged in, %s.", currentUser->userID);
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
    if (send(socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }

    // Successfully handeled the newuser request.
    return 0;
}

int send_handler(int socket, char *message, User *currentUser)
{
    // Validate Parameters
    if (socket < 0 || !message || !currentUser)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the reponse message. */

    // Construct response message
    if (!currentUser->isLogedIn)
    { /* User have not logged in. */
        snprintf(buf, MAX_LINE, "Error: You have not logged in.");
    }
    else
    { /* User have logged in. */
        snprintf(buf, MAX_LINE, "%s sends: %s", currentUser->userID, message);
    }

    // Send the response message to the client
    if (send(socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }

    // Successfully handeled the send request.
    return 0;
}

int logout_handler(int socket, User *currentUser)
{
    // Validate Parameters
    if (socket < 0 || !currentUser)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the reponse message. */

    // Construct response message
    if (!currentUser->isLogedIn)
    { /* User have not logged in. */
        snprintf(buf, MAX_LINE, "Error: You have not logged in.");
    }
    else
    { /* User have logged in. */
        snprintf(buf, MAX_LINE, "You have been logged out, %s.", currentUser->userID);
        currentUser->isLogedIn = false;
    }

    // Send the response message to the client
    if (send(socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }

    // Successfully handeled the logout request.
    return 0;
}

int invalid_handler(int socket, char *command, User *currentUser)
{
    // Validate Parameters
    if (socket < 0 || !command || !currentUser)
    {
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for the reponse message. */

    // Construct response message
    snprintf(buf, MAX_LINE, "Error! Your command is invalid: %s.", command);

    // Send the response message to the client
    if (send(socket, buf, MAX_LINE, 0) == -1)
    { /* Fail to send response */
        return -1;
    }

    // Successfully handeled the invalid request.
    return 0;
}

int startChatRoom(int serverPort)
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

int credential_doesMatch(char *userID, char *password)
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
            return 0;
        }
    }

    // No matching credential found.
    fclose(fp);
    return -2;
}

int client_handler(int client_socket, User *currentUser)
{
    // Validate parameters
    if (client_socket < 0 || !currentUser)
    {
        return -1;
    }

    // Message buffer
    char buf[MAX_LINE];

    // Keep receieving client message
    while (1)
    {
        // Receive client message
        ssize_t recv_result = recv(client_socket, buf, sizeof(buf), 0);
        if (recv_result == 0)
        { /* Lost connection to the client. */
            printf("> Lost connection to the client!\n");
            return -1;
        }
        else if (recv_result == -1)
        {
            printf("> Fail to receieve message from the client!  Closing connection...\n");
            return -1;
        }

        printf("> Message received: %s. (Current user: %s)\n", buf, currentUser->userID);

        // Parse client command
        char *command = strtok(buf, " \n");

        // Send response
        if (strcmp(command, "login") == 0)
        { /* Log in.  login [userID] [password] */
            char *userID = strtok(NULL, " \n");
            char *password = strtok(NULL, " \n");

            if (login_handler(client_socket, userID, password, currentUser) == -1)
            { /* Error when handling login request. */
                printf(">> Fail to handle login request. Closing connection...\n");
                return -1;
            }
            printf("> Successfully handled a login request.\n");
        }
        else if (strcmp(command, "newuser") == 0)
        { /* Create new user. newuser [userID] [password] */
            char *userID = strtok(NULL, " \n");
            char *password = strtok(NULL, " \n");

            if (newuser_handler(client_socket, userID, password, currentUser) == -1)
            { /* Error when handling newuser request. */
                printf(">> Fail to handle newuser request. Closing connection...\n");
                return -1;
            }
            printf("> Successfully handled a newuser request.\n");
        }
        else if (strcmp(command, "send") == 0)
        { /* Send a message. */
            char *message = strtok(NULL, " \n");

            if (send_handler(client_socket, message, currentUser) == -1)
            { /* Error when handling send request. */
                printf(">> Fail to handle send request. Closing connection...\n");
                return -1;
            }
            printf("> Successfully handled a send request.\n");
        }
        else if (strcmp(command, "logout") == 0)
        { /* Log out. */
            printf("Logging out...\n");
            return logout_handler(client_socket, currentUser);
        }
        else
        { /* Invalid command. */
            if (invalid_handler(client_socket, command, currentUser) == -1)
            { /* Error when handling invalid request. */
                printf(">> Fail to handle invalid request. Closing connection...\n");
                return -1;
            }
        } /* End Send Response. */
    }

    return 0;
}