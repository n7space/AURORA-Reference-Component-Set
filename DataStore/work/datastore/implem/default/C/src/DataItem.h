#ifndef DATAITEM_H
#define DATAITEM_H

#include "dataview-uniq.h"

#include <stdint.h>

typedef int DataType;

typedef struct
{
    asn1SccKeyType key;
    asn1SccDataType data;
} DataItem;


#endif