// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

unsigned int calc_color_margin(unsigned short window_width, unsigned int* color_width) {
	window_width /= 2;

	for (unsigned int margin = window_width / 6; margin < window_width; margin++) {
		if ((window_width - margin) % 4) {
			*color_width = (window_width - margin) / 4;
			return margin;
		}
	}

	return 0;
}
