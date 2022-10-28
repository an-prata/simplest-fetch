// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#define PROC_CPU_INFO "/proc/cpuinfo"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

char* get_cpu_model(char* line) {
	FILE* cpuinfo_file;
	cpuinfo_file = fopen(PROC_CPU_INFO, "r");

	if (cpuinfo_file == NULL) {
		char* err = malloc(30);
		strncpy(err, "Could not read ", 30);
		strncat(err, PROC_CPU_INFO, 30);

		perror(err);
		exit(EXIT_FAILURE);
	}	

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

int main() {
	struct utsname utsname;
	struct utsname* utsname_ptr = &utsname;
	
	uname(utsname_ptr);

	char cpu_model[1024];
	char* cpu_model_succes = get_cpu_model(cpu_model);

	if (cpu_model_succes[0] == '\0')
		perror("Failed to retrieve cpu model.");

	printf("\n");
	printf("  Kernel:\t%s\n", utsname.release);
	printf("  Hostname:\t%s\n", utsname.nodename);
	printf("  CPU Model:\t%s\n", cpu_model_succes);
	return 0;
}

