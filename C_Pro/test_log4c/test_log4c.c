#include <stdio.h>
#include <log4c.h>
#include <layout.h>

#if 1
int main() {

    log4c_category_t* mycat = NULL;
    if (log4c_init()){
        printf("log4c_init() failed\n");
    }else{
        mycat = log4c_category_get("output_file");

        log4c_category_log(mycat, LOG4C_PRIORITY_ERROR, "Hello World!");

        /* Explicitly call the log4c cleanup routine */
        if (log4c_fini()){
            printf("log4c_fini() failed\n");
        }
    }
    return 0;
}
#else

#endif