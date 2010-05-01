#include <string.h>
#include <svg-cairo.h>
#include "svg2png.h"
#include <stdbool.h>
#include "types.h"
#include <stdlib.h>
#include <unistd.h>
#include "arg.h"

char* ext(char *file) {
	
	// Return the extention of the file name
	return strrchr(file,'.');
}

// determine if the file is of type *type (this should be a extention including the "." )
int is_type(char *file,char *type) {
	
	// A temp variable to hold the file extention
	char *exts;
	
	// set the extention of the file to the ext variable
	exts = ext(file);

	// Check for NULL
	if (NULL == exts) {
		printf("No extention found\n");
		help();
	}

	if (NULL != exts) {

		// Compare the extention with the type in a non-case sensitive way
		if ((strcasecmp(exts,type) == 0)) {

			// if it is an image of type, return true
			return true;
		}
	
		// if not of specified type, return false
		return false;
	}

	return false;
} // is type

svg_cairo_status_t (*get_render_function(char *format))(FILE*,FILE*,double,int,int) {

	// Check if it is a PNG to be output
	if (0 == strcasecmp(format,".png")) {
		
		// Set the output function to render_to_png
		return &render_to_png;
	}

/**************************************************/
/***   ADD NEW OUTPUT FORMATS HERE              ***/
/**************************************************/
	
	// return a null ptr if unsupported file format
	return NULL;
} // 

// check to see if the file exists
int file_exists(char *file) {
	if (false == access(file,W_OK)) {
		return true;
	}
	return false;
}

int available_formats(char* format) {

	// Check for a NULL before continuing
	if (NULL == format) return false;

	// Array of strings to check agains
	char *formats[4];

	// counter for stepping through the array
	int i = 0;

	// create the available formats string array
	formats[0] = (char*)".png";
	formats[1] = (char*)".svg";
	formats[2] = NULL;

	// search for the desired format
	do {
		
		// compare the array string and the desired format string
		if (0 == strcasecmp(format,formats[i])) {

			// return true if found
			return true; 
		}

		// increase the step counter
		i++;
	} while (NULL != formats[i]);

	// if not found 
	return false;
}
