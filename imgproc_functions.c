
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
#include "imgproc_functions.h"


AllPlugins getPlugins(void) {
    //find directory with plugin shared libraries 
    char * pluginDirName = getenv("PLUGIN_DIR");   //check if PLUGIN_DIR is set
    if (pluginDirName == NULL) { //PLUGIN_DIR not set
	    pluginDirName = "./plugins";
    }

    DIR * pluginDirPtr = opendir(pluginDirName); 
    assert(pluginDirPtr != NULL); //error with opendir 

    Plugin * plugins = malloc(sizeof(Plugin) * 5); //HARDCODE THE SIZE?
    int pluginCount = 0;      

    struct dirent * filePtr = readdir(pluginDirPtr); 
    while(filePtr != NULL) {
	if (strstr(filePtr->d_name, ".so")) { //WILL ANY OTHER FILE HAVE .SO
	    pluginCount++; 
	    if (pluginCount > 5) {
		    plugins = (Plugin *)realloc(plugins, sizeof(Plugin) * pluginCount); 
	    }
	    Plugin newPlugin; 
	    newPlugin.handle = dlopen(filePtr->d_name, RTLD_LAZY); 
	    *(void **) (&newPlugin.get_plugin_name) = dlsym(newPlugin.handle, "get_plugin_name");
	    *(void **) (&newPlugin.get_plugin_desc) = dlsym(newPlugin.handle, "get_plugin_desc"); 
	    *(void **) (&newPlugin.parse_arguments) = dlsym(newPlugin.handle, "parse_arguments"); 
	    *(void **) (&newPlugin.transform_image) = dlsym(newPlugin.handle, "transform_image");

	    if (newPlugin.get_plugin_desc == NULL || newPlugin.get_plugin_name == NULL || 
		newPlugin.parse_arguments == NULL || newPlugin.transform_image == NULL) {
		fatalError("Required API function not found\n"); 
	    }
	    plugins[pluginCount-1] = newPlugin;  
	    filePtr = readdir(pluginDirPtr);
	} else {
	    filePtr = readdir(pluginDirPtr);
	    continue; 

	}

    }

    closedir(pluginDirPtr);

    if (pluginCount == 0) {
	    free(plugins); 
	    plugins = NULL; 
    } else if (pluginCount < 5) {
	    plugins = (Plugin *)realloc(plugins, sizeof(Plugin) * pluginCount);
    }

    AllPlugins all = {pluginCount, plugins};

    return all; 	

}

void fatalError(char * msg) {
    printf("Error: ");
    printf("%s", msg);
    exit(1);
}
