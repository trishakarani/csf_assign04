/*
 * image processing
 * expose plugin - multiplies rgb values by a given factor
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
    float factor;
};

const char *get_plugin_name(void) {
    return "expose"; 
}

const char *get_plugin_desc(void) {
    return "adjust the intensity of all pixels";
}

void *parse_arguments(int num_args, char *args[]) {
    if (num_args != 1) {
	    return NULL;
    }
    float factor = atof(args[0]);
    if (factor < 0) {
	    return NULL;
    }
    struct Arguments argObj = {factor};
    struct Arguments * argPtr = malloc(sizeof(struct Arguments));
    *argPtr = argObj;
    return argPtr;
}

//Helper function to expose the red, green, blue components 
static uint32_t expose_rgb(uint32_t pix, float factor) {
	uint8_t r, g, b, a; 
	img_unpack_pixel(pix, &r, &g, &b, &a); 
	if (r*factor > 255) { r = (uint8_t)255; } 
	    else { r  = (uint8_t)(r*factor); }
	if (g*factor > 255) { g = (uint8_t)255; } 
        else { g  = (uint8_t)(g*factor); }
    if (b*factor > 255) { b = (uint8_t)255; } 
        else { b  = (uint8_t)(b*factor); }
	return img_pack_pixel(r, g, b, a); 
}


Image *transform_image(Image *source, void *arg_data) {
	struct Arguments *args = arg_data;
    float factor = args->factor;
	//Allocate a result Image
	Image *out = img_create(source->width, source->height) ;
	if (!out) {
		free(args); 
		return NULL; 
	}
	unsigned num_pixels = source->width * source->height; 
	for (unsigned i = 0; i < num_pixels; i++) {
		out->data[i] = expose_rgb(source->data[i], factor); 
	}
	free(args); 
	return out; 
}
