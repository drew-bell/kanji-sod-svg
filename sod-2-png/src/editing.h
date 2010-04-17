#include <libxml/tree.h>
#import "arg.h"

/*  xml editing header file */

void print_element_names(xmlNode* a_node);
void process_xml_options(xmlNode* node, argo *selected_opts);