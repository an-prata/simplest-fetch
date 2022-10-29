// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#define PROC_CPU_INFO "/proc/cpuinfo"
#define NUMBER_OF_LINES 3

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>

/// Gets the CPU model from PROC_CPU_INFO.
///
/// @returns
/// The name of the system's CPU model. NULL on failiure.
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

/// Gets the size of the current terminal window.
///
/// @param width
/// A pointer to an int that will store the window's width.
///
/// @param height 
/// A pointer to an int that will store the window's height.
int getWindowSize(int* width, int* height) {
	struct winsize windowSize;
	ioctl(0, TIOCGWINSZ, &windowSize);

	*width = windowSize.ws_col;
	*height = windowSize.ws_row;

	return 0;
}

int main() {
	struct utsname utsname;
	struct utsname* utsname_ptr = &utsname;	
	uname(utsname_ptr);

	char* cpu_model = get_cpu_model();

	if (cpu_model[0] == '\0')
		perror("Failed to retrieve cpu model.");

	int windowWidth;
	int windowHeight;
	getWindowSize(&windowWidth, &windowHeight);

	int leftMarginLength = (windowWidth - strlen(cpu_model) - 15) / 2;
	int topMarginLength = (windowHeight - NUMBER_OF_LINES) / 2;
	char* leftMargin = malloc(leftMarginLength / 8);
	char* topMargin = malloc(topMarginLength);
	
	for (int i = 0; i < leftMarginLength / 8; i++)
		leftMargin[i] = '\t';

	for (int i = 0; i < topMarginLength; i++)
		topMargin[i] = '\n';

	printf("%s", topMargin);
	printf("%s  Kernel:\t%s\n", leftMargin, utsname.release);
	printf("%s  Hostname:\t%s\n", leftMargin, utsname.nodename);
	printf("%s  CPU Model:\t%s\n", leftMargin, cpu_model);
	printf("%s", topMargin);
	return 0;
}

