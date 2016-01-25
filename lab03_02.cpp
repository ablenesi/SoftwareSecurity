#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>

char* trim(char *input, int length) {
    char *output;
    char *message = (char*)malloc(sizeof(char)*(length + 1));
    int index;

    // copy input string to a temporary string
    for (index = 0; index < length; index++) {
        message[index] = input[index];
    }
    message[index] = '\0';

    // trim trailing whitespace
    int len = index - 1;
    while (isspace(message[len])) {
        message[len] = '\0';
        len--;
    }

    // return string without trailing whitespace
    output = message;
    return output;
}

int find(char* input, char c)
{
    int index = 0;

    while ('\0' != input[index])
    {
        if (input[index] == c)
        {
            return index;
        }
        index++;
    }

    return -1;
}

void fibo(int num)
{
    int* nums = NULL;

    nums = new int[num];
    nums[0] = 1;
    nums[1] = 1;

    printf("1 1 ");
    for (int i = 2; i < num; i++)
    {
        // calculating the i'th fibo number
        nums[i] = nums[i - 2] + nums[i - 1];
        printf("%d ", nums[i]);
    }
    printf("\n");
}

void fibo2(int num)
{
    int* nums = NULL;

    nums = (int*)malloc(num);
    nums[0] = 1;
    nums[1] = 1;

    printf("1 1 ");
    for (int i = 2; i < num; i++)
    {
        // calculating the i'th fibo number
        nums[i] = nums[i - 2] + nums[i - 1];
        printf("%d ", nums[i]);
    }
    printf("\n");
}

int main()
{
    char* input = "Hello World!    ";
    int   input2 = 10;
    char* trimmed = NULL;
    char* afterspace = NULL;
    size_t len;

    len = strlen(input);
    trimmed = trim(input, len);

    printf("Original string: '%s'\nTrimmed string: '%s'\n", input, trimmed);

    afterspace = (char*)malloc(len);
    if (NULL == afterspace)
    {
        printf("Insufficient resources\n");
        return 1;
    }

    strncpy(afterspace, &input[find(input, ' ')], len);
    printf("String after the first space: '%s'\n", afterspace);

    fibo(input2);
    fibo2(input2);
}