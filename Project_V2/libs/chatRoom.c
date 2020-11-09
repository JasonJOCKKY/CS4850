#include "../include/chatRoom.h"

const char COMMAND_LIST[NUM_COMMANDS][MAX_LINE] = {
    "login [userID] [password]",
    "newuser [userID] [password]",
    "send all [message]",
    "send [userID] [message]",
    "who",
    "logout"};