#include "dudeNet.h"
#include "DNRouting.h"

#ifdef __linux__
    #include <stdio.h>
#endif

#ifdef __linux__
    void dudeNet_atexit(void){
        int res = DNRouting_close();
        if(res == -1) perror("Close");
    }
#endif