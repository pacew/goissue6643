#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/resource.h>

char *dir;
int vflag;

void
usage (void)
{
	fprintf (stderr, "usage: needed [-v] -C directory cmd\n");
	exit (1);
}

int
main (int argc, char **argv)
{
	int c;
	long l, u, m;
	char cmd[10000];
	long need;
	int i;
	char *p;
	
	while ((c = getopt (argc, argv, "vC:")) != EOF) {
		switch (c) {
		case 'C':
			dir = optarg;
			break;
		case 'v':
			vflag = 1;
			break;
		default:
			usage ();
		}
	}

	if (optind >= argc)
		usage ();

	if (dir == NULL)
		usage ();

	if (chdir (dir) < 0) {
		fprintf (stderr, "can't chdir to %s\n", dir);
		exit (1);
	}

	/* in mbytes */
	l = 1; 
	u = 4 * 1024;
	need = 0;
	while (l <= u) {
		m = (l + u) / 2;

		p = cmd;
		/* ulimit arg is in kbytes */
		p += sprintf (p, "go clean -cache; ulimit -d %ld;", m * 1024);
		for (i = optind; i < argc; i++) {
			p += sprintf (p, " %s", argv[i]);
		}
		p += sprintf (p, " > log 2>&1");

		if (vflag)
			fprintf (stderr, "%s ... ", cmd);

		if (system (cmd) == 0) {
			if (vflag)
				fprintf (stderr, "ok\n");
			need = m;
			u = m - 1;
		} else {
			if (vflag)
				fprintf (stderr, "fail\n");
			l = m + 1;
		}
	}

	if (need == 0) {
		printf ("err\n");
	} else {
		printf ("%ld\n", need);
	}
	return (0);
}
