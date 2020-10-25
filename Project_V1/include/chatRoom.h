#define MAX_ID_LENGTH 32
#define MAX_PASSWORD_LENGTH 8
#define MIN_PASSWORD_LENGTH 4
#define USERLIST_FILE "user.txt" 
#define SERVER_PORT 13347
#define MAX_LINE 256
#define MAX_QUEUE 5

// List of all commands
char commandList[][MAX_LINE] = {
    "login [userID] [password]",
    "newuser [userID] [password]",
    "send [message]",
    "logout"
};