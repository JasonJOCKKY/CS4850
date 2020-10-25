#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/server.h"
#include "../include/chatRoom.h"

int main()
{
    // Start the chat room.
    int server_socket = startChatRoom(SERVER_PORT);
    if (server_socket == -1) {
        printf("Fail to start the chat room!\n");
        return -1;
    }

    // Keep the server running
    while (1) {
        printf("Server running on port %d.  Waiting for a client to connect...\n", SERVER_PORT);

        // Accept connection from a client
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            printf("Fail to accept connection from client!\n");
            continue;
        }
        printf("> A client connection established.\n");

         // Set current user
        User *currentUser = (User*)malloc(sizeof(User));
        if (!currentUser) {
            printf("Fail to initialize current user!\n");
            return -1;
        }
        currentUser->isLogedIn = false; /* The user is not loged in at first. */

        // Send initial message
        char *initialMessage = "You have successfully jointed in the server!";
        if (send(client_socket, initialMessage, sizeof(initialMessage), 0) == -1) {
            printf("Fail to send the initial message!\n");
            return -1;
        }

        // Handle user requests
        int request_handler;
        do {
            request_handler = clientrequest_handler(client_socket, currentUser);
        } while (request_handler == 0);

        // Prevent memory leak
        free(currentUser);

    }

    return 0;
}

int login_handler(int socket, char *userID, char *password, User *currentUser);

int newuser_handler(int socket, char *newUserID, char *newPassword, User *currentUser);

int send_handler(int socket, char *message, User *currentUser);

int logout_handler(int socket, User *currentUser);

int startChatRoom(int serverPort) {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        return -1;
    }

    // Structure for socket address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specific IP and SERVER_PORT
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        return -1;
    }

    // Listen for connections
    if (listen(server_socket, MAX_QUEUE) == -1) {
        return -1;
    }

    return server_socket;
}

int clientrequest_handler(int client_socket, User *currentUser) {

    // Message buffer
    char buf[MAX_LINE];

    // Receive client command
    ssize_t recv_result = recv(client_socket, buf, sizeof(buf), 0);
    if (recv_result == 0) { /* Lost connection to the client. */
        printf("> Lost connection to the client!\n");
        return -1;
    } else if (recv_result == -1) {
        printf("> Fail to receieve command from the client!  Closing connection...\n");
        return -1;
    }

    printf("> Command received: %s\n", buf);

    // Parse client command
    char *command = strtok(buf, " ");

    // Send response
    if (strcmp(command, "login") == 0) { /* Log in.  login [userID] [password] */
        char *userID = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        
        if (login_handeler(client_socket, userID, password, currentUser) == -1) {
            printf("Login handeler error! Closing connection...\n");
            return -1;
        }

    } else if (strcmp(command, "newuser") == 0) { /* Create new user. newuser [userID] [password] */
        char *userID = strtok(NULL, " ");
        char *password = strtok(NULL, " ");

        if (newuser_handler(client_socket, userID, password, currentUser) == -1) {
            printf("Newuser handler error!  Closing connection...\n");
            return -1;
        }

    } else if (strcmp(command, "send") == 0) { /* Send a message. */
        char *message = strtok(NULL, " ");
        if (!message) { /* Incorrect use of command. */
            printf(">> Incorrect command. %s\n", commandList[2]);
            continue;
        }
        
        if (send_request(client_socket, message) == -1) {
            printf(">> Fail to send message request. Closing connection...\n");
            return -1;
        }

    } else if (strcmp(command, "logout") == 0) { /* Log out. */
        printf("Client loged out. Closing connection...\n");
        break;

    } else { /* Invalid command. */
        char message[MAX_LINE] = "Invalid command: ";
        strcat(message, buf);
        send(client_socket, message, sizeof(message), 0);
    }
}

int user_doesExist(char *userID);

int write_newuser(char *userID, char *password);