#ifndef TRAPFILEDATA_H
#define TRAPFILEDATA_H

#include <stdbool.h>

// Определение структуры TrapFileData
typedef struct {
    char* filePath;
    char* creationTime;
    char* lastAccessTime;
} TrapFileData;

#endif // TRAPFILEDATA_H 