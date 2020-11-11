#include <stdio.h>
#include <pthread.h>
#include "../include/client.h"

#include <sys/socket.h>

int connection_established = 0; /* 1 for yes, 0 for no. */

// Thread Methods
void *response_handler(void *socket)
{
    char buf[MAX_LINE];
    while (connection_established == 1)
    {
        // Keeps receieving from the server.
        switch (recv(*((int *)socket), buf, MAX_LINE, 0))
        {
        case 0: /* Successfully received a message. */
            print_server(buf);
            break;
        case -2: /* Lost connection to the server. */
            print_error("Lost connection to the server. Closing...");
            connection_established = -1;
            break;
        default: /* Error. */
            print_error("Fail to receive message from the server.");
            connection_established = -1;
        }
    }

    return NULL;
}

void *input_handler(void *params)
{
    char buf[MAX_LINE];
    while (connection_established == 1)
    {
        // Get user input
        if (get_input(buf) == -1)
        { /* Fail to get user input. */
            print_error("Fail to get user input.");
            return NULL;
        }

        // Process user command.
        switch (command_processer(client_socket, buf))
        {
        case 0: /* Valid server request. Receive server message.*/
            break;
        case 1: /* Log out */
            printf("Logging out...\n");
            return 0;
        case -2: /* Invalid Request */
            print_error("Invalid Request");
            break;
        default: /* Error */
            print_error("Error proccessing command.");
            return -1;
        }
    }
    return NULL;
}

// Client Main
int main(int argc, char const *argv[])
{
    // Connect to the chat room server.
    int client_socket = join_chatRoom(SERVER_PORT);
    if (client_socket == -1)
    {
        print_error("Fail to join the chat room.");
        return -1;
    }

    char buf[MAX_LINE]; /* Buffer for messages. */

    // Receieve initial server message
    if (receive_message(client_socket, buf) != 0)
    {
        print_error("Fail to receieve initial message from the server. Shutting down...");
        return -1;
    }
    print_server(buf);

    // Handle User Commands
    while (1)
    {
        // Get user command.
        print_prompt();
        if (get_input(buf) == -1)
        { /* Fail to get user input. */
            print_error("Fail to get user input.");
            return -1;
        }

        // Process user command.
        switch (command_processer(client_socket, buf))
        {
        case 0: /* Valid server request. Receive server message.*/
            switch (receive_message(client_socket, buf))
            {
            case 0: /* Successfully received a message. */
                print_server(buf);
                break;
            case -2: /* Lost connection to the server. */
                print_error("Lost connection to the server. Closing...");
                break;
            default: /* Error. */
                print_error("Fail to receive message from the server.");
                return -1;
            }
            break;
        case 1: /* Log out */
            printf("Logging out...\n");
            return 0;
        case -2: /* Invalid Request */
            print_error("Invalid Request");
            break;
        default: /* Error */
            print_error("Error proccessing command.");
            return -1;
        }
    }

    return 0;
}
