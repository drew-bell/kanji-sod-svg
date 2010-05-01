#include <stdio.h>
#include "editing.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#pragma mark Prototypes
static void rm_part(xmlNode* a_node, char *node_type);

#pragma mark Functions

/**************************************************************/
/*** Delete a node from the document based on the node type ***/
/**************************************************************/
void rm_part(xmlNode* a_node, char *node_type) {

	// Create a xmlNode for use
    xmlNode *cur_node = NULL;

	// Step through teh xml nodes
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

		// Check the current node is an element and not a space
        if (cur_node->type == XML_ELEMENT_NODE) {
	
			// Look at the type of node found and check if it is what we are looking for
			if (!strcmp((char*)cur_node->name,node_type)) {

				// remove the node from the document structure
				xmlUnlinkNode(cur_node);
			}
		}

		// Search recurrsively
        rm_part(cur_node->children,node_type);
    }
}

/**************************************************************/
/* CURRENTLY UNUSED */
/**************************************************************/
void print_EL_names(xmlNode* a_node) {
    xmlNode *cur_node = NULL;
	
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }
		
        print_EL_names(cur_node->children);
    }
}

/**************************************************************/
/*** Process the options entered at the command line        ***/
/**************************************************************/
xmlDocPtr process_xml_options(xmlDocPtr doc, argo opts) {

	// A pointer to the root element of the file
	xmlNode *root_EL = NULL;

	// Get the root element
	root_EL = xmlDocGetRootElement(doc);
	
	if (opts->no_arrows) {
		rm_part(root_EL,"polyline");
	}
		
	if (opts->no_numbers) {
		rm_part(root_EL,"text");
	}
	
	if (opts->no_Start_mark) {
		rm_part(root_EL,"circle");
	}

	return doc;
}

/**************************************************************/
/*** Setup the basic XML for a new SVG file                 ***/
/**************************************************************/
void setup_basic_doc(xmlDocPtr ND) {
	
	// Node pointers for the new doc
	xmlNodePtr ND_Rnode = NULL;

	// DTD pointer
	xmlDtdPtr dtd = NULL;

	// Add the <!DOCTYPE info>
	dtd = xmlCreateIntSubset(ND, BAD_CAST "svg", BAD_CAST "-//W3C//DTD SVG 1.1//EN", BAD_CAST "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd");

	// Create the root node
	ND_Rnode = xmlNewNode(NULL, BAD_CAST "svg");

	// Set Attributes for the node
	xmlNewProp(ND_Rnode, BAD_CAST "version", BAD_CAST "1.1");
	xmlNewProp(ND_Rnode, BAD_CAST "id", BAD_CAST "Layer_1");
	xmlNewProp(ND_Rnode, BAD_CAST "xmlns", BAD_CAST "http://www.w3.org/2000/svg");
	xmlNewProp(ND_Rnode, BAD_CAST "xmlns:xlink", BAD_CAST "http://www.w3.org/1999/xlink");
	xmlNewProp(ND_Rnode, BAD_CAST "x", BAD_CAST "0px");
	xmlNewProp(ND_Rnode, BAD_CAST "y", BAD_CAST "0px");
	xmlNewProp(ND_Rnode, BAD_CAST "width", BAD_CAST "1000px");
	xmlNewProp(ND_Rnode, BAD_CAST "height", BAD_CAST "1000px");
	xmlNewProp(ND_Rnode, BAD_CAST "viewBox", BAD_CAST "0 0 1000 1000");
	xmlNewProp(ND_Rnode, BAD_CAST "enable-background", BAD_CAST "new 0 0 1000 1000");
	xmlNewProp(ND_Rnode, BAD_CAST "xml:space", BAD_CAST "preserve");
	
	// Set the node just created to the documents root
	xmlDocSetRootElement(ND, ND_Rnode);

}

/**************************************************************/
/*** Output sequential images                               ***/
/**************************************************************/
void create_sequential_images(xmlDocPtr OD, argo opts) {

	// A counter for how many files in the sequence have been output
	int n = 1;

	// make the output file pretty
	xmlIndentTreeOutput = 1;

	// Document pointer to the memory document to be built
	xmlDocPtr ND = NULL;

	// Node pointers for the new doc
	xmlNodePtr ND_Rnode = NULL, node_copy = NULL, OD_Rnode = NULL;

	OD_Rnode = xmlDocGetRootElement(OD);

	// Create document and a node
	ND = xmlNewDoc(BAD_CAST "1.0");

	// Setup the basic output document
	setup_basic_doc(ND);

	// Get the root node for the new document
	ND_Rnode = xmlDocGetRootElement(ND);
	
	// Start parsing the original file
	xmlNode *cur_node = NULL;
	cur_node = OD_Rnode->children;
	cur_node = cur_node->next;

	char kanji[FILENAME_MAX];
	
	strncpy(kanji,opts->svg_file,strlen(opts->svg_file)-4);
	
	for (cur_node = cur_node->next; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

			// Remove all the arrows, text and startmarks in the new doc up to this point
			rm_part(ND_Rnode,"polyline");
			rm_part(ND_Rnode,"text");
			rm_part(ND_Rnode,"circle");

			// Change the colour of the strokes to gray
			change_fill_colour(ND_Rnode,"path","#333333");

			// Copy the current node
			node_copy = xmlCopyNode(cur_node, 1);
			
			// add the copy to the New Document
			xmlAddChild(ND_Rnode, node_copy);
			
			// Make an image
			push_out_image(ND,opts,n,kanji);

			// increase the filename number
			n++;
		}
    }
}

