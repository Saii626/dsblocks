#include <stdbool.h>
#include "shared.h"

#define COL0                            "" /* default status color */
#define COL1                            "" /* default icon color */
#define COL2                            "\r" /* warning icon color */
#define COL3                            "" /* mail block - frozen */
#define COL4                            "" /* mail block - MAILSYNC started */
#define COL5                            "" /* mail block - syncing */

#define TERMCMD(cmd)                    cspawn((char *[]){ "/usr/bin/termite", "-e", cmd, NULL })

typedef struct timespec time_spec_t;

void cspawn(char *const *arg);
void csigself(int signal, int sigval);
ssize_t getcmdout(char *const *arg, char *cmdout, size_t cmdoutlen);
int readint(const char *path, int *var);
void uspawn(char *const *arg);

bool timespec_diff(time_spec_t* a, time_spec_t* b, time_spec_t* result);
int timespec_cmp(time_spec_t* a, time_spec_t* b);
void timespec_add(time_spec_t* a, time_spec_t* b, time_spec_t* result);

int timespec2str(char *buf, int len, time_spec_t *ts);
