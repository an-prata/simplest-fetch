// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#include <unistd.h>
#define NUMBER_OF_LINES 7

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include "sysinf.h"
#include "text.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

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
	switch (power) {
		case 0:
			return "B";
		case 1:
			return base_1024 ? "KiB" : "KB";
		case 2:
			return base_1024 ? "MiB" : "MB";
		case 3:
			return base_1024 ? "GiB" : "GB";
		case 4:
			return base_1024 ? "TiB" : "TB";
		case 5:
			return base_1024 ? "PiB" : "PB";
	}

	return base_1024 ? "?iB" : "?B";
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

	char cpu_model[1024];

	if (get_cpu_model(cpu_model, sizeof(cpu_model))) {
		perror("Failed to retrieve cpu model.");
	}

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

	for (power = 0; mem_capacity > base; power++) {
		mem_capacity /= base;
	}

	char* mem_unit = unit_from_power(power);

	// From here on is rendering.
	printf(ALT_SCREEN_ENTER);
	printf(CURSOR_HIDE);
	disable_canonical_input();

	unsigned short window_width, window_height;
	unsigned int margin_side, margin_top, margin_color, color_block_width, gap;
	
	while (true) {
		// quit on keyboard input
		char in[1];
		if (read(STDIN_FILENO, in, 1)) {
			break;
		}

		{
			unsigned short t_window_width, t_window_height;
			unsigned int t_margin_side, t_margin_top, t_margin_color, t_color_block_width;
			long t_gap;
			
			get_window_size(&t_window_width, &t_window_height);
			t_margin_side = ((t_window_width - strlen(cpu_model) - strlen(LABEL_PROC)) / 2);
			t_gap = (t_window_height / 4) - NUMBER_OF_LINES - 2;
			t_margin_top = (t_window_height - NUMBER_OF_LINES) / 2;
			t_margin_color = calc_color_margin(t_window_width, &t_color_block_width);

			if (t_gap < 1) {
				t_gap = 1;
			}

			if (
				t_window_width 		== window_width 	 &&
				t_window_height 	== window_height 	 &&
				t_margin_side 		== margin_side 		 &&
				t_margin_top 		== margin_top 		 &&
				t_margin_color 		== margin_color 	 &&
				t_color_block_width == color_block_width &&
				t_gap 				== gap
			) {
				continue;
			} else {
				window_width 		= t_window_width; 
				window_height 		= t_window_height; 
				margin_side 		= t_margin_side; 
				margin_top 			= t_margin_top;
				margin_color 		= t_margin_color; 
				color_block_width 	= t_color_block_width; 
				gap 				= t_gap;
			}
		}

		printf(CLEAR);
		
		// Center contents
		PRINT_ITER("\n", margin_top);

		// Hardware and OS stats
		PRINT_ITER(" ", margin_side);
		printf("%s%s\n", LABEL_KERN, kernel_version);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%s\n", LABEL_HOST, utsname.nodename);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%s", LABEL_PROC, cpu_model);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%3.1f %s\n", LABEL_MEM, mem_capacity, mem_unit);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%3.1f %s used of %3.1f %s\n", LABEL_STOR, root_used_dec, unit, root_size_dec, unit);

		PRINT_ITER("\n", gap);

		// Print color blocks
		PRINT_ITER(" ", margin_color);

		printf("%s", COLOR_0);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_1);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_2);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_3);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_4);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_5);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_6);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_7);
		PRINT_ITER(" ", color_block_width);
	
		printf("%s", COLOR_RESET);
		printf("\n");

		// Print to the bottom of the screen
		PRINT_ITER("\n", margin_top);

		usleep(100 * 1000);
	}

	enable_canonical_input();
	printf(ALT_SCREEN_EXIT);
	printf(CURSOR_SHOW); // show cursor
	return 0;
}
