// Functions for client

// Request the server to log the user in.
// \return -1 on error.
int login_request(int socket, char *userID, char *password);

// Check the userID and password are valid. Then request the server to create a new user.
// \return -1 for error sending the message. -2 for invalid userID or password.
int newuser_request(int socket, char *newUserID, char *newPassword);

// Request the server to broadcast a message.
// \return -1 for error.
int send_request(int socket, char *message);

// Request the server to log the user out.
// \return -1 for error.
int logout_request(int socket);

/* Helper Functions */

// Establish connection to the server and join the chatroom.
// \return the socket descripter for the client. -1 on error.
int joinChatRoom(int serverPort);