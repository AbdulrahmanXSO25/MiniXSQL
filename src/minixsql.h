#ifndef MINIXSQL_H
#define MINIXSQL_H

#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Will be used when implementing disk IO */
#define BASE_DIR "../MXDATA"

#define COL_NAME_MAX_LEN UINT8_MAX
#define TBL_NAME_MAX_LEN (1 << 6)
#define VAR_CHAR_MAX_LEN UINT16_MAX
#define MAX_TABLES_CNT (1 << 5)

#define ERR_TABLE_NOT_FOUND 1
#define ERR_MAX_TABLES_REACHED 2
#define ERR_MEMORY_ALLOCATION_FAILED 3
#define ERR_TUPLE_NOT_FOUND 4

typedef uint32_t OID;

typedef uint8_t bool_t;
#define true 1
#define false 0

typedef enum {
    CHAR,
    BOOLEAN,
    UINT_64,
    UINT_32,
    UINT_16,
    UINT_8,
    INT_64,
    INT_32,
    INT_16,
    INT_8,
    FLOAT,
    DOUBLE,
    VARCHAR,
    TIMESTAMP
} COL_DATA_TYPE;

typedef struct Column {
    char col_name[COL_NAME_MAX_LEN];
    COL_DATA_TYPE col_type;
    uint16_t col_len;
} Column;

typedef struct Tuple {
    uint8_t* data;
    size_t length;
} Tuple;

typedef struct Table {
    OID id;
    char name[TBL_NAME_MAX_LEN];
    Column* columns;
    int column_count;
    Tuple* tuples;
    int tuple_count;
    int tuple_capacity;
} Table;

Table* find_table(const char* name);

int CreateTable(const char* name, Column* cols, int col_count);
int DeleteTable(const char* name);

int InsertTuple(const char* table_name, void** data);
int UpdateTuple(const char* table_name, int tuple_index, void** data);
int DeleteTuple(const char* table_name, int tuple_index);
Tuple* GetTuple(const char* table_name, int tuple_index);
void* ReadTupleValue(Table* table, Tuple* tuple, int column_index, size_t* value_size);

#endif // MINIXSQL_H