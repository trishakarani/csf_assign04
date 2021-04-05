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
	return "generates an image containing an N x N arrangement of tiles";
}

void *parse_arguments(int num_args, char *args[]) {
    if (num_args != 1) {
        fatalError("Incorrect number of arguments for expose\n");
    }
    unsigned factor = 0U;
    if (atol(args[0]) < 0) {
        fatalError("Factor cannot be negative\n");
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
    unsigned block_height = height/factor, block_width = width/factor; 
    unsigned width_rem = width % factor, height_rem = height % factor; 
    unsigned counter = 0; 
    

    for (unsigned h = 0; h < block_height; h++) {
        for (unsigned w = 0; w < block_width; w++) {
            for (unsigned n = 0, b_r = h, b_c = w;n < num_blocks; n++, b_c+=block_width) {
                if (b_c >= width) {
                    b_c = w;
                    b_r += block_width;
                }
                
		        out->data[b_r*width + b_c] = source->data[h*factor*width + w*factor];
               // printf("out[%u] = data[%u]\n", b_r*width + b_c, h*factor*width + w*factor);

            }
        }

    }
    free(args); 
    return out; 
}                       

	
