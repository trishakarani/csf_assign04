#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include "image_plugin.h"
#include "pnglite.h"
#include "imgproc_functions.h"

int main(int argc, char** argv) {

    if (argc == 1) {
	    fprintf(stderr, "Usage: imgproc <command> [<command args...>]\nCommands are:\n\tlist\n\texec <plugin> <input img> <output img> [<plugin args...>]\n");
	    return 1; 
    }

    AllPlugins all = getPlugins();
    Plugin * plugins = all.allPlugins; 
    // list command
    if (argc == 2) {
	    if (strcmp(argv[1], "list") != 0) {
	        fatalError("Unknown command name\n");
            //printf("Error: Unknown command name\n");
            //exit(1);
	    } else {
		    printf("Loaded %u plugin(s)\n", all.numPlugins); 
		    for (uint32_t i = 0; i < all.numPlugins; i++) {
		        printf("%s: %s\n", plugins[i].get_plugin_name(), plugins[i].get_plugin_desc());
		    }
	    }
	   
    }
    
    // exec command
    if (strcmp(argv[1], "exec") != 0) {
	    fatalError("Unknown command name"); 
    }
    // find a plugin whose name matches
    // name is in argv[2]    
    Plugin * cursor = plugins;
    while (cursor != NULL) {
        if (strcmp(argv[2], cursor->get_plugin_name()) == 0) {
            break;
        }
        cursor += 1;
    } // cursor should point to correct Plugin object
    if (cursor == NULL) {
        fatalError("Specified plugin not found"); 
    }
    Image * inputImg = img_read_png(argv[3]);
    void * argPtr = cursor->parse_arguments(argc - 5, argv + 5); // will be freed by plugin
    Image * transformedImg = cursor->transform_image(inputImg, argPtr); // check for null?
    if (img_write_png(transformedImg, argv[4]) == 0) {
        fatalError("Transofrmed image could not be written as png");
    }

    return 0; 



}
