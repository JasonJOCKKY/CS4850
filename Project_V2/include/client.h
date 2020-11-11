/* Functions for client
Jingsong Tan
Nov. 2020
*/

#include "chatRoom.h"

// Establish connection to the server and join the chatroom.
// \return the socket descripter for the client.
// \return -1 on error.
int join_chatRoom(int serverPort);

// Scan the user input and store it to the buffer.
// \return -1 for error.
int get_input(char *buffer);

// Process user's input.
// \return 1 for loggin out. -1 for error. -2 for invlid command.
int command_processer(int socket, char *input_string);

// Receive a message from the server and store it into the buffer.
// \return 0 for success. -1 for error. -2 for lost connection.
int receive_message(int socket, char *buffer);

/* Request Functions 
*/

// Request the server to log the user in.
// \return -1 on error. -2 for invalid userID or password.
int login_request(int socket, char *saveptr);

// Check the userID and password are valid. Then request the server to create a new user.
// \param newUserID length should be less than MAX_ID_LENGTH
// \param newPassword length shouldbe beween MAX_PASSWORD_LENGTH and MIN_PASSWORD_LENGTH
// \return -1 for error sending the message. -2 for invalid userID or password.
int newuser_request(int socket, char *saveptr);

// Request the server to send a message.
// The message will be broadcasted if the scope is "all"
// Otherwise the message will be send to one specific user.
// \return -1 for error. \return -2 for invalid request.
int send_request(int socket, char *saveptr);

// Request a current-user list from the server.
// \return -1 for error.
int who_request(int socket);

// Request the server to log the user out.
// \return -1 for error.
int logout_request(int socket);

/* Utility Functions
*/
int print_error(char *error);
int print_server(char *message);
int print_prompt();
