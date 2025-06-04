#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/DataUtils.h"
#include "../include/FileTrapModule.h"

void freeTrapFileData(TrapFileData* data, int count) {
    if (data == NULL) return;
    
    for (int i = 0; i < count; i++) {
        free(data[i].filePath);
        free(data[i].creationTime);
        free(data[i].lastAccessTime);
    }
    free(data);
}

void freeActivityData(ActivityData* data, int count) {
    if (data == NULL) return;
    
    for (int i = 0; i < count; i++) {
        free(data[i].filePath);
        free(data[i].activityType);
        free(data[i].timestamp);
    }
    free(data);
} 