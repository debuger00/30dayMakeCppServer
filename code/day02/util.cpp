#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void errif(bool condition, const char *errmsg){ // 如果condition为true，则输出errmsg并退出程序 打印错误。
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}