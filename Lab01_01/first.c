//
// This sample program is created by Fulop Botond
//
// Course: Secure Coding
// Lab 1
// Assignment 1
//
// Name: 
// Group: 
//

//
// Description: The aim of this program is to read a string from the standatd input and to
//   reverse the order of the characters in every word or number.
//
// Example:
// Input:  Hello World!
// Output: olleH dlroW!
//

#include <stdlib.h>         // malloc, free
#include <stdio.h>          // printf, scanf
#include <string.h>         // strlen
#include <ctype.h>          // isalpha
#include "stack.h"          // This is our stack implementation

int main()
{
    STACK stack;            // stack used to reorder the characters in the words
    char buffer[200];       // input buffer
	//char* buffer;
	char* output = "";           // output string
    long length = 0;            // length of the string
    int i;                 // input index variable
    int o;                 // output index variable
    char c;                 // character from the stack
	int size = 199;
	int error;

    //
    // Initializations
    //
    StackInit(&stack);
    
	//buffer = (char*)malloc(size*sizeof(char));

    //
    // Reading a string from the standard input
    //

    printf("Input: ");
	
	if (scanf_s("%[^\n]s", buffer, size) == 0) {
		printf("Error!\n");
		exit(0);
	}// reading a string until the first newline character

	printf("Read:  %s\n", buffer);  // printing out the original buffer
    
    length = strlen(buffer);        // calculating the length of string
    printf("Length of string: %d\n", length);

	/*if (length > 200){
		printf("Error!\n");
		exit(0);
	}*/

    if (length == 0)                // if the string is empty we have nothing to do
    {
        printf("No input string!");
        goto cleanup;
    }
	
    output = malloc(length+1);      // allocating a character in plus, because the length does not include the ending \0
    memset(output, '\0', length+1); // filling the buffer with null characters

    //
    // Reversing the words and storing them in the output buffer
    //

    o = 0;                          // setting the current output buffer's index to 0
    for(i=0; i<=length; i++)        // traversing the string and processing every character
    {
        if (!isalnum(buffer[i]))    // if it's not alphanumeric we pop everything from the stack and store these characters in the output
        {
            while ('\0' != (c = StackPop(&stack)))  // popping the top character from the string (if there is any)
            {
                output[o++] = c;    // storing the actual character on the end of the output string
            }
            output[o++] = buffer[i];// storing the non alphanumeric character too
        }
        else
        {
            error = StackPush(&stack, buffer[i]);    // it's the part of the word, so we need to put on the stack
			if (error == 0){
				printf("Error: Stack index!\n");
				exit(0);
			}
		}
    }

    printf("Output: %s\n", output); // printing the output string

cleanup:
    //
    // Cleaning up the memory on exit
    //

    if (NULL != output)             // if memory was allocated for the output buffer we need to free it
    {
        free(output);
    }

    return 0;
}