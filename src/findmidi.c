/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: findmidi.c,v 1.1 2001/01/17 18:51:30 ksvalast Exp $

    Desc: 
    Lang: English
*/

#include <proto/exec.h>

#include "camd_intern.h"

/*****************************************************************************

    NAME */

	AROS_LH1(struct MidiNode *, FindMidi,

/*  SYNOPSIS */
	AROS_LHA(STRPTR, name, A1),

/*  LOCATION */
	struct CamdBase *, CamdBase, 12, Camd)

/*  FUNCTION
		Finds the midinode with name 'name'.

    INPUTS
		name - Name of midinode to find.

    RESULT
		NULL if no midinode with that name or a pointer to the midinode if success.

    NOTES
		CL_Linkages must be locked.

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

	return (struct MidiNode *)FindName(&CB(CamdBase)->mymidinodes,name);

   AROS_LIBFUNC_EXIT
}
