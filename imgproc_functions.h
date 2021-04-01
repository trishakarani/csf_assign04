
#ifndef IMGPROC_FUNCTIONS_H
#define IMGPROC_FUNCTIONS_H

#include <stdint.h>
#endif

typedef struct {
    void *handle;
    const char *(*get_plugin_name)(void);
    const char *(*get_plugin_desc)(void);
    void *(*parse_arguments)(int num_args, char *args[]);
    struct Image *(*transform_image)(struct Image *source, void *arg_data);
} Plugin;

typedef struct {
    uint32_t numPlugins; 
    Plugin * allPlugins; 
} AllPlugins; 

/*
*  Searches for available plugin shared libraries, creates an allocated
*  array of Plugins to represent each
*
*
*/
AllPlugins getPlugins();
