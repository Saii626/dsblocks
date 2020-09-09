#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "util.h"

#define NANO 1000000000L

void cspawn(char *const *arg) {
	execv(arg[0], arg);
	perror("cspawn - execv");
	_exit(127);
}

void csigself(int signal, int sigval) {
	union sigval sv;

	signal += SIGRTMIN;
	sv.sival_int = sigval;
	if (sigqueue(pid, signal, sv) == -1) {
		perror("csigdsblocks - sigqueue");
		exit(1);
	}
}

/* getcmdout doesn't null terminate */
ssize_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen) {
	int fd[2];
	ssize_t rd;

	if (pipe(fd) == -1) {
		perror("getcmdout - pipe");
		exit(1);
	}
	switch (fork()) {
		case -1:
			perror("getcmdout - fork");
			exit(1);
		case 0:
			close(ConnectionNumber(dpy));
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
				perror("getcmdout - child - dup2");
				exit(1);
			}
			close(fd[1]);
			execv(arg[0], arg);
			perror("getcmdout - child - execv");
			_exit(127);
		default:
			close(fd[1]);
			rd = read(fd[0], cmdout, cmdoutlen);
			if (rd == -1) {
				perror("getcmdout - read");
				exit(1);
			}
			close(fd[0]);
	}
	return rd;
}

int readint(const char *path, int *var) {
	FILE *fp;

	if (!(fp = fopen(path, "r")))
		return 0;
	if (fscanf(fp, "%d", var) != 1) {
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}

void uspawn(char *const *arg) {
	if (fork() == 0) {
		close(ConnectionNumber(dpy));
		setsid();
		execv(arg[0], arg);
		perror("uspawn - child - execv");
		_exit(127);
	}
}
/* calculates the diff and returns true if result is > 0 */
inline bool timespec_diff(time_spec_t* a, time_spec_t* b, time_spec_t* result) {
	result->tv_sec = a->tv_sec - b->tv_sec;
	result->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (result->tv_nsec < 0) {
		--result->tv_sec;
		result->tv_nsec += NANO;
	}
	return result->tv_sec > 0;
}

inline int timespec_cmp(time_spec_t* a, time_spec_t* b) {
	if (a->tv_sec > b->tv_sec) return 1;
	if (a->tv_sec < b->tv_sec) return -1;

	if (a->tv_nsec > b->tv_nsec) return 1;
	if (a->tv_nsec < b->tv_nsec) return -1;

	return 0;
}

inline void timespec_add(time_spec_t* a, time_spec_t* b, time_spec_t* result) {
	result->tv_sec = a->tv_sec + b->tv_sec;
	result->tv_nsec = a->tv_nsec + b->tv_nsec;
	if (result->tv_nsec > NANO) {
		++result->tv_sec;
		result->tv_nsec -= NANO;
	}
}

// buf needs to store 30 characters
int timespec2str(char *buf, int len, time_spec_t *ts) {
    int ret;
    struct tm t;

    tzset();
    if (localtime_r(&(ts->tv_sec), &t) == NULL)
        return 1;

    ret = strftime(buf, len, "%F %T", &t);
    if (ret == 0)
        return 2;
    len -= ret - 1;

    ret = snprintf(&buf[strlen(buf)], len, ".%03ld", ts->tv_nsec);
    if (ret >= len)
        return 3;

    return 0;
}

