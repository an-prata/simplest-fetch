// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#include <unistd.h>
#include <termios.h>

static struct termios original_term_state;

unsigned int calc_color_margin(unsigned short window_width, unsigned int* color_width) {
	for (unsigned int margin = window_width / 12; margin < window_width; margin++) {
		if ((window_width - margin * 2) % 8) {
			*color_width = (window_width - margin) / 8;
			return margin;
		}
	}

	return 0;
}

int disable_canonical_input(void) {
	if (tcgetattr(STDIN_FILENO, &original_term_state))
        return -1;
    
    struct termios non_canonical = original_term_state;
    
    non_canonical.c_lflag &= ~(ICANON | ECHO);  /* Dont wait for newline and dont echo typed characters. */
    non_canonical.c_cc[VMIN] = 0;               /* Set minimum bytes from read to 1. */
    non_canonical.c_cc[VTIME] = 0;              /* Dont wait for input with read. */
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &non_canonical))
        return -1;
    
    return 0;
}

int enable_canonical_input(void) {
    if (tcsetattr(STDIN_FILENO, TCSANOW, &original_term_state))
        return -1;

    return 0;
}

