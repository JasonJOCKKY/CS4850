#include <stdio.h>
#include "../include/client.h"

// Helper functions
int print_error(char *error)
{
    return printf("<<Error>> %s\n", error);
}

int print_server(char *message)
{
    return printf("<Server> %s\n", message);
}

int print_prompt()
{
    return printf("> ");
}

// Client Main
int main(int argc, char const *argv[])
{
    // Connect to the chat room server.
    int client_socket = join_chatRoom(SERVER_PORT);
    if (client_socket == -1)
    { /* Fail to join the chat room. */
        print_error("Fail to join the chat room.");
        return -1;
    }

    // Handle User Commands
    char buf[MAX_LINE];
    while (1)
    {
        // Receive server message.
        switch (receive_message(client_socket, buf))
        {
        case 0: /* Successfully received a message. */
            // Print the message.
            print_server(buf);
            break;
        case -2: /* Lost connection to the server. */
            print_error("Lost connection to the server. Closing...");
            break;
        default: /* Error. */
            print_error("Fail to receive message from the server.");
            return -1;
        }

        // Get user command.
        print_prompt();
        if (get_input(&buf) == -1)
        { /* Fail to get user input. */
            print_error("Fail to get user input.");
            return -1;
        }

        // Validate user command.
        switch (validate_command(buf))
        {
        case 0: /* Command is valid. Send the command to the server. */
            if (send_request(client_socket, buf) == -1)
            { /* Error sending the request to the server. */
                print_error("Fail to send request to the server. Closing connection...");
                return -1;
            }
            break;
        case 1: /* Command is not valid. */
            print_error("The command is not valid.");
            break;
        case -2: /* Exit client */
            printf("Closing...\n");
            return 0;
        default: /* Error. */
            print_error("Fail to validate command.");
            return -1;
        }
    }

    return 0;
}
