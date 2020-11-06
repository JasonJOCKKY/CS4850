#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/chatRoom.h"
#include "../include/client.h"

int login_request(int socket, char *userID, char *password)
{
    // Validate parameters
    if (socket < 0 || !userID || !password)
    {
        return -1;
    }

    // Construct the request message to send to the server
    char buf[MAX_LINE] = "login ";
    strcat(buf, userID);
    strcat(buf, " ");
    strcat(buf, password);

    // Send the message
    return send(socket, buf, sizeof(buf), 0);
}

int newuser_request(int socket, char *newUserID, char *newPassword)
{
    // Validate parameters
    if (socket < 0 || !newUserID || !newPassword)
    {
        return -1;
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

int send_request(int socket, char *message)
{
    // Validate parameters
    if (socket < 0 || !message)
    {
        return -1;
    }

    // Construct the request message to send to the server
    char buf[MAX_LINE] = "send ";
    strcat(buf, message);

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
    char buf[MAX_LINE] = "send ";

    // Send the message
    return send(socket, buf, sizeof(buf), 0);
}

int joinChatRoom(int serverPort)
{
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

int exitChatRoom(int socket)
{
    return close(socket);
}

int getUserInput(char *buffer)
{
    // Validate parameter
    if (!buffer)
    {
        return -1;
    }

    if (fgets(buffer, MAX_LINE, stdin) == NULL)
    {
        // Error reading the user input
        return -1;
    }

    return 0;
}

long receiveResponse(int socket, char *buffer, size_t length)
{
    return recv(socket, buffer, length, 0);
}