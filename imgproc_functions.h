
#ifndef IMGPROC_FUNCTIONS_H
#define IMGPROC_FUNCTIONS_H

#include <stdint.h>
#include "image.h"

typedef struct {
    void *handle;
    const char *(*get_plugin_name)(void);
    const char *(*get_plugin_desc)(void);
    void *(*parse_arguments)(int num_args, char *args[]);
    Image *(*transform_image)(Image *source, void *arg_data);
} Plugin;

typedef struct {
    uint32_t numPlugins; 
    Plugin * allPlugins; 
} AllPlugins; 

/*
*  Searches for available plugin shared libraries, creates an allocated
*  array of Plugins to represent each
*
*/
AllPlugins getPlugins(void);

/* 
 * Closes all open library plugins using dlclose 
 * and frees the allocated Plugins memory */
void freePlugins(AllPlugins a);

/* 
 * Prints the given message to stderr and exits the program
 */
void fatalError(char * msg);

#endif
