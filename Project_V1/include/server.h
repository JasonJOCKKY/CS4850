/* Functions for server
Jingsong Tan
Nov. 2020
*/

#include <stdbool.h>

// Data structure for user on the server.
typedef struct userStruct
{
    char *userID;
    bool isLogedIn;
} User;

// Handel the request to log the user in, current user will be set to the correct user.  Send awknoledgement back to the client.
// \return -1 on error.
int login_handler(int socket, char *userID, char *password, User *currentUser);

// Handel the request to create a new user. Write the new user information onto the disk.
// \return -1 for error.
int newuser_handler(int socket, char *newUserID, char *newPassword, User *currentUser);

// Handle the request to broadcast a message.
// \return -1 for error.
int send_handler(int socket, char *message, User *currentUser);

// Handle the request to log the user out.
// \return -1 for error.
int logout_handler(int socket, User *currentUser);

// Handle a invalid command. Send error message to the client.
// \return -1 for error.
int invalid_handler(int cocket, char *command, User *currentUser);

/* Helper Functions */

// Start the chatroom, listening.
// \return the socket descripter for the server. -1 on error.
int startChatRoom(int serverPort);

// Runs a loop to handle client request
// \param currentUser The client user.
// \return -1 for error.
int client_handler(int client_socket, User *currentUser);

// Write a new user to the file;
// \return 0 successful. -1 for error. -2 for user already exists.
int write_newuser(char *userID, char *password);

// Check if userID and password matches
// \return 0 for yes. -1 for error. -2 for no.
int credential_doesMatch(char *userID, char *password);