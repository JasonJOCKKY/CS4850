#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/client.h"

int join_chatRoom(int serverPort)
{
    // Validate parameters
    if (serverPort < 0)
    {
        return -1;
    }

    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        // Fail to create client socket.
        return -1;
    }

    // Structure of socket address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(serverPort);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connect to socket
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        // Connection error
        return -1;
    }

    return client_socket;
}

int get_input(char *buffer)
{
    // Validate Parameter
    if (!buffer)
    {
        return -1;
    }

    if (fgets(buffer, MAX_LINE, stdin) == NULL)
    { /* Error reading user input. */
        return -1;
    }

    // Trim the newline character at the end
    if (buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = 0;
    }

    return 0;
}

int validate_command(char *input_string)
{
    // Validate Parameters
    if (!input_string)
    {
        return -1;
    }

    // Parameters for tokenize the strings.
    char *input_token, *list_token;
    char *saveptr_input, *saveptr_list;
    char delim[5] = " \n";

    // Command strings
    char input_command[MAX_LINE] = "";
    char list_command[MAX_LINE] = "";

    // Match the command with a command from the list
    for (int i = 0; i < NUM_COMMANDS; i++)
    {
        // Get the length of the command
        size_t command_length;
        for (command_length = 0; command_length < strlen(COMMAND_LIST[i]); command_length++)
        {
            if (COMMAND_LIST[i][command_length] == '[')
            {
                command_length -= 1;
                break;
            }
        } // if there is no parameter, the command length will be exactly length of the list entry.

        // Construct commands
        strncpy(input_command, input_string, command_length);
        strncpy(list_command, COMMAND_LIST[i], command_length);

        // Compare commands
        if (strcmp(input_command, list_command) == 0)
        {
            // Command asks to exit
            if (i == NUM_COMMANDS - 1)
            {
                return -2;
            }

            // Check if the input matches the entry
            int does_match = 0;
            list_token = strtok_r(COMMAND_LIST[NUM_COMMANDS], delim, &saveptr_list);
            input_token = strtok_r(input_string, delim, &saveptr_input);
            while (list_token)
            {
                list_token = strtok_r(NULL, delim, &saveptr_list);
                input_token = strtok_r(NULL, delim, &saveptr_input);
                if (list_token && !input_token)
                { /* The input does not match the list entry. */
                    does_match = -1;
                    break;
                }
            }

            if (does_match == 0)
            { /* The input string is valid. */
                return 0;
            }
        }
    }

    // The command doesn't match any entry.
    return 1;
}

int validate_newUser(char *newID, char *newPassword);

int send_request(int socket, char *request_string);

int receive_message(int socket, char *buffer);