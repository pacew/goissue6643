#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sflag;

void
usage (void) {
	fprintf (stderr, "usage: mkprog [-s] filename kbytes\n");
	exit (1);
}

int
main (int argc, char **argv)
{
	int c;
	char *name;
	long kbytes;
	int i;
	FILE *outf;

	while ((c = getopt (argc, argv, "s")) != EOF) {
		switch (c) {
		case 's':
			sflag = 1;
			break;
		default:
			usage ();
		}
	}

	if (optind + 2 != argc)
		usage ();

	name = argv[optind++];

	kbytes = strtol (argv[optind++], NULL, 10);

	if ((outf = fopen (name, "w")) == NULL) {
		fprintf (stderr, "can't create %s\n", name);
		exit (1);
	}
	
	fprintf (outf, "package main\n");
	fprintf (outf, "import \"fmt\"\n");
	fprintf (outf, "func main () {\n");

	if (sflag) {
		fprintf (outf, "  hello := \"");
		for (i = 0; i < kbytes * 1024; i++)
			putc ('a' + (i % 26), outf);
		fprintf (outf, "\"\n");
	} else {
		fprintf (outf, "  hello := []byte{0");
		for (i = 0; i < kbytes * 1024; i++)
			fprintf (outf, ",%d", i % 10);
		fprintf (outf, "}\n");
	}
	
	fprintf (outf, "  fmt.Println (hello[0])\n");
	fprintf (outf, "}\n");

	return (0);
}
