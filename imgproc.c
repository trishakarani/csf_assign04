#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include "image_plugin.h"
#include "pnglite.h"

int main(int argc, char** argv) {

    if (argc == 1) {
	fprintf(stderr, "Usage: imgproc <command> [<command args...>]\nCommands are:\n\tlist\n\texec <plugin> <input img> <output img> [<plugin args...>]");
	return 1; 
    }

    if (argc == 2) {
	if (strcmp(argv[1], "list") != 0) {
	    fprintf(stderr, "Error: Unknown command name\n");
	    return 1; 
	}
	printf("Loaded x plugin(s)\n"); 
    }
    
    if (strcmp(argv[1], "exec") != 0) {
	fprintf(stderr, "Error: Unknown command name\n"); 
	return 1; 
    }

    return 0; 



}
