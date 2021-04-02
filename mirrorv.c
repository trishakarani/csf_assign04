//
// Example plugin: it just swaps the blue and green color component
// values for each pixel in the source image.
//

#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	// just define a single dummy field.
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

	// Allocate a result Image
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
