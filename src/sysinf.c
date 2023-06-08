// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/statvfs.h>
#include "sysinf.h"

int get_cpu_model(char* cpu_model, unsigned long len) {
    FILE* cpuinfo_file;
    cpuinfo_file = fopen("/proc/cpuinfo", "r");

    if (!cpuinfo_file) {
        char err[64];
        
        strncpy(err, "Could not read ", 30);
        strncat(err, "/proc/cpuinfo", 30);
        perror(err);
        
        exit(EXIT_FAILURE);
    }

	char line[1024];

    while (fgets(line, 1024, cpuinfo_file)) {
        if (strstr(line, "model name")) {
            // Go to the thrid character. The first is the colon, second is a
            // space, and the third is the start of the CPU model.
            char* segment = &(strchr(line, ':')[2]);
            memcpy(cpu_model, segment, len);
            return 0;
        }
    }

    return -1;
}

int get_root_size(unsigned long* size, unsigned long* usage) {
    struct statvfs filesystem_stats;

    if (statvfs("/etc/fstab", &filesystem_stats)) {
        perror("Failed to retrieve file system information");
        return -1;
    }

    *size = filesystem_stats.f_frsize * filesystem_stats.f_blocks;
    *usage = filesystem_stats.f_frsize * (filesystem_stats.f_blocks - filesystem_stats.f_bfree);
    return 0;
}

long get_memory_capacity() {
    char mem_capacity_str[64];
    FILE* meminfo = fopen("/proc/meminfo", "r");
    int c = 0;

    while (true) {
        char cc = fgetc(meminfo);

        if (cc == EOF) {
            return -1;
        }
        
        if (strchr("1234567890", cc)) {
            mem_capacity_str[c] = cc;
            break;
        }
    }

    while (true) {
        char cc = fgetc(meminfo);
        c++;
        
        if (!strchr("1234567890", cc)) {
            break;
        }

        mem_capacity_str[c] = cc;
    }

	// meminfo is in KB to multiply by 1000.
    return ferror(meminfo) ? -1 : atol(mem_capacity_str) * 1000;
}

