#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/FileTrapModule.h"
#include "../include/ActivityData.h"
#include "../include/TrapFileData.h"

void createSingleTrapFile(const char* filePath, const char* content) {
    FILE* fp = fopen(filePath, "w");
    if (fp) {
        fputs(content, fp);
        fclose(fp);
    }
}

void saveDataToDB_Activity(ActivityData data) {
    // Заглушка: ничего не делает
}

void saveDataToDB_FileTrap(TrapFileData data) {
    // Заглушка: ничего не делает
} 