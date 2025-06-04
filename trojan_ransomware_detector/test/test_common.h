#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "../include/FileTrapModule.h"
#include "../include/BehaviorAnalysisModule.h"
#include "../include/ResponseModule.h"
#include "../include/ReportingModule.h"
#include "../include/Database.h"
#include "../include/ActivityData.h"
#include "../include/FileSystemMonitorModule.h"
#include "../include/DataUtils.h"

// Общие определения для тестов
#define NUM_DIRECTORIES 3
extern const char* trap_directories[];

// Вспомогательные функции
void createTestDirectory(const char* path);
void removeTestDirectory(const char* path);
void createTestFile(const char* path, const char* content);
void updateTrapFiles(void);
void createSingleTrapFile(const char* filePath, const char* content);
void saveDataToDB_Activity(ActivityData data);

#endif // TEST_COMMON_H 