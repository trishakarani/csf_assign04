/*
 * image processing
 * tile plugin - produces an image fill of nxn tiles of original image
 * CSF Assignment 4
 * C. Levitt
 * clevitt1@jh.edu
 * T. Karani
 * tkarani1@jh.edu
*/

#include <stdlib.h>
#include <stdio.h>
#include "image_plugin.h"

struct Arguments {
    unsigned factor; 
};

const char *get_plugin_name(void) {
	return "tile";
}

const char *get_plugin_desc(void) {
	return "tile source image in an NxN arrangement";
}

void *parse_arguments(int num_args, char *args[]) {
    if (num_args != 1) {
	    return NULL;
    }
    unsigned factor = 0U;
    if (atol(args[0]) <= 0) {
	    return NULL;
    } else {
        sscanf(args[0], "%u", &factor);
    }
    struct Arguments argObj = {factor};
    struct Arguments * argPtr = malloc(sizeof(struct Arguments));
    *argPtr = argObj;
    return argPtr;
}

Image *transform_image(Image *source, void *arg_data) {
    struct Arguments *args = arg_data;
    unsigned factor = args->factor;
    Image *out = img_create(source->width, source->height);
    if (!out) {
        free(args);
        return NULL;
    }
    unsigned width = source->width, height = source->height;
    unsigned num_pixels = width * height;
    unsigned num_blocks = factor * factor; 
    unsigned block_widths[num_blocks];
    unsigned block_heights[num_blocks];
    unsigned block_height = height/factor, block_width = width/factor;
    // calculate and store dimensions of each tile
    for (unsigned i = 0; i < num_blocks; i++) {
        block_widths[i] = block_width;
        block_heights[i] = block_height;
    }
    unsigned width_rem = width % factor, height_rem = height % factor;
    for (unsigned i = 0; i < width_rem; i++) {  // for each "1" in the remainder, adds it to the widths of a column, starting from left
        for (unsigned r = 0; r < factor; r++) {
            block_widths[i + r*factor] += 1;
        }
    }
    for (unsigned i = 0; i < height_rem; i++) {  // for each "1" in the remainder, adds it to the heights of a row, starting from top
        for (unsigned c = 0; c < factor; c++) {
            block_heights[i*factor + c] += 1;
        }
    }

    for (unsigned r = 0; r < block_heights[0]; r++) {  // will traverse over all pixels of a tile - r and c are indexes within a tile
        for (unsigned c = 0; c < block_widths[0]; c++) {
            unsigned pixelVal = source->data[r*factor*width + c*factor]; // only need to calculate once for each pixel in a tile
            for (unsigned n = 0; n < num_blocks; n++) { // visits each tile
                unsigned tileRow = n / factor;      // calculating indexes of current tile
                unsigned tileCol = n % factor;
                if (r >= block_heights[n] || c >= block_widths[n]) { // check if current tile has this pixel
                    continue;
                }
                unsigned row = r, column = c;      // calculating location of pixel on output image
                for (unsigned i = 0; i < tileRow; i++) {
                    row += block_heights[i*factor];
                }
                for (unsigned i = 0; i < tileCol; i++) {
                    column += block_widths[i];
                }
		        out->data[row*width + column] = pixelVal;
            }
        }
    }
    free(args); 
    return out; 
}                       

	
