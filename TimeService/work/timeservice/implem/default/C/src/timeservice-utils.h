#ifndef TIMESERVICE_UTILS
#define TIMESERVICE_UTILS

#include <time.h>
#include <stdint.h>

#define true 1
#define false 0

#define NS_PER_SECOND (1000000000)

#define CUC_TIME_ARRAY_SIZE 7

static int cmp_memory(const unsigned char *string_1, const unsigned char *string_2, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (string_1[i] != string_2[i]) {
            return (string_1[i] > string_2[i]) ? 1 : -1;
        }
    }
    return 0;
}

#endif
