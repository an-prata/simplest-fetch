// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#define PROC_CPU_INFO "/proc/cpuinfo"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/statvfs.h>
#include "sysinf.h"

char* get_cpu_model() {
    FILE* cpuinfo_file;
    cpuinfo_file = fopen(PROC_CPU_INFO, "r");

    if (cpuinfo_file == NULL) {
        char* err = malloc(30);
        strncpy(err, "Could not read ", 30);
        strncat(err, PROC_CPU_INFO, 30);

        perror(err);
        exit(EXIT_FAILURE);
    }

	char* line = malloc(1024);

    while(fgets(line, 1024, cpuinfo_file) != NULL) {
        if (strstr(line, "model name") != NULL) {
            char* cpu_name = malloc(strlen(line) - 13);

            for (int character = 13; character < strlen(line); character++)
                cpu_name[character - 13] = line[character];

            return cpu_name;
        }
    }

    return "\0";
}

int get_root_size(unsigned long* size, unsigned long* usage) {
    struct statvfs filesystemStats;

    if (statvfs("/etc/fstab", &filesystemStats) != 0) {
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
    bool numberFinishedReading = false;
    bool foundNumber = false;

    for (int c = 0; !numberFinishedReading;) {
        char cc = fgetc(meminfo);

        if (strchr("1234567890", cc) != NULL) {
            foundNumber = true;
            memCapacityStr[c] = cc;
            c++; // hehe, c++
        } else if (foundNumber) {
            numberFinishedReading = true;
        }
    }

	// meminfo is in KB to multiply by 1000.
    return atol(memCapacityStr) * 1000;
}

