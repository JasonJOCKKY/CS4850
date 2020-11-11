#include <stdio.h>
#include "../include/client.h"

// Test Functions
int test_validate_command(char *input)
{
    printf("validate_command(%s) = %d;\n", input, validate_command(input));
    return 0;
}

int test_validate_newuser(char *input)
{
    printf("validate_newuser(%s) = %d;\n", input, validate_newUser(input));
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

    test_validate_newuser("login tom tompass");
    test_validate_newuser("who");
    test_validate_newuser("logout");
    test_validate_newuser("send all jhdsfsfef");
    test_validate_newuser("newuser");
    test_validate_newuser("send tom uidfhifewf");
    test_validate_newuser("new user dsklfjs fodjhsf");
    test_validate_newuser("newuser jkfdh uidfhifewf");
    test_validate_newuser("newuser jkfdh uidfh");

    char buf[MAX_LINE];
    printf("Input > ");
    get_input(buf);
    printf("Buf = '%s'\n", buf);
    
    return 0;
}
