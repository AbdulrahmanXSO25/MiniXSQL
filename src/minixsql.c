// minixsql.c
#include "minixsql.h"

static Table* tables = NULL;
static int table_count = 0;
static int table_capacity = 0;

/* Helper */
int resize_table_array() {
    int new_capacity = table_capacity == 0 ? 1 : table_capacity * 2;
    Table* new_tables = realloc(tables, new_capacity * sizeof(Table));
    if (new_tables == NULL) {
        return -ERR_MEMORY_ALLOCATION_FAILED;
    }
    tables = new_tables;
    table_capacity = new_capacity;
    return 0;
}

/* Helper */
int resize_tuple_array(Table* table) {
    int new_capacity = table->tuple_capacity == 0 ? 1 : table->tuple_capacity * 2;
    Tuple* new_tuples = realloc(table->tuples, new_capacity * sizeof(Tuple));
    if (new_tuples == NULL) {
        return -ERR_MEMORY_ALLOCATION_FAILED;
    }
    table->tuples = new_tuples;
    table->tuple_capacity = new_capacity;
    return 0;
}

/* Helper */
Table* find_table(const char* name) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(name, tables[i].name) == 0) {
            return &tables[i];
        }
    }
    return NULL;
}

/* Helper */
size_t get_tuple_size(Table* table) {
    size_t size = 0;
    for (int i = 0; i < table->column_count; i++) {
        switch (table->columns[i].col_type) {
            case CHAR:
            case BOOLEAN:
            case UINT_8:
            case INT_8:
                size += 1;
                break;
            case UINT_16:
            case INT_16:
                size += 2;
                break;
            case UINT_32:
            case INT_32:
            case FLOAT:
                size += 4;
                break;
            case UINT_64:
            case INT_64:
            case DOUBLE:
            case TIMESTAMP:
                size += 8;
                break;
            case VARCHAR:
                size += table->columns[i].col_len;
                break;
        }
    }
    return size;
}

int CreateTable(const char* name, Column* cols, int col_count) {
    if (table_count >= table_capacity && resize_table_array() != 0) {
        return -ERR_MEMORY_ALLOCATION_FAILED;
    }

    Table* new_table = &tables[table_count];
    new_table->id = table_count;
    strncpy(new_table->name, name, TBL_NAME_MAX_LEN);
    new_table->column_count = col_count;
    new_table->tuple_count = 0;
    new_table->tuple_capacity = 0;

    new_table->columns = malloc(col_count * sizeof(Column));
    if (new_table->columns == NULL) {
        return -ERR_MEMORY_ALLOCATION_FAILED;
    }
    memcpy(new_table->columns, cols, col_count * sizeof(Column));

    new_table->tuples = NULL;

    table_count++;
    return 0;
}

int DeleteTable(const char* name) {
    Table* table = find_table(name);
    if (table == NULL) {
        return -ERR_TABLE_NOT_FOUND;
    }

    for (int i = 0; i < table->tuple_count; i++) {
        free(table->tuples[i].data);
    }
    free(table->tuples);
    free(table->columns);

    memmove(table, table + 1, (table_count - (table - tables) - 1) * sizeof(Table));
    
    table_count--;
    return 0;
}

int InsertTuple(const char* table_name, void** data) {
    Table* table = find_table(table_name);
    if (table == NULL) {
        return -ERR_TABLE_NOT_FOUND;
    }
    
    if (table->tuple_count >= table->tuple_capacity && resize_tuple_array(table) != 0) {
        return -ERR_MEMORY_ALLOCATION_FAILED;
    }
    
    size_t tuple_size = get_tuple_size(table);
    uint8_t* tuple_data = malloc(tuple_size);
    if (tuple_data == NULL) {
        return -ERR_MEMORY_ALLOCATION_FAILED;
    }
    
    size_t offset = 0;
    for (int i = 0; i < table->column_count; i++) {
        switch (table->columns[i].col_type) {
            case CHAR:
            case BOOLEAN:
            case UINT_8:
            case INT_8:
                memcpy(tuple_data + offset, data[i], 1);
                offset += 1;
                break;
            case UINT_16:
            case INT_16:
                memcpy(tuple_data + offset, data[i], 2);
                offset += 2;
                break;
            case UINT_32:
            case INT_32:
            case FLOAT:
                memcpy(tuple_data + offset, data[i], 4);
                offset += 4;
                break;
            case UINT_64:
            case INT_64:
            case DOUBLE:
                memcpy(tuple_data + offset, data[i], 8);
                offset += 8;
                break;
            case VARCHAR:
                memcpy(tuple_data + offset, data[i], table->columns[i].col_len);
                offset += table->columns[i].col_len;
                break;
            case TIMESTAMP:
                memcpy(tuple_data + offset, data[i], sizeof(time_t));
                offset += sizeof(time_t);
                break;
        }
    }
    
    table->tuples[table->tuple_count].data = tuple_data;
    table->tuples[table->tuple_count].length = tuple_size;
    table->tuple_count++;
    
    return 0;
}

