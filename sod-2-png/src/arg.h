#include <stdbool.h>

typedef struct {
	bool no_arrows;
	bool no_numbers;
	bool no_Start_mark;
	bool sequential_images;
	int width;
	int height;
	int num_of_files;
	char *svg_file;
	char *out_file;
} options;

typedef options *argo;

void process_args(char **argv,int argc, argo opts);
void null_options(argo opts);