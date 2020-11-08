#include <stdio.h>
#include "../include/server.h"

void credentialcheck_test(char *userID, char* password)
{
    printf("credentialcheck(%s, %s) = %d\n", userID, password, credential_doesMatch(userID, password));
}

void writeuser_test(char *userID, char* password)
{
    printf("write_newuser(%s, %s) = %d\n", userID, password, write_newuser(userID, password));
}

int main()
{
    credentialcheck_test("David", "David22");
    credentialcheck_test("Tom", "Tom11");
    credentialcheck_test("David", "David2");
    credentialcheck_test("Davi", "David2");

    writeuser_test("newuser", "newpassword");
    writeuser_test("newuser2", "newpassword2");
    writeuser_test("Beth", "Beth33");
}