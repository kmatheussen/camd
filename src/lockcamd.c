/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: lockcamd.c,v 1.2 2001/01/20 22:07:07 ksvalast Exp $

    Desc: 
    Lang: English
*/

#include <proto/exec.h>

#include "camd_intern.h"

/*****************************************************************************

    NAME */

	AROS_LH1(APTR, LockCAMD,

/*  SYNOPSIS */
	AROS_LHA(ULONG, locktype, D0),

/*  LOCATION */
	struct CamdBase *, CamdBase, 5, Camd)

/*  FUNCTION
		Locks the internal lists in camd.
		You must call UnlockCAMD later.

    INPUTS
		locktype - Only CD_Linkages is legal.

    RESULT
		APTR to send to UnlockCAMD

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY

	2001-01-12 ksvalast first created

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct CamdBase *,CamdBase)

	ObtainSemaphoreShared(CB(CamdBase)->CLSemaphore);
	return CB(CamdBase)->CLSemaphore;

   AROS_LIBFUNC_EXIT
}

