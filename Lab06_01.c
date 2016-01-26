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

int check_for_chars(char* invalid_characters, char* input){
   char *c = mystring = input;
   while (*c)
   {
       if (strchr(invalid_characters, *c))
       {
          return 1; // not OK
       }

       c++;
   }
   return 0; // OK
}
char *str_replace(char *find , char *replace , char *str)
{
    char  *p = NULL , *old = NULL , *new_str = NULL ;
    int c = 0 , find_size;
     
    find_size = strlen(find);
     
    for(p = strstr(str , find) ; p != NULL ; p = strstr(p + find_size , find))
    {
        c++;
    }
     
    c = ( strlen(replace) - find_size )*c + strlen(str);
     
    new_str = malloc( c );
    
    if(new_str == NULL){
        return NULL;
    }

    strcpy(new_str , "");
     
    old = str;
     
    for(p = strstr(str , find) ; p != NULL ; p = strstr(p + find_size , find))
    {
        strncpy(new_str + strlen(new_str) , old , p - old);
        strcpy(new_str + strlen(new_str) , replace);
        old = p + find_size;
    }
     
    strcpy(new_str + strlen(new_str) , old);
     
    return new_str;
}

void check_file_content()
{
    char filepath[100];
    char filename[100];
    char* buff = NULL;
    FILE* f = NULL;
    
    printf("File path: ");
    scanf("%93s", filename);
    filename[93] = 0;

    if(check_for_chars("\\/" , filename) != 0){
        printf("Invalid char in filename\n");
        return;
    }

    // Validate filename for directory traverse it might be to larg cutting of the .txt from the end

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
    char userguid[36];
    HKEY hKey = NULL;
    LONG res;
    
    printf("User GUID: ");
    scanf("%35s", userguid);
    userguid[35] = 0;
    
    // checking if it's a special user (must have a key in it's profile key)

    if(check_for_chars("\\/" , filename) != 0){
        printf("Invalid char in filename\n");
        return;
    }
    //returns -1 on owerflow 
    // regpath size (userguid + SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList\\\\Secret) < 100
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
    
    getchar();
    
    printf("Message: ");
    scanf("%99[^\n]s", message);
    message[99] = 0;

    if(check_for_chars("\"&|<>`;" , message) != 0){
        printf("Invalid char in message\n");
        return;
    }

    // s = 123\"|| char.exe\" starts the calc.exe
    // check for : "&|<>`
    
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
    
    getchar();
    printf("Username: ");
    scanf("%99[^\n]s", username);
    getchar();
    printf("Password: ");
    scanf("%99[^\n]s", password);
    getchar();
    
    // Constructing SQL query
    char* pusername = str_replace("`","``",username);
    char* ppassword = str_replace("`","``", password);
    
    if(pusername == NULL || ppassword == NULL){
        printf("Could not process input\n");
        return;
    }
    
    if(300 - (70 + sizeof(pusername) + sizeof(ppassword)) < 0){
        printf("Input to long\n");
        return;
    }

    snprintf(query, sizeof(query), "SELECT username, password FROM users WHERE username='%s' AND password='%s'", pusername, ppassword);
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