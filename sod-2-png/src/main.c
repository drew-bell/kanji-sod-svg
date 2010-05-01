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
	xmlDocPtr ND = NULL;

	// process all the cli options and return them in the structure
	process_args(argv,argc,opts);

	// Do a little testing on the xml libs to check they are OK
	LIBXML_TEST_VERSION
	
	// Allocate the xmldoc handle
	xmlDocPtr doc;

	// Read the file into memory
	doc = xmlParseFile(opts->svg_file);

	// Pass the root of the doc in memory for removal of unwanted parts and return a node ptr
	ND = process_xml_options(doc, opts);

	// If no output file has been specified, dump the edited svg to stdout.
	if (is_type(opts->out_format,".svg")) {

		FILE *outf;

		if (is_type(opts->out_file,"stdout")) {
			
			// set the output to stdout
			outf = stdout;
			
			} else {

			// open file for writing
				outf = fopen(opts->out_file,"w");
			}
			// Dump the file to document
				xmlDocDump(outf,doc);	

	} else {
		
		if (opts->sequential_images) {

			// Create the image sequence
			create_sequential_images(ND , opts);

			// set the options to remove everything for the final images
			opts->no_arrows = true;
			opts->no_numbers = true;
			opts->no_Start_mark = true;
			
			// reprocess the file
			ND = process_xml_options(doc, opts);

			// Create a single image.
			push_out_image(ND, opts, 0, NULL);

		} else {

			// Create a single image.
			push_out_image(ND, opts, 0, NULL);
			
		}
	}

	/* Clean up */
	xmlCleanupParser();
	xmlMemoryDump();
	xmlFreeDoc(doc);

	// release all the memory allocated for the options struct
	cleanup_o(opts);

	return (0);
} // main


