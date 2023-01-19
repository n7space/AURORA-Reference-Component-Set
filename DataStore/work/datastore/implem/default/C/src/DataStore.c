#include "DataStore.h"
#include "Logs.h"

#define true 1
#define false 0

#define INVALID_KEY 0
#define INVALID_INDEX -1

// typedef struct
// {
//     uint64_t capacity;
//     int delete_oldest_on_overflow;
// } DataStoreConfig;

void DataStore_Configure(DataStore *self, uint64_t capacity, int delete_oldest_on_overflow)
{
    self->max_data_length = (capacity > 0) ? capacity : DEFAULT_MAX_DATA_LENGTH;
    self->delete_oldest_on_overflow = delete_oldest_on_overflow;
}

void DataStore_Init(DataStore *self, uint64_t capacity, int delete_oldest_on_overflow)
{
    self->data_length = 0;
    self->last_key = 0;
    DataStore_Configure(self, capacity, delete_oldest_on_overflow);
}

asn1SccKeyType DataStore_get_new_key(DataStore *self)
{
    self->last_key++;
    return self->last_key;
}

void DataStore_shift_elements_left(DataStore *self, int start_idx)
{
    for (int i = start_idx; i < self->data_length - 1; i++)
    {
        self->data[i] = self->data[i+1];
    }
}

int DataStore_find_index_by_key(DataStore *self, asn1SccKeyType key)
{
    for (int i = 0; i < self->data_length; i++)
    {
        if (self->data[i].key == key) {
            return i;
        }
    }
    return INVALID_INDEX;
}

asn1SccKeyType DataStore_Create(DataStore *self, asn1SccDataType data)
{
    DataItem item;
    if (self->data_length < self->max_data_length) {
        item.key = DataStore_get_new_key(self);
        item.data = data;
        self->data[self->data_length] = item;
        self->data_length++;
        LOG("Item created\n");
    } else {
        if (self->delete_oldest_on_overflow) {
            item.key = self->data[0].key;
            item.data = data;
            self->data[0] = item;
            LOG("Item created\n");
        } else {
            LOG("Rejected! The storage is full\n");
            item.key = INVALID_KEY;
        }
    }
    return item.key;
}

DataType* DataStore_Read(DataStore *self, asn1SccKeyType key)
{
    int index = DataStore_find_index_by_key(self, key);
    if (index == INVALID_INDEX) {
        return NULL;
    }

    LOG("Item found\n");
    return &(self->data[index].data);
}

void DataStore_Update(DataStore *self, asn1SccKeyType key, asn1SccDataType new_data)
{
    DataType *found_data = DataStore_Read(self, key);
    if (found_data == NULL) {
       LOG("Item to update not found\n");
       return; 
    }

    *found_data = new_data;
    LOG("Item updated\n");
}

void DataStore_Delete(DataStore *self, asn1SccKeyType key)
{
    int index = DataStore_find_index_by_key(self, key);
    if (index != INVALID_INDEX) {
        DataStore_shift_elements_left(self, index);
        self->data_length--;
        LOG("Item deleted\n");
    } else {
        LOG("Item to delete not found\n");
    }
}

// int main()
// {
//     DataStore datastore;
//     DataStore_Init(&datastore, 3, false);

//     DataItem dataitem;
//     dataitem.data = 5;
//     DataStore_Create(&datastore, dataitem);
//     DataStore_Create(&datastore, dataitem);
//     uint64_t res = DataStore_Create(&datastore, dataitem);
//     LOG_VARIABLE_ENDL("Key: ", res);

//     DataItem *item = DataStore_Read(&datastore, res);
//     if (item != NULL) {
//         LOG_VARIABLE_ENDL("Read item: ", item->data);
//     } else {
//         LOG("Read item is NULL\n");
//     }

//     DataStore_Configure(&datastore, 3, true);
//     res = DataStore_Create(&datastore, dataitem);

//     LOG_VARIABLE_ENDL("Key: ", res);
//     LOG_VARIABLE_ENDL("datastore.data_length: ", datastore.data_length);

//     DataStore_Update(&datastore, 3, 42);
//     item = DataStore_Read(&datastore, 3);
//     if (item != NULL) {
//         LOG_VARIABLE_ENDL("Read item: ", item->data);
//     } else {
//         LOG("Read item is NULL\n");
//     }

//     DataStore_Delete(&datastore, 3);
//     item = DataStore_Read(&datastore, 3);
//     if (item != NULL) {
//         LOG_VARIABLE_ENDL("Read item: ", item->data);
//     } else {
//         LOG("Read item is NULL\n");
//     }

//     return 0;
// }
