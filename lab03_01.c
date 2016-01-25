#include <windows.h>
#include <stdio.h>

#define CFG_FILE_NAME TEXT("lab03_01.cfg")
#define CFG_MAX_SIZE 512

int main()
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    CHAR* content = NULL;
    CHAR* str = NULL;
    DWORD dwRead = 0;

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
        if (hFile = INVALID_HANDLE_VALUE)
        {
            printf("File can not be opened\n");
            __leave;
        }

        content = malloc(CFG_MAX_SIZE);
        str = malloc(CFG_MAX_SIZE);

        if (!ReadFile(hFile, content, CFG_MAX_SIZE, &dwRead, NULL))
        {
            printf("Read failed!\n");
            __leave;
        }

        strcpy(str, content);

        /// ... do whatever you want with the strings
    }
    __finally
    {
        if (NULL != hFile)
        {
            CloseHandle(hFile);
        }

        free(content);
        free(str);
    }
}