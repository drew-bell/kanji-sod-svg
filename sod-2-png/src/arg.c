#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "arg.h"
#include "types.h"
#include <limits.h>
#include "editing.h"

static const char PROGRAM_VERSION[] = "0.1.0";

char* rewrite_format_string(char* string) {
	return &string[1];
}

void cleanup_o (argo o) {
	if (NULL != o->out_format) free (o->out_format);
	if (NULL != o->svg_file) free (o->svg_file);
	if (NULL != o->out_file) free (o->out_file);
	if (NULL != o) free (o);
}

void null_options (argo o) {
	// Null out all the options
	o->no_arrows = false;
	o->no_numbers = false;
	o->no_Start_mark = false;
	o->sequential_images = false;
	o->width = -1;
	o->height = -1;
	o->num_of_files = 0;
	o->out_format = NULL;
	o->svg_file = NULL;
	o->out_file = NULL;
}

void help (const char *argv0) {
	
	char *argv0_copy = strdup (argv0);
    char *argv0_base = basename (argv0_copy);
	
	fprintf (stderr, "Usage: %s [OPTIONS] <SVG_file> <out_file>\n", argv0_base);
	puts ("");
	fprintf (stderr, "  -w, --width=WIDTH\tWidth of output image in pixels.\n");
	fprintf (stderr, "  -h, --height=HEIGHT\tHeight of output image in pixels.\n");
	fprintf (stderr, "  -m, --sequence\tSequential image.\n");
	fprintf (stderr, "  -o, --format=FORMAT\tThe desired output file format , svg or png [png is default].\n");
	fprintf (stderr, "\t Available formats.\n");
	fprintf (stderr, "\t png : Portable Network Graphics [default]\n");
	fprintf (stderr, "\t svg : Scalable Vector Graphics\n");
	puts ("");
	fprintf (stderr, "  -a, --no-arrows\tRemove arrows from the output image.\n");
	fprintf (stderr, "  -n, --no-numbers\tRemove numbers from the output.\n");
	fprintf (stderr, "  -s, --no-startmark\tRemove start marks from the output.\n");
	puts ("");
	fprintf (stderr, "  -?, --help\t\tGive this help.\n");
	fprintf (stderr, "  -V, --version\t\tProgram Version.\n");
	free (argv0_copy);
	exit(0);
}

void process_args (char **argv, int argc, argo opts) {
	
	int c;
	
	// the long options for the program
	static struct option long_options[] = {

		/* These options set a bool flag. */
		{"version", no_argument, NULL, 'V'}, 
		{"no-arrows", no_argument, NULL, 'a'}, 
		{"no-numbers", no_argument, NULL, 'n'}, 
		{"no-startmark", no_argument, NULL, 's'},  
		{"sequence", no_argument, NULL, 'm'}, 
		{"help", no_argument, NULL, '?'}, 

		/* THese options have an argument requirement */
		{"width", required_argument, NULL, 'w'}, 
		{"format", required_argument, NULL, 'i'}, 
		{"height", required_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	// parse the options and set the values in the options structure
	while ( (c = getopt_long (argc, argv, "V?no:amsw:h:", long_options, NULL)) != -1)
		switch (c) {
			case 'w':
				// Add the argument for the -w tag as an integer to the struct
				opts->width = atoi (optarg);
				break;
			case 'h':
				// Add the argument for the -h tag as an integer to the struct
				opts->height = atoi (optarg);
				break;
			case 'm':
				// Output the strokes in individual images collectively
				opts->sequential_images = true;
				break;
			case 'n':
				// Set the remove numbers option
				opts->no_numbers = true;
				break;
			case 'o':
				// Set the output format.

				// if the agrument provided is not 3 chars in length, exit.
				if (strlen(optarg) != 3) {
					fprintf(stderr,"Argument for option -o incorrect: too long or too short.\n");
					help(argv[0]);
				}

				// Add a '.' to the format string to make processing easier
				char f[5];
				sprintf(f, ".%s",optarg);
				f[4]='\0';

				// Allocate memory for the format string
				opts->out_format = (char*)malloc (5);

				// copy the format string to the allocated memory
				strncpy (opts->out_format, f, 5);

				// ensure the last char is a '\0' for later string processing
				opts->out_format[4] = '\0';

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
				fprintf (stderr, "Version v%s\n", PROGRAM_VERSION);
				exit (0);
				break;
			case '?':
				// Output the help information
				help (argv[0]);
				break;
			default:
				break;
		}
	
	// Add the input file to the args structure
	if (argc - optind >= 1) {

		// Allocate memory for the file name
		opts->svg_file = (char*)malloc (FILENAME_MAX);
		
		// Copy the file name argument to the allocated memory
		strncpy (opts->svg_file, argv[optind++], FILENAME_MAX);
		
		// check for another filename argument
		if (argc - optind >= 1) {

			// Add the output file to the args structure
			// Allocate memory for the file name
			opts->out_file = (char*)malloc (FILENAME_MAX);

			// Copy the file name argument to the allocated memory
			strncpy (opts->out_file, argv[optind++], FILENAME_MAX);

			// check for another filename
			if (argc - optind > 0) {

				// Print out the options info and quit
				help (argv[0]);
			}
		}
	}

	// Check to make sure we have a file to process and that it is a valid file 
	if ( NULL == opts->svg_file || !file_exists (opts->svg_file) || !is_type (opts->svg_file, ".svg")) {

		// Print error and quit
		printf ("No .svg file to process. Quitting...\n");
		
		// dealloc any memory before quitting
		cleanup_o (opts);
		exit (0);
		}

		// Check for output format
		if (NULL == opts->out_format) {

			// check for a filename to set the format from
			if (NULL == opts->out_file) {

				printf ("Setting default output format : PNG.\n");

				// malloc the number of byes in the extention +1 for the format var
				opts->out_format = (char*)malloc (5);
			
				// assign the default file type (png)
				strncpy (opts->out_format, ".png", 5);

				// get the file name
				opts->out_file = out_file_string(opts,0);

				printf ("Setting default file name : %s.\n",opts->out_file); 

			} else {
				if (!available_formats(ext(opts->out_file))) {
					// Write out error
					printf ("Format not found.\n");

					// output the usage help
					help(argv[0]);

					// free any allocated memory and exit
					cleanup_o (opts);
					exit (0);
					
				}

				// malloc the number of byes in the extention +1 for the format var
				opts->out_format = (char*)malloc (strlen (strrchr (opts->out_file, '.'))+1);

				// assign the ext to the format var
				strncpy (opts->out_format, ext (opts->out_file), strlen (strrchr (opts->out_file, '.'))+1);	

			}
		} else if (!available_formats(opts->out_format)) {

				// Write out error
				printf ("Format not found.\n");

				// output the usage help
				help(argv[0]);

				// free any allocated memory and exit
				cleanup_o (opts);
				exit (0);
			}

	if (NULL == opts->out_file) {
		out_file_string(opts,0);
	}
}
