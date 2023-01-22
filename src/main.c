// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#define NUMBER_OF_LINES 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include "sysinf.h"

/// Gets the size of the current terminal window.
///
/// @param width
/// A pointer to an int that will store the window's width.
///
/// @param height 
/// A pointer to an int that will store the window's height.
int get_window_size(int* width, int* height) {
	struct winsize windowSize;
	ioctl(0, TIOCGWINSZ, &windowSize);

	*width = windowSize.ws_col;
	*height = windowSize.ws_row;

	return 0;
}

char* unit_from_power(int power) {
	char* unit = malloc(3);

	switch (power) {
		case 0:
			unit = "B";
			break;
		case 1:
			unit = "KiB";
			break;
		case 2:
			unit = "MiB";
			break;
		case 3:
			unit = "GiB";
			break;
		case 4:
			unit = "TiB";
			break;
		case 5:
			unit = "PiB";
			break;
	}

	return unit;
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
	get_window_size(&windowWidth, &windowHeight);

	int leftMarginLength = ((windowWidth - strlen(cpu_model)) / 2) - 12;
	int topMarginLength = (windowHeight - NUMBER_OF_LINES) / 2;
	int colorSpacingLength = ((windowWidth - (leftMarginLength * 2)) / 8);
	char* leftMargin = malloc(leftMarginLength + 1);
	char* topMargin = malloc(topMarginLength + 1);
	char* colorSpacing = malloc(colorSpacingLength + 1);
	
	memset(leftMargin, ' ', leftMarginLength);
	leftMargin[leftMarginLength + 1] = '\0';
	
	memset(topMargin, '\n', topMarginLength);
	topMargin[topMarginLength + 1] = '\0';

	memset(colorSpacing, ' ', colorSpacingLength);
	colorSpacing[colorSpacingLength + 1] = '\0';
	
	// Cut down utsname.release to be just the kernel version.
	char* kernelVersion = strtok(utsname.release, "-");

	int power;
	unsigned long rootSize;
	unsigned long rootUsed;
	get_root_size(&rootSize, &rootUsed);

	double rootSizeDec = (double)rootSize;
	double rootUsedDec = (double)rootUsed;

	for (power = 0; rootSizeDec > 1024.0; power++) {
		rootSizeDec /= 1024.0;
		rootUsedDec /= 1024.0;
	}

	char* unit = unit_from_power(power);

	double memCapacity = (double)get_memory_capacity();

	for (power = 0; memCapacity > 1024.0; power++)
		memCapacity /= 1024.0;

	char* memUnit = unit_from_power(power);

	printf("%s", topMargin);
	printf("%s  Kernel:\t\t%s\n", leftMargin, kernelVersion);
	printf("%s  Hostname:\t\t%s\n", leftMargin, utsname.nodename);
	printf("%s  Processor Model:\t%s", leftMargin, cpu_model);
	printf("%s  Memory Capacity:\t%3.1f %s\n", leftMargin, memCapacity, memUnit);
	printf("%s  Drive Capacity:\t%3.1f %s used of %3.1f %s\n", leftMargin, rootUsedDec, unit, rootSizeDec, unit);
	printf("\n");
	printf("%s\033[40m%s\033[0m", leftMargin, colorSpacing);
	printf("\033[41m%s\033[0m", colorSpacing);
	printf("\033[42m%s\033[0m", colorSpacing);
	printf("\033[43m%s\033[0m", colorSpacing);
	printf("\033[44m%s\033[0m", colorSpacing);
	printf("\033[45m%s\033[0m", colorSpacing);
	printf("\033[46m%s\033[0m", colorSpacing);
	printf("\033[47m%s\033[0m", colorSpacing);
	printf("\033[?25l"); // Hide cursor
	printf("%s\n", topMargin);
	// Keep the terminal prompt from showing until enter key is pressed
	getchar();
	printf("\033[?25h"); // show cursor

	return 0;
}
