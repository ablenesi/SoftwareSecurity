#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef void* (__stdcall *PDirect3DCreate9)(UINT);

void CreateProcessTest()
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    memset(&pi, 0, sizeof(pi));

    CreateProcessA(NULL, "\"c:\\Program Files\\Windows Defender\\MSASCui.exe\"",
        NULL, NULL,
        FALSE,
        NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &si,
        &pi);
}

void LoadLibraryTest()
{
    HMODULE d3d9;
    PDirect3DCreate9 Direct3DCreate9 = NULL;

    d3d9 = LoadLibraryExA("d3d9.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (NULL == d3d9)
    {
        printf("d3d9.dll not found");
        return;
    }
    printf("D3d9 loaded at %p\n", d3d9);

    Direct3DCreate9 = (PDirect3DCreate9)GetProcAddress(d3d9, "Direct3DCreate9");
    if (NULL == Direct3DCreate9)
    {
        printf("Function Direct3DCreate9 not found\n");
        goto cleanup;
    }

    Direct3DCreate9(123);

cleanup:
    FreeLibrary(d3d9);
}

int clean(char* buffer){
    int i = 0;
    
    if(buffer == null){
        return 1;
    }
    int size = strlen(buffer);

    if(size == 0){
        return 1;
    }
    
    if( (buffer[0] == ":") ||
        (buffer[0] == "\\" && buffer[1] == "\\")) || 
        (buffer[0] == "\\" && buffer[1] == "." && buffer[2] == "\\")){
        return 1;
    }

    if(size >= 2){
        if(isalpha(buffer[0]) && buffer[1] == ":" && buffer[2] == "\\"){
            for(int i = 5; i< size; i++){
                if(buffer[i] == ":"){
                    return 1;
                }
            }
        }

        if(size >=5){
            if((buffer[0] == "\\" && buffer[1] == "?" && buffer[2] == "\\" && isalpha(buffer[3]) && buffer[4] == ":" && buffer[5] == "\\"){
                for(int i = 5; i< size; i++){
                    if(buffer[i] == ":"){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void CreateFileTest()
{
    char buffer[MAX_PATH];
    HANDLE hFile = INVALID_HANDLE_VALUE;

    printf("File path: ");
    scanf_s("%[^\n]s", buffer, _countof(buffer));
    buffer[sizeof(buffer)-1] = 0;
    getchar();

    int i = strlen(buffer);
    while(i>2 && (buffer[i-1] == "." || buffer[i] =="\\"){
        i--;
    }
    buffer[i] = "\0";

    if(clean(buffer) == 0 ){
        hFile = CreateFileA(
            buffer,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (INVALID_HANDLE_VALUE == hFile)
        {
            printf("The file could not be opened: %d\n", GetLastError());
            return;
        }

        if (INVALID_HANDLE_VALUE != hFile)
        {
            CloseHandle(hFile);
        }
    }else{
        printf("Incorrect file path.");
    }
}

int main()
{
    CreateFileTest();
    CreateProcessTest();
    LoadLibraryTest();
}