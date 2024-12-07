// utils.c
#include "utils.h"

void PrintTuple(const char* table_name, int tuple_index) {
    Table* table = find_table(table_name);
    Tuple* tuple = GetTuple(table_name, tuple_index);
    if (table == NULL || tuple == NULL) {
        printf("Tuple not found\n");
        return;
    }

    for (int i = 0; i < table->column_count; i++) {
        printf("%s: ", table->columns[i].col_name);
        size_t value_size;
        void* value = ReadTupleValue(table, tuple, i, &value_size);

        switch (table->columns[i].col_type) {
            case CHAR:
                printf("%c", *(char*)value);
                break;
            case BOOLEAN:
                printf("%s", *(bool_t*)value ? "[v]" : "[ ]");
                break;
            case UINT_8:
                printf("%u", *(uint8_t*)value);
                break;
            case INT_8:
                printf("%d", *(int8_t*)value);
                break;
            case UINT_16:
                printf("%u", *(uint16_t*)value);
                break;
            case INT_16:
                printf("%d", *(int16_t*)value);
                break;
            case UINT_32:
                printf("%u", *(uint32_t*)value);
                break;
            case INT_32:
                printf("%d", *(int32_t*)value);
                break;
            case UINT_64:
                printf("%lu", *(uint64_t*)value);
                break;
            case INT_64:
                printf("%ld", *(int64_t*)value);
                break;
            case FLOAT:
                printf("%f", *(float*)value);
                break;
            case DOUBLE:
                printf("%lf", *(double*)value);
                break;
            case VARCHAR:
                printf("%s", (char*)value);
                break;
            case TIMESTAMP:
                {
                    time_t timestamp = *(time_t*)value;
                    char time_str[26];
                    ctime_r(&timestamp, time_str);
                    time_str[24] = '\0'; // remove newline
                    printf("%s", time_str);
                }
                break;
        }
        printf("\n");
    }
    printf("Tuple length: %zu bytes\n", tuple->length);
}

