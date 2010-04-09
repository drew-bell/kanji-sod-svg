#include <stdio.h>
#include "editing.h"

// Prototypes
static void remove_part(xmlNode* a_node, char *node_type);

// Functions
void remove_part(xmlNode* a_node, char *node_type) {
    xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp(cur_node->name,node_type)) {
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

void process_xml_options(xmlNode* node, argo *selected_opts) {
	
	if (selected_opts->no_arrows) {
		remove_part(node,"polyline");
	}
		
	if (selected_opts->no_numbers) {
		remove_part(node,"text");
	}
	
	if (selected_opts->no_Start_mark) {
		remove_part(node,"circle");
	}

}
