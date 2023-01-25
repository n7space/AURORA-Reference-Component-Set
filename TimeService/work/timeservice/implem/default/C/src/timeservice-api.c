#include "timeservice-api.h"

#define CFS_2E32 4294967296ull
#define BYTE_CAPACITY 256
#define CUC_TFIELD_SIZE 7

asn1SccCfsTimestamp nanoseconds_to_cfs_timestamp(uint64_t nanoseconds)
{
    // scale fractional range to (0..CFS_2E32)
    uint64_t fraction = nanoseconds % NS_PER_SECOND;
    fraction *= CFS_2E32;
    fraction /= NS_PER_SECOND;

    asn1SccCfsTimestamp timestamp;
    timestamp.seconds = nanoseconds / NS_PER_SECOND;
    timestamp.subseconds = (uint32_t) fraction;
    return timestamp;
}

asn1SccCucTimestamp nanoseconds_to_cuc_timestamp(uint64_t nanoseconds)
{
    asn1SccCucTimestamp timestamp;
    long double seconds = (long double)nanoseconds / NS_PER_SECOND;

    // CUC timestamp contains three bytes for fractions of a second
    uint64_t seconds_shifted = seconds * BYTE_CAPACITY * BYTE_CAPACITY * BYTE_CAPACITY; 
    seconds_shifted = adjust_endianess(seconds_shifted);
    unsigned char *time = (unsigned char *)&seconds_shifted;

    for (size_t i = 0; i < CUC_TFIELD_SIZE; i++)
    {
        timestamp.arr[i] = time[CUC_TFIELD_SIZE - 1 - i];
    }
    return timestamp;
}

void TimeService_Startup(TimeService *const self)
{
    self->starting_time = get_time();
}

void TimeService_CfsTimestamp(TimeService *const self, asn1SccULongInteger nanoseconds, asn1SccCfsTimestamp *timestamp)
{
    // Unused in this implementation
    (void)self;
    
    *timestamp = nanoseconds_to_cfs_timestamp(nanoseconds);
}

asn1SccComparisonResult TimeService_CfsTimestampCmp(TimeService *const self, const asn1SccCfsTimestamp *timestamp1, const asn1SccCfsTimestamp *timestamp2)
{
    // Unused in this implementation
    (void)self;

    if (timestamp1->seconds > timestamp2->seconds)
    {
        return 1;
    }
    else if (timestamp1->seconds < timestamp2->seconds)
    {
        return -1;
    }
    else
    {
        if (timestamp1->subseconds > timestamp2->subseconds)
        {
            return 1;
        }
        else if (timestamp1->subseconds < timestamp2->subseconds)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}


asn1SccClockStatusEnum TimeService_ClockStatus(TimeService *const self)
{
    // Unused in this implementation
    (void)self;

    if (check_clock_available()) {
        return ClockStatusEnum_available;
    } else {
        return ClockStatusEnum_unavailable;
    }
}

void TimeService_CucTimestamp(TimeService *const self, asn1SccULongInteger nanoseconds, asn1SccCucTimestamp *timestamp)
{
    // Unused in this implementation
    (void)self;

    *timestamp = nanoseconds_to_cuc_timestamp(nanoseconds);
}


asn1SccComparisonResult TimeService_CucTimestampCmp(TimeService *const self, const asn1SccCucTimestamp *timestamp1, const asn1SccCucTimestamp *timestamp2)
{
    // Unused in this implementation
    (void)self;

    return cmp_memory(timestamp1->arr, timestamp2->arr, CUC_TFIELD_SIZE);
}


uint64_t TimeService_ObetTime(TimeService *const self)
{
    uint64_t measured_time = get_time();
    return measured_time - self->starting_time;
}