void PrintTable(const char* table_name) {
    Table* table = find_table(table_name);
    if (table == NULL) {
        printf("Table not found\n");
        return;
    }

    int* col_widths = malloc(table->column_count * sizeof(int));
    for (int i = 0; i < table->column_count; i++) {
        col_widths[i] = strlen(table->columns[i].col_name);
    }

    for (int i = 0; i < table->tuple_count; i++) {
        Tuple* tuple = &table->tuples[i];
        for (int j = 0; j < table->column_count; j++) {
            size_t value_size;
            void* value = ReadTupleValue(table, tuple, j, &value_size);
            char buffer[256];
            int len;

            switch (table->columns[j].col_type) {
                case CHAR:
                    len = snprintf(buffer, sizeof(buffer), "%c", *(char*)value);
                    break;
                case BOOLEAN:
                    len = snprintf(buffer, sizeof(buffer), "%s", *(bool_t*)value ? "[v]" : "[ ]");
                    break;
                case UINT_8:
                    len = snprintf(buffer, sizeof(buffer), "%u", *(uint8_t*)value);
                    break;
                case INT_8:
                    len = snprintf(buffer, sizeof(buffer), "%d", *(int8_t*)value);
                    break;
                case UINT_16:
                    len = snprintf(buffer, sizeof(buffer), "%u", *(uint16_t*)value);
                    break;
                case INT_16:
                    len = snprintf(buffer, sizeof(buffer), "%d", *(int16_t*)value);
                    break;
                case UINT_32:
                    len = snprintf(buffer, sizeof(buffer), "%u", *(uint32_t*)value);
                    break;
                case INT_32:
                    len = snprintf(buffer, sizeof(buffer), "%d", *(int32_t*)value);
                    break;
                case UINT_64:
                    len = snprintf(buffer, sizeof(buffer), "%lu", *(uint64_t*)value);
                    break;
                case INT_64:
                    len = snprintf(buffer, sizeof(buffer), "%ld", *(int64_t*)value);
                    break;
                case FLOAT:
                    len = snprintf(buffer, sizeof(buffer), "%.2f", *(float*)value);
                    break;
                case DOUBLE:
                    len = snprintf(buffer, sizeof(buffer), "%.2lf", *(double*)value);
                    break;
                case VARCHAR:
                    len = snprintf(buffer, sizeof(buffer), "%s", (char*)value);
                    break;
                case TIMESTAMP:
                    {
                        time_t timestamp = *(time_t*)value;
                        struct tm* tm_info = localtime(&timestamp);
                        len = strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
                    }
                    break;
            }

            if (len > col_widths[j]) {
                col_widths[j] = len;
            }
        }
    }

    /* Print header */
    for (int i = 0; i < table->column_count; i++) {
        printf("+-");
        for (int j = 0; j < col_widths[i]; j++) {
            printf("-");
        }
        printf("-");
    }
    printf("+\n");

    for (int i = 0; i < table->column_count; i++) {
        printf("| %-*s ", col_widths[i], table->columns[i].col_name);
    }
    printf("|\n");

    for (int i = 0; i < table->column_count; i++) {
        printf("+-");
        for (int j = 0; j < col_widths[i]; j++) {
            printf("-");
        }
        printf("-");
    }
    printf("+\n");

    /* Print data */
    for (int i = 0; i < table->tuple_count; i++) {
        Tuple* tuple = &table->tuples[i];
        for (int j = 0; j < table->column_count; j++) {
            size_t value_size;
            void* value = ReadTupleValue(table, tuple, j, &value_size);
            char buffer[256];

            switch (table->columns[j].col_type) {
                case CHAR:
                    snprintf(buffer, sizeof(buffer), "%c", *(char*)value);
                    break;
                case BOOLEAN:
                    snprintf(buffer, sizeof(buffer), "%s", *(bool_t*)value ? "true" : "false");
                    break;
                case UINT_8:
                    snprintf(buffer, sizeof(buffer), "%u", *(uint8_t*)value);
                    break;
                case INT_8:
                    snprintf(buffer, sizeof(buffer), "%d", *(int8_t*)value);
                    break;
                case UINT_16:
                    snprintf(buffer, sizeof(buffer), "%u", *(uint16_t*)value);
                    break;
                case INT_16:
                    snprintf(buffer, sizeof(buffer), "%d", *(int16_t*)value);
                    break;
                case UINT_32:
                    snprintf(buffer, sizeof(buffer), "%u", *(uint32_t*)value);
                    break;
                case INT_32:
                    snprintf(buffer, sizeof(buffer), "%d", *(int32_t*)value);
                    break;
                case UINT_64:
                    snprintf(buffer, sizeof(buffer), "%lu", *(uint64_t*)value);
                    break;
                case INT_64:
                    snprintf(buffer, sizeof(buffer), "%ld", *(int64_t*)value);
                    break;
                case FLOAT:
                    snprintf(buffer, sizeof(buffer), "%.2f", *(float*)value);
                    break;
                case DOUBLE:
                    snprintf(buffer, sizeof(buffer), "%.2lf", *(double*)value);
                    break;
                case VARCHAR:
                    snprintf(buffer, sizeof(buffer), "%s", (char*)value);
                    break;
                case TIMESTAMP:
                    {
                        time_t timestamp = *(time_t*)value;
                        struct tm* tm_info = localtime(&timestamp);
                        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
                    }
                    break;
            }

            printf("| %-*s ", col_widths[j], buffer);
        }
        printf("|\n");
    }

    /* Print footer */
    for (int i = 0; i < table->column_count; i++) {
        printf("+-");
        for (int j = 0; j < col_widths[i]; j++) {
            printf("-");
        }
        printf("-");
    }
    printf("+\n");

    free(col_widths);
}