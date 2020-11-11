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
        case -1: /* Error. */
            print_error("Fail to receive message from the server.");
            connection_established = -1;
            break;
        case 0: /* Lost connection to the server. */
            print_error("Lost connection to the server. Closing...");
            connection_established = -1;
            break;
        default: /* Successfully received a message. */
            print_server(buf);
        }
    }

    return NULL;
}

void *input_handler(void *socket)
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
        switch (command_processer(*((int *)socket), buf))
        {
        case 1: /* Log out */
            printf("Logging out...\n");
            connection_established = 0;
            break;
        case -1: /* Error */
            print_error("Error proccessing command!");
            connection_established = -1;
            break;
        case -2: /* Invalid Request */
            print_error("Invalid Request!");
            break;
        default: /* Success */
            break;
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

    // Input and output threads
    connection_established = 1;
    pthread_t tInput, tResponse;
    pthread_create(&tInput, NULL, &input_handler, &client_socket);
    pthread_create(&tResponse, NULL, &response_handler, &client_socket);
    
    while (connection_established == 1) ;

    pthread_cancel(tInput);
    pthread_cancel(tResponse);

    return 0;
}
