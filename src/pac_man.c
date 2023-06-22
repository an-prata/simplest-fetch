// Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pac_man.h"

static const char* pac_mans[] = PAC_MANS;

static char* get_sub_str(char* str, char delim, size_t len) {
	for (unsigned int i = 0; i < len; i++) {
		if (!str[i]) {
			if (i + 1 < len) {
				str = &str[i + 1];
				len -= i;
				i = 0;
			}

			continue;
		}

		if (str[i] == delim) {
			str[i] = '\0';
			puts(str);
			return str;
		}
	}

			puts(str);
	return NULL;
}

const char* get_pac_man() {
	const char* pac_man, * current_dir;
	char* path = getenv("PATH");
	size_t path_len = strlen(path);

	while ((current_dir = get_sub_str(path, ':', path_len))) {
		DIR* dir = opendir(current_dir);
		
		if (dir) {
			struct dirent* dirent;
			unsigned int prev_pac = 0 - 1;

			while ((dirent = readdir(dir))) {
				for (unsigned int i = 0; i < sizeof(pac_mans) / sizeof(*pac_mans); i++) {
					if (!strcmp(dirent->d_name, pac_mans[i]) && i < prev_pac) {
						pac_man = pac_mans[i];
						prev_pac = i;
					}
				}
			}
		}
	}

	return pac_man;
}
