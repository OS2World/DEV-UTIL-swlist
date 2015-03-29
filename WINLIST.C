//
// List entries in the OS/2 switch-list structures (Window list)
//
// Author: J. van Wijk
//
#define WINLIST_N   "OS/2 Window / Switch-list dump"
#define WINLIST_C "(c) 1998; Jan van Wijk"
#define WINLIST_V   "1.01 27-07-98" // Minor update for freeware version
//efine WINLIST_V   "1.00 22-03-93" // Initial version

#include <malloc.h>
#include <string.h>
#include <stdio.h>

#define INCL_BASE
#define INCL_WIN
#define INCL_DOS
#include <os2.h>


void ListTaskListEntry(void);

/************************************************************************/
/*                                                                      */
/* Function : main: list                                                */
/*                                                                      */
/* Purpose  : This function list tasklist entries                       */
/*                                                                      */
/* Input    : none                                                      */
/*                                                                      */
/* Output   : none                                                      */
/*                                                                      */
/************************************************************************/
int main
(
    int                argc,
    char              *argv[]
)
{
   printf("\n  %s; %s  %s\n\n", WINLIST_N, WINLIST_V, WINLIST_C);

   ListTaskListEntry();

   return (0);
}

/************************************************************************/
/*                                                                      */
/* Function : ListTaskListEntry                                         */
/*                                                                      */
/* Purpose  : The task list (shown by Ctrl ESC) is listed               */
/*                                                                      */
/* Input    : none                                                      */
/*                                                                      */
/* Output   : none                                                      */
/*                                                                      */
/************************************************************************/
void ListTaskListEntry
(
    void
)
{
    FILE              *wlist;
    USHORT             i;
    size_t             usSWBSize;
    USHORT             count;
    PSWBLOCK           ppswblk;
    HAB                hab;

    if ((hab = WinInitialize( 0L)) != NULLHANDLE) /* start PM session        */
    {
        count = WinQuerySwitchList(hab, NULL, 0);
        if (count > 0)
        {
            printf( "Windowlist entries: %u\n\n", count);
            usSWBSize = (count * sizeof(SWBLOCK));
            if ((ppswblk = (SWBLOCK*) malloc( usSWBSize)) != NULL)
            {
                printf( "%4.4s %8.8s %8.8s %4.4s %3.3s %1.1s %1.1s %-s\n",
                        "hsw",
                        "hwnd",
                        "hwndIcon",
                        "PID",
                        "SID",
                        "V",
                        "J",
                        "szSwtitle"
                      );
                count = WinQuerySwitchList(hab, ppswblk, usSWBSize);
                for (i = 0; i < count; i++)
                {
                    printf( "%4.4X %8.8x %8.8x %4.4x %3.3x %1.1x %1.1x %-s\n",
                            ppswblk->aswentry[i].hswitch,
                            ppswblk->aswentry[i].swctl.hwnd,
                            ppswblk->aswentry[i].swctl.hwndIcon,
                            ppswblk->aswentry[i].swctl.idProcess,
                            ppswblk->aswentry[i].swctl.idSession,
                            ppswblk->aswentry[i].swctl.uchVisibility,
                            ppswblk->aswentry[i].swctl.fbJump,
                            ppswblk->aswentry[i].swctl.szSwtitle
                          );
                }
                free( ppswblk);
            }
        }
        WinTerminate(hab);
    }
}                                              // einde 'ListTaskListEntry'
/*---------------------------------------------------------------------------*/
