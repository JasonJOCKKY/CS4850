#include <stdio.h>
#include "../include/client.h"

// Test Functions
int test_validate_command(char *input)
{
    printf("validate_command(%s) = %d;\n", input, validate_command(input));
    return 0;
}

// Main
int main(int argc, char const *argv[])
{
    test_validate_command("login tom tompass");
    test_validate_command("who");
    test_validate_command("logout");
    test_validate_command("send all jhdsfsfef");
    test_validate_command("newuser");
    test_validate_command("send tom uidfhifewf");
    test_validate_command("new user dsklfjs fodjhsf");
    test_validate_command("newuser jkfdh uidfhifewf");
    
    return 0;
}
