#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

#define BUF_SIZE 1024*1024

#ifdef _WIN32
#include <Windows.h>
#pragma comment(lib, "advapi32.lib")

// On Windows only _snprintf is defined
#define snprintf _snprintf
#endif

void check_file_content()
{
    char filepath[100];
    char filename[100];
    char* buff = NULL;
    FILE* f = NULL;
    
    printf("File path: ");
    scanf("%99s", filename);
    
    snprintf(filepath, sizeof(filepath), ".\\%s.txt", filename);
    
    f = fopen(filepath, "r");
    if (NULL == f)
    {
        printf("File not found\n");
        goto cleanup;
    }
    
    buff = malloc(BUF_SIZE);
    if (NULL == buff)
    {
        goto cleanup;
    }
    
    if (0 == fread(buff, BUF_SIZE, 1, f))
    {
        goto cleanup;
    }
    
    printf("File content:\n%s", buff);
    
    cleanup:
    if (NULL != buff)
    {
        free(buff);
    }
    
    if (NULL != f)
    {
        fclose(f);
    }
}

#ifdef _WIN32
// Registry operations are Windows only
void check_registry_content()
{
    char regpath[100];
    char userguid[100];
    HKEY hKey = NULL;
    LONG res;
    
    printf("User GUID: ");
    scanf("%99s", userguid);
    
    // checking if it's a special user (must have a key in it's profile key)
    snprintf(regpath, sizeof(regpath), "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList\\%s\\Secret", userguid);
    
    res = RegOpenKeyA(HKEY_LOCAL_MACHINE, regpath, &hKey);
    if (ERROR_SUCCESS != res)
    {
        // the key is not in it's profile list, so we do not allow anything for it
        printf("Access denied for this user\n");
        printf("Key to user's profile: %s\n", regpath);
        goto cleanup;
    }
    
    printf("Access granted\n");
    printf("Key to user's profile: %s\n", regpath);
    
    cleanup:
    if (NULL != hKey)
    {
        RegCloseKey(hKey);
    }
}
#endif

void check_shell()
{
    char message[100];
    char command[200];
    
    fgetchar();
    
    printf("Message: ");
    scanf("%99[^\n]s", message);
    
    snprintf(command, sizeof(command), "echo \"Your message: %s\"", message);
    
    system(command);
}

int cb_user_query(void* ctx, int argc, char** argv, char** column)
{
    argv;
    column;
    
    if (argc > 0)
    {
        // a user was found
        *(int*)ctx = 1;
    }
    
    return 0;
}

void check_sql()
{
    char query[300];
    char username[100];
    char password[100];
    sqlite3* conn = NULL;
    char* errormsg;
    int found = 0;
    
    fgetchar();
    printf("Username: ");
    scanf("%99[^\n]s", username);
    fgetchar();
    printf("Password: ");
    scanf("%99[^\n]s", password);
    fgetchar();
    
    // Constructing SQL query
    snprintf(query, sizeof(query), "SELECT username, password FROM users WHERE username='%s' AND password='%s'", username, password);
    printf("QUERY string: %s\n", query);
    
    if (SQLITE_OK != sqlite3_open("database.db", &conn))
    {
        printf("database.db not found");
        goto cleanup;
    }
    
    sqlite3_exec(conn, query, cb_user_query, &found, &errormsg);
    if (NULL != errormsg)
    {
        printf("ERROR: %s\n", errormsg);
        goto cleanup;
    }
    
    if (found)
    {
        printf("Access granted!\n");
    }
    else
    {
        printf("Access denied!\n");
    }
    
    cleanup:
    if (NULL != conn)
    {
        sqlite3_close(conn);
    }
}

int main()
{
    printf("=== check_file_content ===\n");
    check_file_content();
    
#ifdef _WIN32
    printf("=== check_registry_content ===\n");
    check_registry_content();
#endif
    
    printf("=== check_shell ===\n");
    check_shell();
    
    printf("=== check_sql ===\n");
    check_sql();
}