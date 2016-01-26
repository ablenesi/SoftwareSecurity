#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_FILENAME 256
#define SIZE_OF_PATTERN  256

long int get_file_size(FILE* file)
{
    int res = 0;
    fpos_t orig_pos;
    fpos_t size_of_file;
    char pos_modified = 0;
    long int returnvalue = 0;

    res = fgetpos(file, &orig_pos);   // getting the original position of the file cursor
    if (res)
    {
        // fgetpos failed
        goto cleanup;
    }

    res = fseek(file, 0, SEEK_END);
    if (res)
    {
        // seek failed
        goto cleanup;
    }
    pos_modified = 1;

    res = fgetpos(file, &size_of_file); // getting where is the end of file
    if (res)
    {
        goto cleanup;
    }

    returnvalue = (long int)size_of_file; // works on windows, on linux it's a structure

cleanup:
    if (pos_modified)
    {
        fsetpos(file, &orig_pos);    // setting back the original cursor position
    }

    return (long int)size_of_file;
}

int scanfile(const char* filename, const char* pattern)
{
    FILE* f = NULL;
    char* content = NULL;
    long int size_of_file = 0;
    int res;
    char* i = NULL;
    int ptrn_len;
    int found = 0;

    f = fopen(filename, "rb");
    if (NULL == f)
    {
        printf("File '%s' canot be opened\n");
        goto cleanup;
    }

    // the file was opened, we must get the size of file
    size_of_file = get_file_size(f);
    if (!size_of_file)
    {
        // empty file or some error happened while getting the size of file
        goto cleanup;
    }

    content = (char*)malloc(size_of_file);
    if (NULL == content)
    {
        goto cleanup;
    }

    res = fread(content, size_of_file, 1, f); // visszateriti, h hany byte-ot tudott kiolvasni
    if (!res)
    {
       
        goto cleanup;
    }

    ptrn_len = strlen(pattern);

	// kell az if (ellenorzeskent)
	if (size_of_file < ptrn_len){
		goto cleanup; // kilepes
	}
    
	for (i = content; i <= content + size_of_file - ptrn_len; i++) // hiba: kell az = , mivel nem olvassa az utolso karaktert
    {
        if (!memcmp(i, pattern, ptrn_len))
        {
            // the pattern found
            found = 1;
            goto cleanup;
        }
    }

cleanup:
    if (NULL != content)
    {
        free(content);
    }

    if (NULL != f)
    {
        fclose(f);
    }

    return found;
}

int main(int argc, char** argv)
{
    char filename[SIZE_OF_FILENAME];
    char pattern[SIZE_OF_PATTERN];
    char found = 0;

    if (3 != argc)
    {
        printf("Usage: lab02_01 <filename> <pattern_to_search>\n");
        exit(-1);
    }

    // getting the filename and the pattern from the arguments
    strncpy(filename, argv[1], (sizeof(filename) - 1)); // hiba: "-1" kell
    strncpy(pattern, argv[2], (sizeof(pattern) - 1)); // hiba: "-1" kell

    found = scanfile(filename, pattern);
    if (found)
    {
        printf("pattern FOUND in file\n");
    }
    else
    {
        printf("pattern NOT FOUND\n");
    }

    return found;
}