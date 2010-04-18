#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "arg.h"

static const char PROGRAM_VERSION[] = "0.0.1";

void null_options(argo *opts) {
	// Null out all the options
	opts->no_arrows = false;
	opts->no_numbers = false;
	opts->no_Start_mark = false;
	opts->sequential_images = false;
	opts->width = -1;
	opts->height = -1;
	opts->num_of_files = 0;
	opts->svg_file = malloc(sizeof(char*));
	opts->out_file = malloc(sizeof(char*));
	opts->svg_file = NULL;
	opts->out_file = NULL;
}

void help(const char *argv0) {
	
	char *argv0_copy = strdup (argv0);
    char *argv0_base = basename (argv0_copy);
	
	fprintf(stderr,"Usage: %s [OPTIONS] <SVG_file> <out_file>\n",argv0_base);
	puts ("");
	fprintf(stderr,"  -w, --width=WIDTH\tWidth of output image in pixels\n");
	fprintf(stderr,"  -h, --height=HEIGHT\tHeight of output image in pixels\n");
	fprintf(stderr,"  -m, --sequence\tSequential image.\n");
	puts ("");
	fprintf(stderr,"  -a, --no-arrows\tRemove arrows from the output image.\n");
	fprintf(stderr,"  -n, --no-numbers\tRemove numbers from the output.\n");
	fprintf(stderr,"  -s, --no-startmark\tRemove start marks from the output.\n");
	puts ("");
	fprintf(stderr,"  -?, --help\t\tGive this help.\n");
	fprintf(stderr,"  -V, --version\t\tProgram Version.\n");
	free (argv0_copy);
}

void process_args(char **argv,int argc, argo *opts) {
	
	int c;
	
	// the long options for the program
	static struct option long_options[] = {
		/* These options don't set a flag.
		 We distinguish them by their indices.  */
		{"version", no_argument, NULL, 'V'},
		{"no-arrows", no_argument, NULL, 'a'},
		{"no-numbers", no_argument, NULL, 'n'},
		{"no-startmark", no_argument, NULL, 's'},
		{"width", required_argument, NULL, 'w'},
		{"height", required_argument, NULL, 'h'},
		{"sequence", no_argument, NULL, 'm'},
		{"help", no_argument, NULL, '?'},
		{NULL, 0, NULL, 0}
	};
	
	// parse the options and set the values in the options structure
	while ((c = getopt_long(argc, argv, "V?namsw:h:", long_options, NULL)) != -1)
		switch (c) {
			case 'w':
			// Add the argument for the -w tag as an integer to the struct
				opts->width = atoi(optarg);
				break;
			case 'h':
			// Add the argument for the -h tag as an integer to the struct
				opts->height = atoi(optarg);
				break;
			case 'm':
			// Output the strokes in individual images collectively
				opts->sequential_images = true;
				break;
			case 'n':
			// Set the remove numbers option
				opts->no_numbers = true;
				break;
			case 's':
			// Set the remove start mark option
				opts->no_Start_mark = true;
				break;
			case 'a':
			// Set the remove arrows option
				opts->no_arrows = true;
				break;
			case 'V':
			// Output the version of the program option
				fprintf(stderr,"Version v%s\n",PROGRAM_VERSION);
				exit(0);
				break;
			case '?':
			// Output the help information
				help(argv[0]);
				exit(1);
				break;
			default:
				break;
		}

	// Add the input file to the args structure
    if (argc - optind >= 1) {
		opts->svg_file = argv[optind++];
		if (argc - optind >= 1) {
			// Add the output file to the args structure
			opts->out_file = argv[optind++];
			if (argc - optind > 0) {
				help (argv[0]);
				exit (1);
    	    }
		}
    }

// Print some debug info
	if (opts->sequential_images) fprintf(stderr,"opts->sequential_images\n");
	if (opts->width > 0) fprintf(stderr,"opts->width = %i\n",opts->width);
	if (opts->height > 0) fprintf(stderr,"opts->height = %i\n",opts->height);
	if (opts->svg_file != NULL) fprintf(stderr,"opts->svg_file = %s\n",opts->svg_file);
	if (opts->out_file != NULL) printf("opts->out_file = %s\n",opts->out_file);

}
