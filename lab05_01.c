#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_strncpy()
{
    char buff[10];
    
    strncpy(buff, "1234567890", sizeof(buff));
    buff[9] = '\0';

    printf("%s\n", buff);
 }

void test_strncat()
{
    char buff[10];
    
    strncpy(buff, "1234567", sizeof(buff));
    strncat(buff, "1234", sizeof(buff) - strlen(buff) - 1);
    
    printf("%s\n", buff);
}

// Copied from FreeBSD man pages:
//     The strlcpy() and strlcat() functions copy and concatenate strings
//     respectively.  They are designed to be safer, more consistent, and less
//     error prone replacements for strncpy(3) and strncat(3).  Unlike those
//     functions, strlcpy() and strlcat() take the full size of the buffer (not
//     just the length) and guarantee to NUL-terminate the result (as long as
//     size is larger than 0 or, in the case of strlcat(), as long as there is
//     at least one byte free in dst).  Note that a byte for the NUL should be
//     included in size. Also note that strlcpy() and strlcat() only operate on
//     true "C'" strings.  This means that for strlcpy() src must be NUL-termi-
//     nated and for strlcat() both src and dst must be NUL-terminated.
//
//     The strlcpy() function copies up to size - 1 characters from the NUL-ter-
//     minated string src to dst, NUL-terminating the result.
//
//     The strlcat() function appends the NUL-terminated string src to the end
//     of dst.  It will append at most size - strlen(dst) - 1 bytes, NUL-termi-
//     nating the result.

size_t strlcpy(char* destination, char* source, size_t dest_size)
{
    size_t size = 0;

    if(dest_size == 0){
        return strlen(source);
    }

    while ((size < (dest_size - 1)) && *source)
    {
        *destination = *source;
        destination++;
        source++;
        size++;
    }
    
    *destination = '\0';
    return size + strlen(source);
}

size_t strlcat(char* destination, char* source, size_t dest_size)
{

    size_t size = 0;
    if(dest_size == 0){
        return strlen(source);
    }

    while (*destination)
    {
        destination++;
        size++;
    }
    
    while ((size < (dest_size -1)) && *source)
    {
        *destination = *source;
        destination++;
        source++;
        size++;
    }
    
    *destination = '\0';

    return size + strlen(source);
}

void test_strlcpy()
{
    char buff[10];
    size_t n;
    
    if ((n = strlcpy(buff, "123456789", sizeof(buff))) >= sizeof(buff))
    {
        printf("tst_cpy>strlcpy source too long\n");
        return;
    }else{
        printf("%s\n", buff);
    }
    
    // if (strlcpy(buff+n, "123", sizeof(buff)) >= sizeof(buff))
    // {
    //     printf("tst_cpy>strlcpy second problem\n");
    //     return;
    // }
}

void test_strlcat()
{
    char buff[10];
    size_t n;
    
    if ((n = strlcpy(buff, "123456789", sizeof(buff))) >= sizeof(buff))
    {
        printf("tst_cat>strlcpy source too long\n");
        return;
    }
    
    if (strlcat(buff, "123", sizeof(buff)) >= sizeof(buff))
    {
        printf("tst_cat>strlcat overflow\n");
        return;
    }
}

int main()
{
    test_strncpy();
    test_strncat();
    test_strlcpy();
    test_strlcat();
}