void dump_tmp(xmlDocPtr ND) {
	
	// File handle for the temp output file
	FILE *tf;
	
	// Create a tmp intermediary file
	tf = fopen("/tmp/svg2png_tmpfile","w");
	
	// check for an error
	if (NULL == tf) {
		perror("/tmp/svg2png_tmpfile");
		exit(1);
	}
	
	// output the altered file
	xmlDocDump(tf,ND);

	// close the temp file
	fclose(tf);
}

char* out_file_string(argo opts, int n) {
	
	// If the is no output filename set, set the output to the same as the input
		char *kanji;
		kanji = malloc(FILENAME_MAX);

		if (opts->out_file != NULL) {
			strncpy (kanji, opts->out_file, strlen(opts->out_file) - 4);
			
		} else {
		
		// copy everything except the last 4 chars.
		strncpy (kanji, opts->svg_file, strlen(opts->svg_file) - 4);
		}

		// ensure the string is terminated
		kanji[strlen(opts->svg_file) - 4] = '\0';

	if (!opts->sequential_images) {

		// Append the outputformat extention
		strncat (kanji, "_out", 4);

		// Appent the file extention / format identifier
		strncat (kanji, opts->out_format, 4);
		
		// return the address to the memory
		return kanji;
	} else {

		// make the final completed kanji file name different
		if (n == 0) { 
			sprintf(kanji, "%s finished%s", kanji, opts->out_format); 
			return kanji; 
		}

		// create a numbered file name
		sprintf(kanji, "%s stroke %d_%s", kanji, n, opts->out_format);

		return kanji;
	}
	
	return NULL;
}

void push_out_image(xmlDocPtr ND, argo opts, int n, char *kanji) {

	// Output file name
	char *filename;

	//File name handles
	FILE *output_format, *svg;
	
	// Dump the tmp file
	dump_tmp(ND);

	if (n > 0) {

		// get the output file name for a multiple image output run
		filename = out_file_string (opts,n);
	} else {

		//set the file name to that chosen
		filename = opts->out_file;
	}

	// open file for writing
	output_format = fopen(filename,"w");
	
	// check for an error opening the file
	if (output_format == NULL) {
		perror(filename);
		exit(1);
	}

	// open the temp file for conversion to png
	svg = fopen("/tmp/svg2png_tmpfile","r");

	// check for an error opening the file
	if (svg == NULL) {
		perror("/tmp/svg2png_tmpfile");
		exit(1);
	}

	// Pointer to function
	svg_cairo_status_t (*render_functptr)(FILE*,FILE*,double,int,int) = NULL;
	
	// point the render function to the correct one
	render_functptr = get_render_function(opts->out_format);
	if (NULL == render_functptr) {
		perror(" Error assigning the render pointer");
		return;
	}
	
	// render the output file
	render_functptr(svg, output_format, 1.0, opts->width, opts->height);

	// close all files
	fclose(svg);
	fclose(output_format);
	
	// delete the temp file
	unlink("/tmp/svg2png_tmpfile");
}

/**************************************************************/
/*** Change the fill colour of an element of a desired type ***/
/**************************************************************/
void change_fill_colour(xmlNode *a_node, char *node_type, char *colour) {

	// create a node to work from
	xmlNode *cur_node = NULL;

	// step through the nodes in the document
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

		// check we are looking at an element
        if (cur_node->type == XML_ELEMENT_NODE) {

		// check for the desired node type
			if (!strcmp((char*)cur_node->name,node_type)) {
				
				// remove any property called "fill"
				xmlUnsetProp(cur_node,(unsigned char*)"fill");
				
				// set a fill colour for the node
				xmlNewProp(cur_node, BAD_CAST "fill", BAD_CAST colour);
			}
		}

		// Recurrsively reset the colour in all nodes for all elements of type node_type
        change_fill_colour(cur_node->children,node_type, colour);
    }	
}
