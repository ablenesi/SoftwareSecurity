#include "lab03_03.h"
#include <windows.h>

int failed = 0;

#define _assert(cond, x) if (cond == false) {\
                    printf("Assetion failed: %s\n", x);\
                    failed++;                           \
                  }
                  
#define _assert_v(cond, x) if (cond == false) {\
                    printf("Assetion failed: %s\n", x);\
                    failed++;                           \
                  }

#define _safe_assert(x, __assert_func)                      \
                    {   bool __res = false, __exc = false;  \
                        __try {                             \
                            __res = (x);                    \
                        }__except(EXCEPTION_EXECUTE_HANDLER)\
                        { __exc = false;}                   \
                        if (__exc){                         \
                            printf("Exception occurred: %s\n", #x);\
                        } else {                            \
                            __assert_func(__res, #x);       \
                        }                                   \
                    }
                  
#define safe_assert(x)   _safe_assert(x, _assert)
#define safe_assert_v(x) _safe_assert(x, _assert_v)

#define assert(x) safe_assert(x)
#define assert_v(x) safe_assert_v(x)

void safe_str()
{
    char buff[0x1000];
    
    assert_v(safe_strcpy("", buff, 0) == resOVERFLOW);
    assert_v(safe_strcpy("12", buff, 3) == resOK);
    assert_v(safe_strcpy("12", buff, 2) == resOVERFLOW);
    assert_v(safe_strcpy("", buff, 0x1000) == resOK);
}

void safe_int()
{
    int ires;
    
    assert(safe_intadd(1, INT_MAX, &ires) == resOVERFLOW);
    assert(safe_intadd(INT_MAX, 1, &ires) == resOVERFLOW);
    
    assert(safe_intadd(INT_MIN, -1, &ires) == resUNDERFLOW);
    assert(safe_intadd(-1, INT_MIN, &ires) == resUNDERFLOW);
    
    assert(safe_intadd(INT_MIN+1, -1, &ires) == resOK);
    assert(safe_intadd(-1, INT_MIN+1, &ires) == resOK);
    
    assert(safe_intadd(INT_MAX-1, 1, &ires) == resOK);
    assert(safe_intadd(1, INT_MAX-1, &ires) == resOK);
    
    assert(safe_intadd(INT_MAX, 0, &ires) == resOK);
    assert(safe_intadd(0, INT_MAX, &ires) == resOK);
    
    assert(safe_intadd(INT_MAX/2, INT_MAX/2, &ires) == resOK);
    assert(safe_intadd(INT_MIN/2, INT_MIN/2, &ires) == resOK);
    
    assert(safe_intadd(INT_MIN, 0, &ires) == resOK);
    assert(safe_intadd(0, INT_MIN, &ires) == resOK);
    
    assert(safe_intsub(-1, 1, &ires) == resOK);
    
    assert(safe_intsub(1, -INT_MAX, &ires) == resOVERFLOW);
    assert(safe_intsub(INT_MAX, -1, &ires) == resOVERFLOW);
    
    assert(safe_intsub(INT_MIN,   1, &ires) == resUNDERFLOW);
    
    assert(safe_intsub(INT_MIN+1,     1, &ires) == resOK);
    assert(safe_intsub(-1, -(INT_MIN+1), &ires) == resOK);
    
    assert(safe_intsub(INT_MAX-1,   -1, &ires) == resOK);
    assert(safe_intsub(1, -(INT_MAX-1), &ires) == resOK);
    
    assert(safe_intsub(INT_MAX, 0, &ires) == resOK);
    assert(safe_intsub(0, INT_MAX, &ires) == resOK);
    
    assert(safe_intsub(INT_MIN, 0, &ires) == resOK);
    assert(safe_intsub(0, INT_MIN, &ires) == resOK);

    assert(safe_intmul(INT_MAX/2, 2, &ires) == resOK);
    assert(safe_intmul((INT_MAX/2)-1, 2, &ires) == resOK);
    assert(safe_intmul((INT_MAX/2)+1, 2, &ires) == resOVERFLOW);
    
    assert(safe_intmul(INT_MIN/2, 2, &ires) == resOK);
    assert(safe_intmul((INT_MIN/2)-1, 2, &ires) == resUNDERFLOW);
    
    assert(safe_intmul(INT_MAX, 0, &ires) == resOK);
    assert(safe_intmul(0, INT_MAX, &ires) == resOK);
    assert(safe_intmul(INT_MIN, 0, &ires) == resOK);
    assert(safe_intmul(0, INT_MIN, &ires) == resOK);
    
    assert(safe_intdiv(INT_MAX, 1, &ires) == resOK);
    assert(safe_intdiv(INT_MAX, INT_MAX, &ires) == resOK);
    assert(safe_intdiv(INT_MAX, 0, &ires) == resDIVBYZERO);
}

void safe_uint()
{
    unsigned int res;
    
    assert(safe_uintadd(1, UINT_MAX, &res) == resOVERFLOW);
    assert(safe_uintadd(UINT_MAX/2, UINT_MAX/2, &res) == resOK);
    assert(safe_uintadd(UINT_MAX, 1, &res) == resOVERFLOW);
    assert(safe_uintadd(0, UINT_MAX, &res) == resOK);
    
    assert(safe_uintsub(0, 0, &res) == resOK);
    assert(safe_uintsub(1, 1, &res) == resOK);
                                                 
    assert(safe_uintsub(UINT_MAX, 0, &res) == resOK);
    assert(safe_uintsub(0, UINT_MAX, &res) == resUNDERFLOW);
    
    assert(safe_uintmul((UINT_MAX/2)+1, 2, &res) == resOVERFLOW);
    assert(safe_uintmul(0, 2, &res) == resOK);
    assert(safe_uintmul(2, 0, &res) == resOK);

    assert(safe_uintdiv(UINT_MAX, 1, &res) == resOK);
    assert(safe_uintdiv(UINT_MAX, UINT_MAX, &res) == resOK);
    assert(safe_uintdiv(UINT_MAX, 0, &res) == resDIVBYZERO);
}

int main()
{
    int ires;
    int i=0;
    
    printf("Integers\n");
    printf("========\n");
    safe_int();
    printf("Uintegers\n");
    printf("=========\n");
    safe_uint();
    printf("Strings\n");
    printf("=======\n");
    safe_str();
    
    
    printf("================================\nFailures: %d", failed);
}