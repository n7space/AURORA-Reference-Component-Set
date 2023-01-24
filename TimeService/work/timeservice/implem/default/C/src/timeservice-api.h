#ifndef TIMESERVICE_API_H
#define TIMESERVICE_API_H

#include "timeservice-utils.h"

#if defined(RTEMS6_TARGET)
#include "timeservice-rtems.h"
#elif defined(GENERIC_LINUX_TARGET)
#include "timeservice-linux.h"
#else
#include "timeservice-dummy.h"
#endif

#include "dataview-uniq.h"

/// @brief Structure representing TimeService
typedef struct
{
    asn1SccULongInteger starting_time;
} TimeService;

void TimeService_Startup(TimeService *const self);


void TimeService_CfsTimestamp(TimeService *const self, const asn1SccULongInteger nanoseconds, asn1SccCfsTimestamp *timestamp);


asn1SccComparisonResult TimeService_CfsTimestampCmp(TimeService *const self, const asn1SccCfsTimestamp *timestamp1, const asn1SccCfsTimestamp *timestamp2);


asn1SccClockStatusEnum TimeService_ClockStatus(TimeService *const self);


void TimeService_CucTimestamp(TimeService *const self, const asn1SccULongInteger nanoseconds, asn1SccCucTimestamp *timestamp);


asn1SccComparisonResult TimeService_CucTimestampCmp(TimeService *const self, const asn1SccCucTimestamp *timestamp1, const asn1SccCucTimestamp *timestamp2);


asn1SccULongInteger TimeService_ObetTime(TimeService *const self);

#endif
