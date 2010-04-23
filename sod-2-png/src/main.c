#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "arg.h"
#include "editing.h"
#include "svg2png.h"
#include "types.h"
#include <unistd.h>

int main (int argc, char **argv) {

	// A structure to hold all the cli arguments
	argo opts = (argo)malloc(sizeof(options));

	// Null out all options before using
	null_options(opts);
	
	// create an element to hold the root node
	xmlNode *root_element = NULL;

	// process all the cli options and return them in the structure
	process_args(argv,argc,opts);

	// Do a little testing on the xml libs to check they are OK
	LIBXML_TEST_VERSION

	// Allocate the xmldoc handle
	xmlDocPtr doc;

	if (opts->svg_file == NULL) {

		// Tell the user that the program quit due to lack of input file
		fprintf(stderr,"No file to process.\n");

		// Exit the program normally
		exit(0);
	} else {

		// Read the file into memory
		doc = xmlParseFile(opts->svg_file);
	}

	// Pass the doc in memory for removal of unwanted parts
	process_xml_options(doc, opts);

	// If no output file has ben specified, dump the edited svg to stdout.
	if (opts->out_file == NULL) {
		
		// Dump the file
		xmlDocDump(stdout,doc);

	} else if (is_type(opts->out_file,".svg")) {
		
		// File handle
		FILE *output_format;
		
		// open file for writing
		output_format = fopen(opts->out_file,"w");

		// Dump the file to document
		xmlDocDump(output_format,doc);

	} else {

		// Pointer to function
		svg_cairo_status_t (*render_functptr)(FILE*,FILE*,double,int,int) = NULL;
		
		// point the render function to the correct one
		render_functptr = get_render_function(opts->out_file);
		
		if (NULL != render_functptr) {
			// Create file handles
			FILE *output_format, *temp_file, *svg;
			
			// Create a tmp intermediary file
			temp_file = fopen("/tmp/svg2png_tmpfile","w");
			
			// output the altered file
			xmlDocDump(temp_file,doc);
			
			// close the temp file
			fclose(temp_file);
			
			// open file for writing
			output_format = fopen(opts->out_file,"w");
			
			// open the temp file for conversion to png
			svg = fopen("/tmp/svg2png_tmpfile","r");
			
			// render the output file
			render_functptr(svg, output_format, 1.0, opts->width, opts->height);
			
			// close all files
			fclose(svg);
			fclose(output_format);
			
			// delete the temp file
			unlink("/tmp/svg2png_tmpfile");
		} else {

			// Output error for the user
			fprintf(stderr,"Unknown output file type : %s\n", ext(opts->out_file));
		}
	}

	/* Clean up */
	xmlCleanupParser();
	xmlMemoryDump();
	xmlFreeDoc(doc);
	free(opts->svg_file);
	free(opts->out_file);
	free(opts);

	return (0);
} // main


