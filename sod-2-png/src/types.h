#include <svg-cairo.h>


int is_type(char *file,char *type);
char* ext(char *file);
svg_cairo_status_t (*get_render_function(char *file))(FILE*,FILE*,double,int,int);
int file_exists(char *file);
int available_formats(char* format);
