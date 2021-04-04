#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
    float factor;
};

const char *get_plugin_name(void) {
    return "expose"; 
}

const char *get_plugin_desc(void) {
    return "changes all red/green/blue color component values by a specified factor"; 
}

void *parse_arguments(int num_args, char *args[]) {
    //check if factor is neg - fatalerror
    if (num_args != 1) {
        fatalError("Incorrect number of arguments for expose\n");
    }
    float factor = atof(args[0]);
    if (factor < 0) {
        fatalError("Factor can not be negative\n");
    }
    struct Arguments argObj = {factor};
    struct Arguments * argPtr = malloc(sizeof(struct Arguments));
    *argPtr = argObj;
    return argPtr;
}

//Helper function to expose the red, green, blue components 
static uint32_t expose_rbg(uint32_t pix, float factor) {
	uint8_t r, g, b, a; 
	img_unpack_pixel(pix, &r, &g, &b, &a); 
	
	r *= factor; g *= factor; b *= factor; 
	if (r > 255) { r = 255; }
	if (g > 255) { g = 255; }
	if (b > 255) { b = 255; }

	return img_pack_pixel(r, b, g, a); 

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
		out->data[i] = expose_rbg(source->data[i], factor); 
	}

	free(args); 
	return out; 
}
