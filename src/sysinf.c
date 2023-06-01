// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/statvfs.h>
#include "sysinf.h"

char* get_cpu_model() {
    FILE* cpuinfo_file;
    cpuinfo_file = fopen("/proc/cpuinfo", "r");

    if (!cpuinfo_file) {
        char* err = malloc(30);
        strncpy(err, "Could not read ", 30);
        strncat(err, "/proc/cpuinfo", 30);

        perror(err);
        exit(EXIT_FAILURE);
    }

	char* line = malloc(1024);

    while(fgets(line, 1024, cpuinfo_file)) {
        if (strstr(line, "model name")) {
            char* cpu_name = malloc(strlen(line) - 13);

            for (int character = 13; character < strlen(line); character++)
                cpu_name[character - 13] = line[character];

            return cpu_name;
        }
    }

    return NULL;
}

int get_root_size(unsigned long* size, unsigned long* usage) {
    struct statvfs filesystemStats;

    if (statvfs("/etc/fstab", &filesystemStats)) {
        perror("Failed to retrieve file system information");
        return -1;
    }

    *size = filesystemStats.f_frsize * filesystemStats.f_blocks;
    *usage = filesystemStats.f_frsize * (filesystemStats.f_blocks - filesystemStats.f_bfree);
    return 0;
}

long get_memory_capacity() {
    char memCapacityStr[64];
    FILE* meminfo = fopen("/proc/meminfo", "r");
    bool foundNumber = false;
    int c = 0;

    while (true) {
        char cc = fgetc(meminfo);

        if (strchr("1234567890", cc) != NULL) {
            foundNumber = true;
            memCapacityStr[c] = cc;
            c++; // hehe, c++
        } else if (foundNumber) {
            break;
        }
    }

	// meminfo is in KB to multiply by 1000.
    return ferror(meminfo) ? -1 : atol(memCapacityStr) * 1000;
}

