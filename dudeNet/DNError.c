#include "DNError.h"
#include <stdio.h>


err_t DNError;

#if defined(DN_ERROR_VERBOSE) && defined(PRINTABLE) 
    const char* DNError_func; //indicates in which function error occurred
#endif /* DN_ERROR_VERBOSE */


#if defined(DN_ERROR_VERBOSE) && defined(PRINTABLE) 
    const char* DNError_verbose[N_ERROR] = {
        "No error",
        "Timeout",
        "Packet is directed to another device",
        "Unexpected sender",
        "Wrong CRC"
    };
#endif /* DN_ERROR_VERBOSE */

#if defined(DN_ERROR_VERBOSE) && defined(PRINTABLE) 
    // print on stderr:
    //      <message>   \t      <error_func>: <error_verbose[DNError]>
    void DNError_infoError(const char* message){
        fprintf(stderr, "\033[1;31m"); //bold red

        if(message == NULL) fprintf(stderr, "*** %s: %s ***\n", DNError_func, DNError_verbose[DNError]);
        else fprintf(stderr, "*** %s\t%s: %s ***\n", message, DNError_func, DNError_verbose[DNError]);
    
        fprintf(stderr,"\033[0m"); //remove color

    }

#endif /* DN_ERROR_VERBOSE */