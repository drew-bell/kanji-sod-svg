#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "arg.h"
#include "editing.h"
#include "svg2png.h"

int main (int argc, char **argv) {

	// A structure to hold all the cli arguments
	argo *opts = malloc(sizeof(opts));

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
		fprintf(stderr,"No file to process.\n");
		exit(0);
	} else {

		// Read the file into memory
		doc = xmlParseFile(opts->svg_file);
	}

	// Get the root element node
	root_element = xmlDocGetRootElement(doc);

	// Pass the first node of the doc in memory for removal of unwanted parts
	process_xml_options(root_element, opts);

	FILE *png, *temp_file, *svg;

	temp_file = fopen("tmpfile","w");

	// output the altered file
	xmlDocDump(temp_file,doc);

	fclose(temp_file);

	if (NULL == opts->png_file) {
		png = stdout;
	} else {
		png = fopen(opts->png_file,"w");
	}

	svg = fopen("tmpfile","r");

	//render to png
	render_to_png(svg, png, 1.0, opts->width, opts->height);

	/* Clean up */
	xmlCleanupParser();
	xmlMemoryDump();
	xmlFreeDoc(doc);
	fclose(svg);
	fclose(png);

	unlink("tmpfile");
	return (0);

} // main
