/*
 * image processing
 * mirrorv plugin - reflects an image vertically
 * CSF Assignment 4
 * C. Levitt
 * clevitt1@jh.edu
 * T. Karani
 * tkarani1@jh.edu
*/

#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	int dummy;
};

const char *get_plugin_name(void) {
	return "mirrorv";
}

const char *get_plugin_desc(void) {
	return "mirror image vertically";
}

void *parse_arguments(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter
	if (num_args != 0) {
		return NULL;
	}
	return calloc(1, sizeof(struct Arguments));
}


Image *transform_image(Image *source, void *arg_data) {
	struct Arguments *args = arg_data;
	Image *out = img_create(source->width, source->height);
	if (!out) {
		free(args);
		return NULL;
	}
    // array in row-major order, one row follows another
    unsigned width = source->width;
    unsigned height = source->height;
    for (unsigned r = 0; r < height; r++) {
        for (unsigned c = 0; c < width; c++) {
            out->data[r*width + c] = source->data[(height - 1 - r)*width + c];
        }
    }
	free(args);
	return out;
}
