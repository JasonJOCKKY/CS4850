/* Functions for client
Jingsong Tan
Nov. 2020
*/

#include "chatRoom.h"

// Request the server to log the user in.
// \return -1 on error.
int login_request(int socket, char *userID, char *password);

// Check the userID and password are valid. Then request the server to create a new user.
// \param newUserID length should be less than MAX_ID_LENGTH
// \param newPassword length shouldbe beween MAX_PASSWORD_LENGTH and MIN_PASSWORD_LENGTH
// \return -1 for error sending the message.
// \return -2 for invalid userID or password.
int newuser_request(int socket, char *newUserID, char *newPassword);

// Request the server to broadcast a message.
// \return -1 for error.
int send_request(int socket, char *message);

// Request the server to log the user out.
// \return -1 for error.
int logout_request(int socket);

/* Helper Functions */

// Establish connection to the server and join the chatroom.
// \return the socket descripter for the client.
// \return -1 on error.
int joinChatRoom(int serverPort);

// Scan the user input and store it to the buffer.
// \return -1 for error.
int getUserInput(char *buffer);

// Receieve response from the server.
// \param buffer Store the response into the buffer.
// \param length length of the message.
// \return -1 on error.
long receiveResponse(int socket, char *buffer, size_t length);