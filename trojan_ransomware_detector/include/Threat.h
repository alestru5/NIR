#ifndef THREAT_H
#define THREAT_H

#include <stdbool.h>

// Определение структуры Threat
typedef struct {
    char* description;
    char* severity; // Можно использовать enum для строгости
} Threat;

#endif // THREAT_H 