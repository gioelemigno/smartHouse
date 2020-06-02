#pragma once

#include <stdint.h>

#define DN_ERROR_VERBOSE
#define PRINTABLE

// result of function, if error occurred a function return -1 and set error variable according to
typedef int8_t res_t;

#if defined(DN_ERROR_VERBOSE) && defined(PRINTABLE) 
extern const char* DNError_func; //indicates in which function error occurred
#endif /* DN_ERROR_VERBOSE */

#define N_ERROR     5
typedef enum err_t{
    ERR_NO_ERROR    =   0,  // used as initial value
    ERR_TIMEOUT     =   1,
    ERR_DST,
    ERR_SRC,
    ERR_CRC
}err_t;

extern err_t DNError;

#if defined(DN_ERROR_VERBOSE) && defined(PRINTABLE) 
    extern const char* DNError_verbose[N_ERROR];

    // print on stderr:
    //      <message>   -      <error_func>: <error_verbose[DNError]>
    void DNError_infoError(const char* message);
#endif /* DN_ERROR_VERBOSE */
