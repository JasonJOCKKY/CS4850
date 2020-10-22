#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 13347
#define MAX_LINE 256

// Login
void login() {

}

// New User
#define MAX_ID_LENGTH 32
#define MAX_PASSWORD_LENGTH 8
#define MIN_PASSWORD_LENGTH 4
#define USERLIST_FILE "user.txt" 

void newUser() {

}

// Send
void userSend() {

}

// Logout
void logout() {

}

// Main
int main(int argc, char **argv) {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Fail to create a socket!\n");
        return -1;
    }

    // Structure for socket address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connect to socket
    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        printf("Connection error!\n");
        return -1;
    } 

    char buf[MAX_LINE];

    // Initial message from server
    if (recv(sockfd, buf, MAX_LINE, 0) == -1) {
        printf("Fail to receieve a message!\n");
        return -1;
    }
    printf("%s\n", buf);

    // Keep the client running
    while (1) {
        printf("> ");
        scanf("%s", buf);
        send(sockfd, buf, strlen(buf), 0);
    }
    
    return 0;
}