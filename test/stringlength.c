#include <string.h>
#include "include.h"

int main(void)
{
    int ret = 1;

    ASSERT(3 == strlen("foo"));
    ASSERT(0 == strlen(""));

    ret = 0;
cleanup:
    return ret;
}
