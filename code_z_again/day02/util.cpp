#include <stdio.h>
#include "util.h"

void errif(bool condition, const char *s)
{
    if (condition)
    {
        perror(s);
    }
}