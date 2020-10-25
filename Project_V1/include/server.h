/* Functions for server
*/

#include <stdbool.h>

// Data structure for user on the server.
typedef struct userStruct {
    char *userID;
    bool isLogedIn;
} User;

// Handel the request to log the user in, current user will be set to the correct user.  Send awknoledgement back to the client.
// \return -1 on error. -2 for missing userID or password. -3 for incorrect userID or password.
int login_handeler(int socket, char *userID, char *password, User *currentUser);

// Handel the request to create a new user. Write the new user information onto the disk.
// \return -1 for error. -2 for invalid userID or password. -3 for user already exist.
int newuser_handler(int socket, char *newUserID, char *newPassword, User *currentUser);

// Handle the request to broadcast a message.
// \return -1 for error.
int send_handler(int socket, char *message, User *currentUser);

// Handle the request to log the user out.
// \return -1 for error.
int logout_handler(int socket, User *currentUser);

/* Helper Functions */

// Start the chatroom, listening.
// \return the socket descripter for the server. -1 on error.
int startChatRoom(int serverPort);

// Runs a loop to handle client request
// \return 0 for normal, -1 for closing connection, -2 for shutting down the server.
int clientrequest_handler(int client_socket, User *currentUser);

// Check if a user already exists.
// \return 0 for false. 1 for true. -1 for error.
int user_doesExist(char *userID);

// Write a new user to the file;
int write_newuser(char *userID, char *password);