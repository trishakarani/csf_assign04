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
	return "mirrorh";
}

const char *get_plugin_desc(void) {
	return "mirror image horizontally";
}

void *parse_arguments(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter

	if (num_args != 0) {
		return NULL;
	}
	return calloc(1, sizeof(struct Arguments));
}

// Helper function to swap the blue and green color component values.
static uint32_t swap_bg(uint32_t pix) {
	uint8_t r, g, b, a;
	img_unpack_pixel(pix, &r, &g, &b, &a);
	return img_pack_pixel(r, b, g, a);
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
	unsigned num_pixels = source->width * source->height;
    unsigned width = source->width;
    unsigned height = source->height;
    for (unsigned r = 0; r < height; r++) {
        for (unsigned c = 0; c < width; c++) {
            out->data[r*c] = source->data[r*(width - 1- c)];
        }
    }


	free(args);

	return out;
}
