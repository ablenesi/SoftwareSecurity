#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include <new>

char* trim(char *input, int length) {
    if(length < 0){
        return NULL;
    }

    char *output;
    char *message = (char*) malloc(sizeof(char)*(length + 1));

    if(message == NULL){
        printf("Couldn't allocate memory.");
        return NULL;
    }

    int index;

    // copy input string to a temporary string
    for (index = 0; index < length; index++) {
        message[index] = input[index];
    }
    message[index] = '\0';

    // trim trailing whitespace
    int len = index - 1;
    if(len < 0){
        return NULL;
    }
    while (len >= 0 && isspace(message[len])) {
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
    if(num < 2){
        printf("To small num value.");
        return;
    }

    if(num > INT_MAX / sizeof(int)){
        printf("To big num value.");
        return;   
    }

    int* nums = NULL;

    // PROBLEM 
    try{
        nums = new int[num];        
    }catch(std::bad_alloc& ba){
        printf("Couldn't allocate memory");
        return;
    }
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

    delete[]nums;
}

void fibo2(int num)
{
    if(num < 2){
        printf("To small num value.");
        return;
    }

    int* nums = NULL;

    nums = (int*)malloc(num * sizeof(int));
    
    if(nums == NULL){
        printf("Couldn't allocate memory.");
        return;
    }

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

    if(nums != NULL){
        free(nums);
    }
}

int main()
{
    char* input = (char*) "Hello World!    ";
    int   input2 = 10;
    char* trimmed = NULL;
    char* afterspace = NULL;
    size_t len;

    len = strlen(input);
    trimmed = trim(input, len);
    if(trimmed == NULL){
        printf("Failed at trim()");
        return 1;
    }

    printf("Original string: '%s'\nTrimmed string: '%s'\n", input, trimmed);

    afterspace = (char*)malloc(len * sizeof(char));
    if (NULL == afterspace)
    {
        printf("Insufficient resources\n");
        return 1;
    }

    if(find(input, ' ') > 0 && find(input, ' ') <= len){
        strncpy(afterspace, &input[find(input, ' ')], len);
    }else{
        printf("Wrong find return value.");
    }

    printf("String after the first space: '%s'\n", afterspace);

    fibo(input2);
    fibo2(input2);

    if(afterspace != NULL){
        free(afterspace);    
    }
    if(trimmed != NULL){
        free(trimmed);
    }

}