/*
 * Source file:
 *
 *        swlist.c  - switch-list functions
 *
 * Description:
 *
 *        Implementation of "swlist control" and "swlist info"
 *
 * Contents:
 *
 *   Public:
 *
 *        SwListQueryScrid     Get screen-session id for (sub)title
 *        SwListQueryHandle    Get switchlist-handle for (sub)title in the
 *                             specified screen-session
 *        SwListActivate       Make session in switch-list active (focus)
 *        SwListRemove         Remove indicated task from windowlist
 *        SwListVisibility     Hide/show session in switch-list and desktop
 *
 * Remarks:
 *
 * Author:         J. van Wijk
 *
 * Date     By     Comment
 * ======== ====== ============================================================
 * 07-04-93 JvW    Initial version
 * 22-06-98 JvW    Sample version, for demonstration purposes
 *
 *                 Note: probably not directly compilable
 *                       Not tested in this stripped-down version,
 *                       original code in production since 1993
 */

/*===== Include section =====================================================*/

#include <string.h>                             // C string functions
#include <stdio.h>                              // Std Input/Output
#include <stdlib.h>                             // Std functions

#define INCL_BASE                               // base definitions
#define INCL_WINWINDOWMGR                       // PM window functions
#define INCL_WINSWITCHLIST                      // PM switchlist functions
#include <os2.h>                                // OS/2 master include

#include "swlist.h"                             // swlist defs

/*===== Local constants =====================================================*/

/*===== Local type definitions ==============================================*/

/*===== Module globale variabelen ===========================================*/

/*===== Local function definitions ==========================================*/

// query switch list, retry until nr of entries stable
static USHORT SwListQueryList                   // RET   nr of entries
(
    HAB                hab,                     // IN    PM anchor
    PSWBLOCK          *list                     // OUT   switch list structure
);

//
/**************************************************************************
 *
 *  Name       : SwListQueryScrid
 *
 *  Description: screen-session id for specified (sub) title
 *
 *  API's      : WinQuerySwitchList
 *
 *  Return     : returncode
 *
 *************************************************************************/
ULONG SwListQueryScrid                          // RET   screen-session nr
(
    HAB                hab,                     // IN    PM anchor
    char              *title                    // IN    (sub) title
)
{
    ULONG              scrid = 0;               // default returnvalue
    USHORT             i;                       // counter in swlist
    USHORT             count;                   // nr of entries
    PSWBLOCK           ppswblk;                 // ptr to structure

    count = SwListQueryList(hab, &ppswblk);
    if (count > 0)
    {
        for ( i = 0;
             (i < count) && (scrid == 0);       // all entries or until found
              i++)
        {
            if (strstr( ppswblk->aswentry[i].swctl.szSwtitle,
                        title) != NULL)         // found (sub)title in list
            {
                scrid = ppswblk->aswentry[i].swctl.idSession;
            }
        }
        free( ppswblk);
    }
    return(scrid);
}                                               // end SwListQueryScrid
/*---------------------------------------------------------------------------*/

//
/**************************************************************************
 *
 *  Name       : SwListQueryHandle
 *
 *  Description: query switch handle for (sub) title in screen-session
 *
 *  API's      : WinQuerySwitchList
 *
 *  Return     : Switch handle
 *
 *************************************************************************/
HSWITCH SwListQueryHandle                       // RET   switch handle
(
    HAB                hab,                     // IN    PM anchor
    char              *title,                   // IN    (sub) title
    ULONG              scrid                    // IN    screen-session nr
)
{
    HSWITCH            hswi = NULLHANDLE;       // default return value
    USHORT             i;                       // counter in swlist
    USHORT             count;                   // nr of entries
    PSWBLOCK           ppswblk;                 // ptr to structure

    count = SwListQueryList(hab, &ppswblk);
    if (count > 0)
    {
        for ( i = 0;
             (i < count) &&                     // all entries or
             (hswi == NULLHANDLE);              // until found
              i++)
        {
            if ((scrid == 0) ||                 // not specified or match
                (scrid == ppswblk->aswentry[i].swctl.idSession))
            {
                if (strstr( ppswblk->aswentry[i].swctl.szSwtitle,
                            title) != NULL)     // found (sub)title in list
                {
                    hswi = ppswblk->aswentry[i].hswitch;
                }
            }
        }
        free( ppswblk);
    }
    return(hswi);
}                                               // end SwListQueryHandle
/*---------------------------------------------------------------------------*/

