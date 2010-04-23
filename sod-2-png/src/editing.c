#include <stdio.h>
#include "editing.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Prototypes
static void remove_part(xmlNode* a_node, char *node_type);

// Functions
void remove_part(xmlNode* a_node, char *node_type) {
    xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp((char*)cur_node->name,node_type)) {
				xmlUnlinkNode(cur_node);
			}
		}
        remove_part(cur_node->children,node_type);
    }
}

void print_element_names(xmlNode* a_node) {
    xmlNode *cur_node = NULL;
	
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }
		
        print_element_names(cur_node->children);
    }
}

//void process_xml_options(xmlNode* node, argo *selected_opts) {
void process_xml_options(xmlDocPtr doc, argo selected_opts) {

	// A pointer to the root element of the file
	xmlNode *root_element = NULL;

	// Get the root element
	root_element = xmlDocGetRootElement(doc);
		
	if (selected_opts->sequential_images) {
		create_sequential_images(root_element, selected_opts);
		
		// later removing all 
		selected_opts->no_arrows = true;
		selected_opts->no_numbers = true;
		selected_opts->no_Start_mark = true;
	}
	if (selected_opts->no_arrows) {
		remove_part(root_element,"polyline");
	}
		
	if (selected_opts->no_numbers) {
		remove_part(root_element,"text");
	}
	
	if (selected_opts->no_Start_mark) {
		remove_part(root_element,"circle");
	}

}

void setup_basic_doc(xmlDocPtr ND) {
	
	// Node pointers for the new doc
	xmlNodePtr ND_root_node = NULL;

	// DTD pointer
	xmlDtdPtr dtd = NULL;

	// Create the root node
	ND_root_node = xmlNewNode(NULL, BAD_CAST "svg");

	// Set Attributes for the node
	xmlNewProp(ND_root_node, BAD_CAST "version", BAD_CAST "1.1");
	xmlNewProp(ND_root_node, BAD_CAST "id", BAD_CAST "Layer_1");
	xmlNewProp(ND_root_node, BAD_CAST "xmlns", BAD_CAST "http://www.w3.org/2000/svg");
	xmlNewProp(ND_root_node, BAD_CAST "xmlns:xlink", BAD_CAST "http://www.w3.org/1999/xlink");
	xmlNewProp(ND_root_node, BAD_CAST "x", BAD_CAST "0px");
	xmlNewProp(ND_root_node, BAD_CAST "y", BAD_CAST "0px");
	xmlNewProp(ND_root_node, BAD_CAST "width", BAD_CAST "1000px");
	xmlNewProp(ND_root_node, BAD_CAST "height", BAD_CAST "1000px");
	xmlNewProp(ND_root_node, BAD_CAST "viewBox", BAD_CAST "0 0 1000 1000");
	xmlNewProp(ND_root_node, BAD_CAST "enable-background", BAD_CAST "new 0 0 1000 1000");
	xmlNewProp(ND_root_node, BAD_CAST "xml:space", BAD_CAST "preserve");
	
	// Set the node to the documents root
	xmlDocSetRootElement(ND, ND_root_node);
	
	// Add the <!DOCTYPE info>
	dtd = xmlCreateIntSubset(ND, BAD_CAST "svg", BAD_CAST "-//W3C//DTD SVG 1.1//EN", BAD_CAST "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd");	
}


void create_sequential_images(xmlNode *OD_root_node, argo opts) {

	int number = 0;

	// make the output file pretty
	xmlIndentTreeOutput = 1;

	// Document pointer to the memory document to be built
	xmlDocPtr ND = NULL;

	// Node pointers for the new doc
	xmlNodePtr ND_root_node = NULL, node_copy = NULL;

	// Create document and a node
	ND = xmlNewDoc(BAD_CAST "1.0");

	// Setup the basic output document
	setup_basic_doc(ND);

	// Get the root node for the new document
	ND_root_node = xmlDocGetRootElement(ND);
	
	// Start parsing the original file
	xmlNode *cur_node = NULL;
	cur_node = OD_root_node->children;
	cur_node = cur_node->next;

	for (cur_node = cur_node->next; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

			// Remove all the arrows, text and startmarks in the new doc to this point
			remove_part(ND_root_node,"polyline");
			remove_part(ND_root_node,"text");
			remove_part(ND_root_node,"circle");
			change_fill_colour(ND_root_node,"path");

			// Copy the current node
			node_copy = xmlCopyNode(cur_node, 1);
			
			// add the copy to the New Document
			xmlAddChild(ND_root_node, node_copy);
			
			// Make an image
			push_out_image(ND,opts,number);

			// increase the filename number
			number++;
		}
    }
}

void push_out_image(xmlDocPtr ND, argo opts, int number) {

	// Output file name
	char filename[FILENAME_MAX];

	//File name handles
	FILE *output_format, *temp_file, *svg;
	
	// Create a tmp intermediary file
	temp_file = fopen("/tmp/svg2png_tmpfile","w");
	
	// output the altered file
	xmlDocDump(temp_file,ND);

	// close the temp file
	fclose(temp_file);

	// Create the output image name
	sprintf(filename, "image_%d_.png", number);

	// open file for writing
	output_format = fopen(filename,"w");
	
	// open the temp file for conversion to png
	svg = fopen("/tmp/svg2png_tmpfile","r");
	
	// Pointer to function
	svg_cairo_status_t (*render_functptr)(FILE*,FILE*,double,int,int) = NULL;
	
	// point the render function to the correct one
	render_functptr = get_render_function(filename);
	
	// render the output file
	render_functptr(svg, output_format, 1.0, opts->width, opts->height);

	// close all files
	fclose(svg);
	fclose(output_format);
	
	// delete the temp file
	unlink("/tmp/svg2png_tmpfile");
}

void change_fill_colour(xmlNode *a_node, char *node_type) {
	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp((char*)cur_node->name,node_type)) {
				
				// remove any property called "fill"
				xmlUnsetProp(cur_node,(unsigned char*)"fill");
				
				// set a fill colour for the node
				xmlNewProp(cur_node, BAD_CAST "fill", BAD_CAST "#333333");
			}
		}
        change_fill_colour(cur_node->children,node_type);
    }	
}
