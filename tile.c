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
	exit(1); 
        //fatalError("Incorrect number of arguments for expose\n");
    }
    unsigned factor = 0U;
    if (atol(args[0]) < 0) {
	exit(1);
        //fatalError("Factor cannot be negative\n");
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
/*
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
            block_widths[i*factor + c] += 1;
        }
    }

    unsigned counter = 0; 
  */  

    for (unsigned h = 0; h < block_height; h++) {  // will traverse over all pixels of a tile
        for (unsigned w = 0; w < block_width; w++) {
            for (unsigned n = 0, b_r = h, b_c = w; n < num_blocks; n++, b_c+=block_width) { // visits each block
                if (b_c >= width) {                                                         // b_r is row of pixel in block
                    b_c = w;                                                                // b_c is column of pixel in block
                    b_r += block_width;
                }
                
		        out->data[b_r*width + b_c] = source->data[h*factor*width + w*factor];
                // printf("out[%u] = data[%u]\n", b_r*width + b_c, h*factor*width + w*factor);
            }
        }
    }
/*
    for (unsigned r = 0; r < block_heights[0]; r++) {  // will traverse over all pixels of a tile - r and c are indexes within a tile
        for (unsigned c = 0; c < block_widths[0]; c++) {
            unsigned pixelVal = source->data[r*factor*width + c*factor]; // only need to calculate once for each location
            for (unsigned n = 0; n < num_blocks; n++) { // visits each tile
                unsigned tileRow = n / factor;      // calculating indexes of tile
                unsigned tileCol = n % factor;
                if (r >= block_heights[n] || c >= block_widths[n]) { // check if current tile has this pixel
                    continue;
                }
                // calculating location of pixel on output image
                unsigned row = r, column = c;
                for (unsigned i = 0; i < tileRow; i++) {
                    row += block_heights[i*factor];
                }
                for (unsigned i = 0; i < tileCol; i++) {
                    column += block_widths[i];
                }
		        out->data[row*width + column] = pixelVal;
                // printf("out[%u] = data[%u]\n", b_r*width + b_c, h*factor*width + w*factor);
            }
        }
    }
*/

    free(args); 
    return out; 
}                       

	
