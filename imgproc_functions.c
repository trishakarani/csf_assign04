
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
    assert(pluginDirPtr != NULL); //check for error with opendir 

    Plugin * plugins = malloc(sizeof(Plugin) * 5); //create array of Plugins starting with size 5 
    int pluginCount = 0;      

    struct dirent * filePtr = readdir(pluginDirPtr); 
    while(filePtr != NULL) { //read until eof 
	    if (strstr(filePtr->d_name, ".so")) { //check if d_name is for a plugin file (.so)
	        pluginCount++; 
	        if (pluginCount > 5) {
		        plugins = (Plugin *)realloc(plugins, sizeof(Plugin) * pluginCount); 
	        }

		//create path to plugin file 
	        char pluginPath[1000];
	        sprintf(pluginPath, "%s/%s", pluginDirName, filePtr->d_name);
	        Plugin newPlugin; 
	        newPlugin.handle = dlopen(pluginPath, RTLD_LAZY);

		//get function pointers for all fields of the Plugin
	        *(void **) (&newPlugin.get_plugin_name) = dlsym(newPlugin.handle, "get_plugin_name");
	        *(void **) (&newPlugin.get_plugin_desc) = dlsym(newPlugin.handle, "get_plugin_desc"); 
	        *(void **) (&newPlugin.parse_arguments) = dlsym(newPlugin.handle, "parse_arguments"); 
	        *(void **) (&newPlugin.transform_image) = dlsym(newPlugin.handle, "transform_image");

		//exit program if any functions do not exist 
	        if (newPlugin.get_plugin_desc == NULL || newPlugin.get_plugin_name == NULL || 
		        newPlugin.parse_arguments == NULL || newPlugin.transform_image == NULL) {
		        fatalError("Required API function not found\n"); 
	        }
	        plugins[pluginCount-1] = newPlugin;  
	        filePtr = readdir(pluginDirPtr);  //continue reading file
	    } else { //if not a .so file, continue reading file
	        filePtr = readdir(pluginDirPtr);
	        continue; 
        }

    }

    closedir(pluginDirPtr);

    if (pluginCount == 0) { //if no plugins were valid, exit program
	    free(plugins); 
	    fatalError("No valid plugins\n"); 
    } else if (pluginCount < 5) { //reduce size of allocated memory if needed
	    plugins = (Plugin *)realloc(plugins, sizeof(Plugin) * pluginCount);
    } 

    AllPlugins a = {pluginCount, plugins};

    return a; 	
}

void freePlugins(AllPlugins a) {
    Plugin * plugins = a.allPlugins; 
    for (uint32_t i = 0; i < a.numPlugins; i++) {
	dlclose(plugins[i].handle); 
    }
    free(plugins); 
}

void fatalError(char * msg) {
    printf("Error: ");
    printf("%s", msg);
    exit(1);
}
