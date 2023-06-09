// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#ifndef TEXT_H
#define TEXT_H

#define LABEL_KERN "  Kernel:             "
#define LABEL_HOST "  Hostname:           "
#define LABEL_PROC "  Processor Model:    "
#define LABEL_MEM  "  Memory Capacity:    "
#define LABEL_STOR "  Drive Capacity:     "

#define COLOR_0 "\033[40m"
#define COLOR_1 "\033[41m"
#define COLOR_2 "\033[42m"
#define COLOR_3 "\033[43m"
#define COLOR_4 "\033[44m"
#define COLOR_5 "\033[45m"
#define COLOR_6 "\033[46m"
#define COLOR_7 "\033[47m"
#define COLOR_RESET "\033[0m" 

#define CURSOR_HIDE "\033[?25l"
#define CURSOR_SHOW "\033[?25h"

#define ALT_SCREEN_ENTER "\033[?47h"
#define ALT_SCREEN_EXIT "\033[?47l"
#define CLEAR "\033[2J"

#define PRINT_ITER(s, n) \
	for (unsigned int i = 0; i < (n); i++) \
		printf("%s", s);

unsigned int calc_color_margin(unsigned short window_width, unsigned int* color_width);

int disable_canonical_input(void);

int enable_canonical_input(void);

#endif // TEXT_H
