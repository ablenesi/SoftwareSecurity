#include "lab03_03.h"

// source - source null terminated string what will be copied into destination
// destination - destination buffer
// length - length of the destination buffer (with the null character)
SAFE_RESULT safe_strcpy(char* source, char* destination, size_t length){

	// the length must be at leat 1
	
	int len = strlen(source);
	int i = 0;

	while(i < len && i < length -1){
		destination[i] = source[i];
		i++;
	}
	destination[length] = '\0';

	// In case the destionation is shorter than the source
	if (len + 1 > length ){
		return resOVERFLOW;
	}

	if(len + 1 < length ){
		return resUNDERFLOW;
	}

	return resOK;
}

// result == a+b if no error has occurred
SAFE_RESULT safe_intadd(int a, int b, int* result){
	
	if(a > 0 && b > 0){
		if(INT_MAX - a < b){
			return resOVERFLOW;
		}
	}

	if(a < 0 && b < 0){
		if(INT_MIN - a > b){			
			return resUNDERFLOW;
		}
	}

	*result = a + b;
	return resOK;
}

// result == a-b if no error has occurred
SAFE_RESULT safe_intsub(int a, int b, int* result){
	return safe_intadd(a, b * -1, result);
}

// result == a*b if no error occurred
SAFE_RESULT safe_intmul(int a, int b, int* result){

	if((a == 0) || (b == 0)){
		*result = 0;
		return resOK;
	}

	if(a > 0 && b > 0){
		if(INT_MAX / a < b){
			return resOVERFLOW;
		}
	}

	if(a < 0 && b < 0){
		if(INT_MAX / a > b){
			return resOVERFLOW;
		}
	}

	if(a > 0 && b < 0){
		if(INT_MIN / a > b){
			return resUNDERFLOW;
		}
	}
	
	if(a < 0 && b > 0){
		if(INT_MIN / a < b){
			return resUNDERFLOW;
		}
	}

	*result = a * b;
	return resOK;
}

// result == a/b if no error occurred
SAFE_RESULT safe_intdiv(int a, int b, int* result){
	if( b == 0 ){
		return resDIVBYZERO;
	}

	*result = a / b;
	return resOK;
}

// result == a+b if no error has occurred
SAFE_RESULT safe_uintadd(unsigned int a, unsigned int b, unsigned int* result){
	if(UINT_MAX - b < a){
		return resOVERFLOW;
	}

	*result = a + b;
	return resOK;
}

// result == a-b if no error has occurred
SAFE_RESULT safe_uintsub(unsigned int a, unsigned int b, unsigned int* result){
	if(b < a && b != 0){
		return resUNDERFLOW;
	}
	if(a == 0 && b == UINT_MAX){
		return resUNDERFLOW;
	}
	*result = a - b;
	return resOK;
}

// result == a*b if no error occurred
SAFE_RESULT safe_uintmul(unsigned int a, unsigned int b, unsigned int* result){
	if((a == 0) || (b == 0)){
		*result = 0;
		return resOK;
	}

	if(UINT_MAX / a < b){
		return resOVERFLOW;
	}

	*result = a * b;
	return resOK;
}
// result == a/b if no error occurred
SAFE_RESULT safe_uintdiv(unsigned int a, unsigned int b, unsigned int* result){
	if( b == 0 ){
		return resDIVBYZERO;
	}

	*result = a / b;
	return resOK;
}
