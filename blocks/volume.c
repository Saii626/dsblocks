#include <stdio.h>

#include "../util.h"
#include "volume.h"

#define ICON0                           COL1 "" COL0
#define ICON1                           COL1 "'" COL0

#define BUFLENGTH                       10

#define PAMIXER                         (char *[]){ "/usr/bin/pulsemixer", "--get-mute", "--get-volume", NULL }

//#define PAVUCONTROL                     (char *[]){ "/usr/bin/pavucontrol-qt", NULL }
#define SETDEFAULTVOL                   (char *[]){ "/usr/bin/pulsemixer", "--set-volume", "50", NULL }
#define GETVOL                          (char *[]){ "/usr/bin/pulsemixer", "--get-volume", NULL }
#define TOGGLEMUTE                      (char *[]){ "/usr/bin/pulsemixer", "--toggle-mute", NULL }

void
volumeu(char *str, int sigval)
{
        char buf[BUFLENGTH];

        buf[getcmdout(PAMIXER, buf, BUFLENGTH) - 1] = '\0';
        if (buf[0] == '1') /* output was 1 */
                snprintf(str, CMDLENGTH, ICON0 " --- ");
        else {/* output was `0' */
		buf[getcmdout(GETVOL, buf, BUFLENGTH) - 1] = '\0';
                snprintf(str, CMDLENGTH, ICON1 "%s%%", buf);
	}
}

void
volumec(int button)
{
        switch(button) {
                case 1:
                        cspawn(TOGGLEMUTE);
                        break;
                case 2:
                        cspawn(SETDEFAULTVOL);
                        break;
        }
}
