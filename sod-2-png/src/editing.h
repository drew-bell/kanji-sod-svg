#include <libxml/tree.h>
#import "arg.h"

/*  xml editing header file */

void print_EL_names(xmlNode* a_node);
xmlDocPtr process_xml_options(xmlDocPtr doc, argo selected_opts);
void change_fill_colour(xmlNode *a_node, char *node_type, char *colour);
void setup_basic_doc(xmlDocPtr NewDoc);
void create_sequential_images(xmlDocPtr OD, argo opts);
void setup_basic_doc(xmlDocPtr NewDoc);
void push_out_image(xmlDocPtr ND, argo opts, int number, char *kanji);
void dump_tmp(xmlDocPtr ND);
char* out_file_string(argo opts, int n);