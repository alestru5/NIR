#ifndef ACTIVITYDATA_H
#define ACTIVITYDATA_H

#include <stdbool.h>

// Определение структуры ActivityData
typedef struct {
    char* filePath;
    char* activityType;
    char* timestamp;
} ActivityData;

#endif // ACTIVITYDATA_H 