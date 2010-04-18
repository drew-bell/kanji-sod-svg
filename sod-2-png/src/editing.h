#include <libxml/tree.h>
#import "arg.h"

/*  xml editing header file */

void print_element_names(xmlNode* a_node);
void process_xml_options(xmlDocPtr doc, argo *selected_opts);
void change_fill_colour(xmlNode *a_node, char *node_type);
void setup_basic_doc(xmlDocPtr NewDoc);
void create_sequential_images(xmlNode *OD_root_node, argo *opts);
void setup_basic_doc(xmlDocPtr NewDoc);
void push_out_image(xmlDocPtr ND, argo *opts, int number);