/*
 * image processing
 * main function
 * CSF Assignment 4
 * C. Levitt
 * clevitt1@jh.edu
 * T. Karani
 * tkarani1@jh.edu
*/

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
	        freePlugins(a);
		fatalError("Unknown command name\n");
	    } else {
		    printf("Loaded %u plugin(s)\n", a.numPlugins); 
		    for (uint32_t i = 0; i < a.numPlugins; i++) {
		        printf("%s: %s\n", plugins[i].get_plugin_name(), plugins[i].get_plugin_desc());
		    }
	    }
    }
    // exec command
    if (argc < 5 || strcmp(argv[1], "exec") != 0) {
	freePlugins(a);
	fatalError("Unknown command name"); 
    }   
    int pluginLoc = -1; 
    for (uint32_t i = 0; i < a.numPlugins; i++) {  // find a plugin whose name matches - name is in argv[2] 
	    if (strcmp(argv[2], plugins[i].get_plugin_name()) == 0) {
		    pluginLoc = i; 
		    break;
	    }
    }
    if (pluginLoc == -1) { 
	freePlugins(a); 
        fatalError("Specified plugin not found");
    }
    Image * inputImg = img_read_png(argv[3]);     // read input image
    if (inputImg == NULL) {
	    img_destroy(inputImg); 
	    freePlugins(a);	
	    fatalError("Invalid image file\n"); 
    }
    void * argPtr = plugins[pluginLoc].parse_arguments(argc - 5, argv + 5); // will be freed by plugin
    if (argPtr == NULL) {
	    img_destroy(inputImg);
	    freePlugins(a);
	    fatalError("Improper conditions for plugin\n"); 
    }    
    Image * transformedImg = plugins[pluginLoc].transform_image(inputImg, argPtr); // execute transformation
    if (transformedImg == NULL) {
        img_destroy(inputImg);
	freePlugins(a);
        fatalError("Invalid image transformation\n");
    }
    if (img_write_png(transformedImg, argv[4]) == 0) {   // write as png
	img_destroy(inputImg);
	img_destroy(transformedImg);
	freePlugins(a); 
        fatalError("Transofrmed image could not be written as png");
    }
    img_destroy(inputImg);
    img_destroy(transformedImg);
    freePlugins(a);
    
    return 0; 

}
