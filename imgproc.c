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

    AllPlugins a = getPlugins();
    Plugin * plugins = a.allPlugins; 
    // list command
    if (argc == 2) {
	    if (strcmp(argv[1], "list") != 0) {
            free(plugins);
	    fatalError("Unknown command name\n");
            //printf("Error: Unknown command name\n");
            //exit(1);
	    } else {
		    printf("Loaded %u plugin(s)\n", a.numPlugins); 
		    for (uint32_t i = 0; i < a.numPlugins; i++) {
		        printf("%s: %s\n", plugins[i].get_plugin_name(), plugins[i].get_plugin_desc());
		    }
	    }
	   
    }
    
    // exec command
    if (strcmp(argv[1], "exec") != 0) {
        free(plugins);
	fatalError("Unknown command name"); 
    }
    // find a plugin whose name matches
    // name is in argv[2]    
    Plugin * cursor = plugins;
    int pluginLoc = -1; 
    for (uint32_t i = 0; i < a.numPlugins; i++) {
	    if (strcmp(argv[2], plugins[i].get_plugin_name()) == 0) {
		pluginLoc = i; 
		break;
	    }
    }

    if (pluginLoc == -1) { 
	free(plugins);
        fatalError("Specified plugin not found");
    }
    
/*
    while (cursor != NULL) {
        if (strcmp(argv[2], cursor->get_plugin_name()) == 0) {
            break;
        }
        cursor += 1;
    } // cursor should point to correct Plugin object
    if (cursor == NULL) {
        free(plugins);
        fatalError("Specified plugin not found"); 
    }
  */  
    Image * inputImg = img_read_png(argv[3]);
    if (inputImg == NULL) {
	free(plugins); 
	fatalError("Invalid image file\n"); 
    }
    void * argPtr = plugins[pluginLoc].parse_arguments(argc - 5, argv + 5); // will be freed by plugin
    Image * transformedImg = plugins[pluginLoc].transform_image(inputImg, argPtr); // check for null?
    if (img_write_png(transformedImg, argv[4]) == 0) {
        free(plugins);
        fatalError("Transofrmed image could not be written as png");
    }
    //close the handle of each plugin
    free(plugins);
    
    return 0; 



}
