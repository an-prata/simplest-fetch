// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include "pac_man.h"
#include "sysinf.h"
#include "text.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static const char* pac_mans[] = PAC_MANS;

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

char* unit_from_power(int power, bool base_1024) {
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
	bool base_1024 = false;
	unsigned int base = 1000;
	
	if (argc > 1) {
		if (!strcmp(argv[1], "help")) {
			printf("use -i for base 1024, ommit for base 1000\n");
			exit(0);
		}
		
		base_1024 = !strcmp(argv[1], "-i");
		base = base_1024 ? 1024 : 1000;
	}
	
	// This is every variable outputted as part of the fetch.
	unsigned long stor_capacity, stor_used;
	double stor_capacity_dec, stor_used_dec, mem_capacity;
	char * stor_unit, * mem_unit, * kern_version, cpu_model[1024];
	const char* pac_man = NULL;
	
	if (get_cpu_model(cpu_model, sizeof cpu_model)) {
		perror("Failed to retrieve cpu model.");
	}

	// Cut down utsname.release to be just the kernel version.
	struct utsname utsname;
	uname(&utsname);
	kern_version = strtok(utsname.release, "-");

	get_root_size(&stor_capacity, &stor_used);

	stor_capacity_dec = (double)stor_capacity;
	stor_used_dec = (double)stor_used;
	mem_capacity = (double)get_memory_capacity();

	int e;

	for (e = 0; stor_capacity_dec > base; e++) {
		stor_capacity_dec /= base;
		stor_used_dec /= base;
	}

	stor_unit = unit_from_power(e, base_1024);

	for (e = 0; mem_capacity > base; e++) {
		mem_capacity /= base;
	}

	mem_unit = unit_from_power(e, base_1024);

	DIR* dir = opendir("/bin");

	if (dir) {
		struct dirent* dirent;
		unsigned int prev_pac = 0 - 1;

		while ((dirent = readdir(dir))) {
			for (unsigned int i = 0; i < sizeof(pac_mans) / sizeof(*pac_mans); i++) {
				if (!strcmp(dirent->d_name, pac_mans[i])) {
					if (i < prev_pac) {
						pac_man = pac_mans[i];
					}

					prev_pac = i;
				}
			}
		}
	}

	// From here on is rendering.
	printf(ALT_SCREEN_ENTER);
	printf(CURSOR_HIDE);
	disable_canonical_input();

	// Everyting used for rendering
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
			t_gap = (t_window_height / 4) - LABELS;
			t_margin_top = (t_window_height - (LABELS + t_gap + 1)) / 2;
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
		printf("%s%s\n", LABEL_KERN, kern_version);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%s\n", LABEL_PAC, pac_man);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%s\n", LABEL_HOST, utsname.nodename);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%s", LABEL_PROC, cpu_model);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%3.1f %s\n", LABEL_MEM, mem_capacity, mem_unit);
	
		PRINT_ITER(" ", margin_side);
		printf("%s%3.1f %s used of %3.1f %s\n", LABEL_STOR, stor_used_dec, stor_unit, stor_capacity_dec, stor_unit);

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
