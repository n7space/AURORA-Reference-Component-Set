#ifndef DATASTORE_H
#define DATASTORE_H

#include "DataItem.h"

#define DEFAULT_MAX_DATA_LENGTH 1024

typedef struct
{
    int delete_oldest_on_overflow;
    uint64_t max_data_length;
    uint64_t data_length;
    asn1SccKeyType last_key;
    DataItem data[DEFAULT_MAX_DATA_LENGTH];
} DataStore;

asn1SccKeyType DataStore_Create(DataStore *self, asn1SccDataType data);
DataType* DataStore_Read(DataStore *self, asn1SccKeyType key);
void DataStore_Update(DataStore *self, asn1SccKeyType key, asn1SccDataType new_data);
void DataStore_Delete(DataStore *self, asn1SccKeyType key);

#endif
