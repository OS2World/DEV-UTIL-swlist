#ifndef SWLIST_H
#define SWLIST_H
/*
 * Include file:
 *
 *        SWLIST.H  - swlist functions
 *
 * Description:
 *
 *        External definitions swlist functions
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
 */

/*===== constants ===========================================================*/

/*===== macro's =============================================================*/

/*===== type definitions ====================================================*/

/*===== function definitions ================================================*/


    //- ALN query screen-session id for specified (sub) title
ULONG SwListQueryScrid                          // RET   screen-session nr
(
    HAB                hab,                     // IN    PM anchor
    char              *title                    // IN    (sub) title
);

    //- query switch handle for (sub) title in screen-session
HSWITCH SwListQueryHandle                       // RET   switch handle
(
    HAB                hab,                     // IN    PM anchor
    char              *title,                   // IN    (sub) title
    ULONG              scrid                    // IN    screen-session nr
);

    //- activate indicated emulator
BOOL SwListActivate                             // RET   success
(
    HSWITCH            hswi                     // IN    swlist handle
);

    //- remove indicated task from windowlist
BOOL SwListRemove                               // RET   success
(
    HSWITCH            hswi                     // IN    swlist handle
);

    //- Make a task (in)vissible in windowlist and desktop
BOOL SwListVisibility                           // RET   success
(
    HSWITCH            hswi,                    // IN    swlist handle
    ULONG              swlv,                    // IN    swlist visibility
    ULONG              winv                     // IN    window visibility
);

#endif
