#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/client.h"
#include "../include/chatRoom.h"

// Main
int main() {

    // Join the chatroom
    int client_socket = joinChatRoom(SERVER_PORT);
    if(client_socket == -1) {
        printf("Fail to join the chat room.");
        return -1;
    }

    // Buffer for messages
    char buf[MAX_LINE];

    // Receive and print the initial response
    if (recv(client_socket, buf, sizeof(buf), 0) <= 0) {
        printf("Fail to receive the initial response!\n");
        return -1;
    }
    printf("Server > %s\n", buf);

    // Chatroom Handeler
    while (1) {
        // Get user input
        printf("> ");
        if (fgets(buf, MAX_LINE, stdin) == NULL) {
            printf("Fail to get user input.\n");
            return -1;
        }
        // Parse command
        char *command = strtok(buf, " "); /* The first word entered by a user is the command. */

        // Make request
        if (strcmp(command, "login") == 0) { /* Log in.  login [userID] [password] */
            char *userID = strtok(NULL, " ");
            char *password = strtok(NULL, " ");
            if (!userID || !password) { /* Incorrect use of command. */
                printf(">> Incorrect command. %s\n", commandList[0]);
                continue;
            }
            if (login_request(client_socket, userID, password) == -1) {
                printf(">> Fail to send login request. Closing connection...\n");
                return -1;
            }

        } else if (strcmp(command, "newuser") == 0) { /* Create new user. newuser [userID] [password] */
            char *userID = strtok(NULL, " ");
            char *password = strtok(NULL, " ");
            if (!userID || !password) { /* Incorrect use of command. */
                printf(">> Incorrect command. %s\n", commandList[1]);
                continue;
            }

            int request_result = newuser_request(client_socket, userID, password) == -1;
            if (request_result == -1) { /* Error sending the message. */
                printf(">> Fail to send newuser request. Closing connection...\n");
                return -1;
            } else if (request_result == -2) { /* Invalid userID or password. */
                printf(">> Invalid userID or password. Please make sure the userID is less than %d characters and the length of the password is between %d and %d.\n", MAX_ID_LENGTH, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
                continue;
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
            printf("Logging out...\n");
            if (logout_request(client_socket) == -1) {
                printf("Error logging out!\n");
                return -1;
            }
            return 0;

        } else { /* Invalid command. */
            printf(">> Please enter a valid command:\n");
            for (int i = 0; i < 4; i++) {
                printf("-- %s\n", commandList[i]);
            }
            continue;
        }

        // Receive response
        ssize_t receive_result = recv(client_socket, buf, sizeof(buf), 0);
        // printf("receive_result = %ld\n", receive_result);
        if (receive_result == 0) { /* Lost connection to server */
            printf(">> Lost connection to server. Chutting down...\n");
            return -1;
        } else if (receive_result == -1) { /* Error receiving message. */
            printf(">> Fail to receive message from the server. Closing connection...\n");
            return -1;
        }
        printf("<Server> %s\n", buf);
    }

    
    return 0;
}

int login_request(int socket, char *userID, char *password) {
    char buf[MAX_LINE] = "login ";
    strcat(buf, userID);
    strcat(buf, " ");
    strcat(buf, password);

    return send(socket, buf, sizeof(buf), 0);
}

int newuser_request(int socket, char *newUserID, char *newPassword) {
    return 0;
}

int send_request(int socket, char *message) {
    return 0;
}

int logout_request(int socket) {
    return 0;
}

int joinChatRoom(int serverPort) {

    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        // Fail to create client socket.
        return -1;
    }

    // Structure of socket address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(serverPort);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connect to socket
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        // Connection error
        return -1;
    }

    return client_socket;
}