int UpdateTuple(const char* table_name, int tuple_index, void** data) {
    Table* table = find_table(table_name);
    if (table == NULL) {
        return -ERR_TABLE_NOT_FOUND;
    }
    
    if (tuple_index >= table->tuple_count) {
        return -ERR_TUPLE_NOT_FOUND;
    }
    
    size_t offset = 0;
    for (int i = 0; i < table->column_count; i++) {
        switch (table->columns[i].col_type) {
            case CHAR:
            case BOOLEAN:
            case UINT_8:
            case INT_8:
                memcpy(table->tuples[tuple_index].data + offset, data[i], 1);
                offset += 1;
                break;
            case UINT_16:
            case INT_16:
                memcpy(table->tuples[tuple_index].data + offset, data[i], 2);
                offset += 2;
                break;
            case UINT_32:
            case INT_32:
            case FLOAT:
                memcpy(table->tuples[tuple_index].data + offset, data[i], 4);
                offset += 4;
                break;
            case UINT_64:
            case INT_64:
            case DOUBLE:
            case TIMESTAMP:
                memcpy(table->tuples[tuple_index].data + offset, data[i], 8);
                offset += 8;
                break;
            case VARCHAR:
                memcpy(table->tuples[tuple_index].data + offset, data[i], table->columns[i].col_len);
                offset += table->columns[i].col_len;
                break;
        }
    }
    
    return 0;
}

int DeleteTuple(const char* table_name, int tuple_index) {
    Table* table = find_table(table_name);
    if (table == NULL) {
        return -ERR_TABLE_NOT_FOUND;
    }
    
    if (tuple_index >= table->tuple_count) {
        return -ERR_TUPLE_NOT_FOUND;
    }
    
    free(table->tuples[tuple_index].data);
    
    memmove(&table->tuples[tuple_index], &table->tuples[tuple_index + 1], 
            (table->tuple_count - tuple_index - 1) * sizeof(Tuple));
    
    table->tuple_count--;
    return 0;
}

Tuple* GetTuple(const char* table_name, int tuple_index) {
    Table* table = find_table(table_name);
    if (table == NULL || tuple_index >= table->tuple_count) {
        return NULL;
    }
    return &table->tuples[tuple_index];
}

void* ReadTupleValue(Table* table, Tuple* tuple, int column_index, size_t* value_size) {
    if (column_index >= table->column_count) {
        return NULL;
    }

    size_t offset = 0;
    for (int i = 0; i < column_index; i++) {
        switch (table->columns[i].col_type) {
            case CHAR:
            case BOOLEAN:
            case UINT_8:
            case INT_8:
                offset += 1;
                break;
            case UINT_16:
            case INT_16:
                offset += 2;
                break;
            case UINT_32:
            case INT_32:
            case FLOAT:
                offset += 4;
                break;
            case UINT_64:
            case INT_64:
            case DOUBLE:
            case TIMESTAMP:
                offset += 8;
                break;
            case VARCHAR:
                offset += table->columns[i].col_len;
                break;
        }
    }

    switch (table->columns[column_index].col_type) {
        case CHAR:
        case BOOLEAN:
        case UINT_8:
        case INT_8:
            *value_size = 1;
            break;
        case UINT_16:
        case INT_16:
            *value_size = 2;
            break;
        case UINT_32:
        case INT_32:
        case FLOAT:
            *value_size = 4;
            break;
        case UINT_64:
        case INT_64:
        case DOUBLE:
        case TIMESTAMP:
            *value_size = 8;
            break;
        case VARCHAR:
            *value_size = table->columns[column_index].col_len;
            break;
    }

    return tuple->data + offset;
}