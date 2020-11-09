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

// Validate the command.
// \return 0 if command is valid. 1 for invalid command. -1 for error. -2 for exit client.
int validate_command(char *input_string);

// Validate the new user's ID and password.
// \return 0 for valid. 1 for invalid. -1 for error.
int validate_newUser(char *newID, char *newPassword);

// Send the request string to the server.
// \return -1 for error.
int send_request(int socket, char *request_string);

// Receive a message from the server and store it into the buffer.
// \return 0 for success. -1 for error. -2 for lost connection.
int receive_message(int socket, char *buffer);