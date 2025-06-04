#include "test_common.h"

const char* trap_directories[] = {
    "trojan_ransomware_detector/trap_files",
    "trojan_ransomware_detector/trap_files/dir1",
    "trojan_ransomware_detector/trap_files/dir2"
};

void createTestDirectory(const char* path) {
    mkdir(path, 0755);
}

void removeTestDirectory(const char* path) {
    DIR* dir = opendir(path);
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char file_path[256];
                snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);
                remove(file_path);
            }
        }
        closedir(dir);
    }
    rmdir(path);
}

void createTestFile(const char* path, const char* content) {
    FILE* file = fopen(path, "w");
    if (file != NULL) {
        fprintf(file, "%s", content);
        fclose(file);
    }
} 