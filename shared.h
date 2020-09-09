#include <limits.h>
#include <X11/Xlib.h>

#define NILL                            INT_MIN
#define CMDLENGTH                       25

#ifdef DEBUG
	#define debug(x) x
#else
	#define debug(x)
#endif


extern Display *dpy;
extern pid_t pid;