//
/**************************************************************************
 *
 *  Name       : SwListActivate
 *
 *  Description: activate indicated emulator
 *
 *  API's      : WinSwitchToProgram
 *
 *  Return     : BOOL result
 *
 *************************************************************************/
BOOL SwListActivate                             // RET   success
(
    HSWITCH            hswi                     // IN    swlist handle
)
{
    BOOL               rc = FALSE;              // return code

    if (hswi != NULLHANDLE)
    {
        if (WinSwitchToProgram(hswi) == NO_ERROR)
        {
            rc = TRUE;
        }
    }
    return(rc);
}                                               // end SwListActivate
/*---------------------------------------------------------------------------*/

//
/**************************************************************************
 *
 *  Name       : SwListRemove
 *
 *  Description: remove indicated task from windowlist
 *
 *  API's      : WinRemoveSwitchEntry
 *
 *  Return     : BOOL result
 *
 *************************************************************************/
BOOL SwListRemove                               // RET   success
(
    HSWITCH            hswi                     // IN    swlist handle
)
{
    BOOL               rc = FALSE;              // return code

    if (hswi != NULLHANDLE)
    {
        if (WinRemoveSwitchEntry(hswi) == NO_ERROR)
        {
            rc = TRUE;
        }
    }
    return(rc);
}                                               // end SwListRemove
/*---------------------------------------------------------------------------*/

//
/**************************************************************************
 *
 *  Name       : SwListVisibility
 *
 *  Description: Make a task (in)vissible in windowlist and desktop
 *
 *  API's      : WinQuerySwitchEntry
 *               WinChangeSwitchEntry
 *               WinSetWindowPos
 *
 *  Return     : BOOL result
 *
 *************************************************************************/
BOOL SwListVisibility                           // RET   success
(
    HSWITCH            hswi,                    // IN    swlist handle
    ULONG              swlv,                    // IN    swlist visibility
    ULONG              winv                     // IN    window visibility
)
{
    BOOL               rc = TRUE;               // return code
    SWCNTRL            swc;                     // switchlist control

    if (hswi != NULLHANDLE)                     // 1.02: rc TRUE if NULLHANDLE
    {
        if (WinQuerySwitchEntry(hswi, &swc) == NO_ERROR)
        {
            if (!WinSetWindowPos( swc.hwnd, NULLHANDLE, 0,0,0,0, winv))
            {
                rc = FALSE;
            }
            swc.uchVisibility = swlv;
            if (WinChangeSwitchEntry(hswi, &swc) != NO_ERROR)
            {
                rc = FALSE;
            }
        }
        else
        {
            rc = FALSE;
        }
    }
    return(rc);
}                                               // end SwListVisibility
/*---------------------------------------------------------------------------*/

/**************************************************************************
 *
 *  Name       : SwListQueryList
 *
 *  Description: query switch list, retry until nr of entries stable
 *
 *  API's      : WinQuerySwitchList
 *
 *  Return     : nr of entries, switchlist
 *
 *************************************************************************/
static USHORT SwListQueryList                   // RET   nr of entries
(
    HAB                hab,                     // IN    PM anchor
    PSWBLOCK          *list                     // OUT   switch list structure
)
{
    USHORT             count = 0;               // nr of entries 1st API call
    USHORT             cnt_2 = 0;               // nr of entries 2nd API call
    PSWBLOCK           ppswblk = NULL;          // ptr to structure
    size_t             usSWBSize;               // size of structure

    do
    {
        if (ppswblk != NULL)
        {
            free( ppswblk);
        }
        count = WinQuerySwitchList(hab, NULL, 0);
        if (count > 0)
        {
            usSWBSize = (count * sizeof(SWBLOCK));
            if ((ppswblk = (SWBLOCK*) malloc( usSWBSize)) != NULL)
            {
                cnt_2 = WinQuerySwitchList(hab, ppswblk, usSWBSize);
            }
        }
    } while (count != cnt_2);
    *list = ppswblk;                            // assign output
    return(count);
}                                               // end SwListQueryList
/*---------------------------------------------------------------------------*/

