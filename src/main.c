// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#define NUMBER_OF_LINES 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include "sysinf.h"

static int base_1024 = 0; 
static double base = 1000;

/// Gets the size of the current terminal window.
///
/// @param width
/// A pointer to an int that will store the window's width.
///
/// @param height 
/// A pointer to an int that will store the window's height.
int get_window_size(unsigned short* width, unsigned short* height) {
	struct winsize window_size;
	ioctl(0, TIOCGWINSZ, &window_size);

	*width = window_size.ws_col;
	*height = window_size.ws_row;

	return 0;
}

char* unit_from_power(int power) {
	char* unit = malloc(base_1024 ? 4 : 3);

	switch (power) {
		case 0:
			unit = "B";
			break;
		case 1:
			unit = base_1024 ? "KiB" : "KB";
			break;
		case 2:
			unit = base_1024 ? "MiB" : "MB";
			break;
		case 3:
			unit = base_1024 ? "GiB" : "GB";
			break;
		case 4:
			unit = base_1024 ? "TiB" : "TB";
			break;
		case 5:
			unit = base_1024 ? "PiB" : "PB";
			break;
	}

	return unit;
}

int main(int argc, char** argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "help")) {
			printf("use -i for base 1024, ommit for base 1000\n");
			exit(0);
		}
		
		base_1024 = !strcmp(argv[1], "-i");
		base = base_1024 ? 1024.0 : 1000.0;
	}
	
	struct utsname utsname;
	uname(&utsname);

	char* cpu_model = get_cpu_model();

	if (!cpu_model)
		perror("Failed to retrieve cpu model.");

	unsigned short window_width, window_height;
	get_window_size(&window_width, &window_height);

	unsigned int left_margin_length = ((window_width - strlen(cpu_model)) / 2) - 12;
	unsigned int top_margin_length = (window_height - NUMBER_OF_LINES) / 2;
	unsigned int color_spacing_length = ((window_width - (left_margin_length * 2)) / 8);
	char* left_margin = malloc(left_margin_length + 1);
	char* top_margin = malloc(top_margin_length + 1);
	char* color_spacing = malloc(color_spacing_length + 1);
	
	memset(left_margin, ' ', left_margin_length);
	left_margin[left_margin_length + 1] = '\0';
	
	memset(top_margin, '\n', top_margin_length);
	top_margin[top_margin_length + 1] = '\0';

	memset(color_spacing, ' ', color_spacing_length);
	color_spacing[color_spacing_length + 1] = '\0';
	
	// Cut down utsname.release to be just the kernel version.
	char* kernel_version = strtok(utsname.release, "-");

	int power;
	unsigned long root_size, root_used;
	get_root_size(&root_size, &root_used);

	double root_size_dec = (double)root_size;
	double root_used_dec = (double)root_used;

	for (power = 0; root_size_dec > base; power++) {
		root_size_dec /= base;
		root_used_dec /= base;
	}

	char* unit = unit_from_power(power);

	double mem_capacity = (double)get_memory_capacity();

	for (power = 0; mem_capacity > base; power++)
		mem_capacity /= base;

	char* mem_unit = unit_from_power(power);

	printf("%s", top_margin);
	printf("%s  Kernel:\t\t%s\n", left_margin, kernel_version);
	printf("%s  Hostname:\t\t%s\n", left_margin, utsname.nodename);
	printf("%s  Processor Model:\t%s", left_margin, cpu_model);
	printf("%s  Memory Capacity:\t%3.1f %s\n", left_margin, mem_capacity, mem_unit);
	printf("%s  Drive Capacity:\t%3.1f %s used of %3.1f %s\n", left_margin, root_used_dec, unit, root_size_dec, unit);
	printf("\n");
	printf("%s\033[40m%s\033[0m", left_margin, color_spacing);
	printf("\033[41m%s\033[0m", color_spacing);
	printf("\033[42m%s\033[0m", color_spacing);
	printf("\033[43m%s\033[0m", color_spacing);
	printf("\033[44m%s\033[0m", color_spacing);
	printf("\033[45m%s\033[0m", color_spacing);
	printf("\033[46m%s\033[0m", color_spacing);
	printf("\033[47m%s\033[0m", color_spacing);
	printf("\033[?25l"); // Hide cursor
	printf("%s\n", top_margin);
	// Keep the terminal prompt from showing until enter key is pressed
	getchar();
	printf("\033[?25h"); // show cursor

	return 0;
}
