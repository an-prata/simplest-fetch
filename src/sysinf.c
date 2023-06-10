// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/statvfs.h>
#include "sysinf.h"

sysinf_err_t get_cpu_model(char* cpu_model, unsigned long len) {
    FILE* cpuinfo_file;
    cpuinfo_file = fopen("/proc/cpuinfo", "r");

    if (!cpuinfo_file) {
        return SI_ERR_COULD_NOT_OPEN;
    }

	char line[1024];

    while (fgets(line, 1024, cpuinfo_file)) {
        if (strstr(line, "model name")) {
            // Go to the thrid character. The first is the colon, second is a
            // space, and the third is the start of the CPU model.
            char* segment = &(strchr(line, ':')[2]);
            memcpy(cpu_model, segment, len);
            fclose(cpuinfo_file);
            return SI_ERR_NONE;
        }
    }

    fclose(cpuinfo_file);
    return SI_ERR_COULD_NOT_FIND;
}

sysinf_err_t get_root_size(unsigned long* size, unsigned long* usage) {
    struct statvfs filesystem_stats;

    if (statvfs("/etc/fstab", &filesystem_stats)) {
        return SI_ERR_COULD_NOT_STAT;
    }

    *size = filesystem_stats.f_frsize * filesystem_stats.f_blocks;
    *usage = filesystem_stats.f_frsize * (filesystem_stats.f_blocks - filesystem_stats.f_bfree);
    return SI_ERR_NONE;
}

sysinf_err_t get_memory_capacity(unsigned int* mem_cap) {
    char mem_capacity_str[64];
    FILE* meminfo = fopen("/proc/meminfo", "r");

    if (!meminfo) {
        return SI_ERR_COULD_NOT_OPEN;
    }
    
    int c = 0;

    while (true) {
        char cc = fgetc(meminfo);

        if (cc == EOF) {
            fclose(meminfo);
            return SI_ERR_COULD_NOT_FIND;
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

    if (ferror(meminfo)) {
        fclose(meminfo);
        return SI_ERR_COULD_NOT_READ;
    }

    *mem_cap = (unsigned int)atol(mem_capacity_str) * 1000;
    return SI_ERR_NONE;
}

