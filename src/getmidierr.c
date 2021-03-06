/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: getmidierr.c,v 1.1 2001/01/17 18:51:30 ksvalast Exp $

    Desc: 
    Lang: English
*/

#include "camd_intern.h"



/*****************************************************************************

    NAME */

	AROS_LH1(UBYTE, GetMidiErr,

/*  SYNOPSIS */
	AROS_LHA(struct MidiNode *, midinode, A0),

/*  LOCATION */
	struct CamdBase *, CamdBase, 30, Camd)

/*  FUNCTION
		Gets the current error-state of a midinode.

    INPUTS
		midinode - pointer to midinode

    RESULT
		0 if everything was okey, not 0 else.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
		GetMidi, WaitMidi

    INTERNALS

    HISTORY

	2001-01-12 ksvalast first created

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct CamdBase *,CamdBase)

	struct MyMidiNode *mymn=(struct MyMidiNode *)midinode;
	UBYTE temp=mymn->error;
	mymn->error=0;
	return temp;

   AROS_LIBFUNC_EXIT
}


