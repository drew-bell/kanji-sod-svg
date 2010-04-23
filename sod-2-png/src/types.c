#include <string.h>
#include <svg-cairo.h>
#include "svg2png.h"
#include <stdbool.h>
#include "types.h"
#include <stdlib.h>
#include <unistd.h>

// determine if the file is of type *type (this should be a extention including the "." )
int is_type(char *file,char *type) {
	
	// A temp variable to hold the file extention
	char *ext;
	
	// set the extention of the file to the ext variable
	ext = strrchr(file, '.');
	
	// Compare the extention with the type in a non-case sensitive way
	if ((strcasecmp(ext,type) == 0)) {
		
		// if it is an image of type, return true
		return true;
	}
	
	// if not of specified type, return false
	return false;
} // is type

char* ext(char *file) {
	
	// Return the extention of the file name
	return strrchr(file,'.');
}

svg_cairo_status_t (*get_render_function(char *file))(FILE*,FILE*,double,int,int) {
	
	// Check if it is a PNG to be output
	if (is_type(file,".png")) {
		
		// Set the output function to render_to_png
		return &render_to_png;
	} else {
		
		// return a null pointer if the file type is unknown
		return NULL;
	}
} // 

// check to see if the file exists
int file_exists(char *file) {
	if (false == access(file,W_OK)) {
		return true;
	}
	return false;
}