#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void	pr_times(clock_t, struct tms *, struct tms *);
static void	do_cmd(char *);

int
main(int argc, char *argv[])
{
	do_cmd(argv[1]);	/* once for each command-line arg */
}

static 
void do_cmd(char *cmd)		/* execute and time the "cmd" */
{
	struct tms	tmsstart, tmsend;
	clock_t		start, end;
	int			status;

	start = times(&tmsstart);
	status = system(cmd);
	end = times(&tmsend);

	pr_times(end-start, &tmsstart, &tmsend);
}

static void
pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
	static long		clktck = 0;

	if (clktck == 0)	/* fetch clock ticks per second first time */
		if ( (clktck = sysconf(_SC_CLK_TCK)) < 0)
			exit(1);
	
	fprintf(stderr, "  real:  %7.4f\n", real / (double) clktck);
	fprintf(stderr, "  user:  %7.4f\n",
			(tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
	fprintf(stderr, "  sys:   %7.4f\n",
			(tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
}