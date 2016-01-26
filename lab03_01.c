#include <windows.h>
#include <stdio.h>

#define CFG_FILE_NAME TEXT("lab03_01.cfg")
#define CFG_MAX_SIZE 512

int main()
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    CHAR* content = NULL;
    CHAR* str = NULL;
    DWORD dwRead = 0; //unsigned long

    __try
    {
        hFile = CreateFile(CFG_FILE_NAME,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );
        // MISTAKE  NR. 1
        if (hFile == INVALID_HANDLE_VALUE) // = => ==
        {
            printf("File can not be opened\n");
            __leave;
        }

        content = malloc(CFG_MAX_SIZE);
        // MISTAKE  NR. 2
        // We have to test the return value of the malloc
        if(content == NULL){
            printf("Couldn't allocate memory for content.\n");
            __leave;
        }

        str = malloc(CFG_MAX_SIZE);
        // MISTAKE  NR. 3
        // We have to test the return value of the malloc
        if(str == NULL){
            printf("Couldn't allocate memory for str.\n");
            __leave;
        }

        if (!ReadFile(hFile, content, CFG_MAX_SIZE, &dwRead, NULL))
        {
            printf("Read failed!\n");
            __leave;
        }

        // MISTAKE  NR. 4

        strncpy(str, content, CFG_MAX_SIZE - 1);

        /// ... do whatever you want with the strings
    }
    __finally
    {
        if (INVALID_HANDLE_VALUE != hFile) // if(NULL != hFile) MISTAKE
        {
            CloseHandle(hFile);
        }

        // We have to check for NULL values
        if(NULL != content){
            free(content);    
        }
        
        // We have to check for NULL values
        if(NULL != str){
           free(str);
        }
    }
}