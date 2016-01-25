#ifndef _LAB03_03_H_
#define _LAB03_03_H_

//
// This library provides safe integer and string functions;
// Every function definition must be implemented; in the function
// first of all you must check for overflow/underflow and must ensure
// to not give an incorrect value back from the function.
//

//
// Author: 
// Group: 
//

typedef enum SAFE_RESULT
{
    resOK,          // The operation was succesful
    resOVERFLOW,    // An overlow happened
    resUNDERFLOW,   // An underflow happened
    resDIVBYZERO,   // Division with 0 is not defined
};

// source - source null terminated string what will be copied into destination
// destination - destination buffer
// length - length of the destination buffer (with the null character)
SAFE_RESULT safe_strcpy(char* source, char* destination, size_t length);

// result == a+b if no error has occurred
SAFE_RESULT safe_intadd(int a, int b, int* result);
// result == a-b if no error has occurred
SAFE_RESULT safe_intsub(int a, int b, int* result);
// result == a*b if no error occurred
SAFE_RESULT safe_intmul(int a, int b, int* result);
// result == a/b if no error occurred
SAFE_RESULT safe_intdiv(int a, int b, int* result);

// result == a+b if no error has occurred
SAFE_RESULT safe_uintadd(unsigned int a, unsigned int b, unsigned int* result);
// result == a-b if no error has occurred
SAFE_RESULT safe_uintsub(unsigned int a, unsigned int b, unsigned int* result);
// result == a*b if no error occurred
SAFE_RESULT safe_uintmul(unsigned int a, unsigned int b, unsigned int* result);
// result == a/b if no error occurred
SAFE_RESULT safe_uintdiv(unsigned int a, unsigned int b, unsigned int* result);


#endif//_SAFE_FUNCTIONS_H_