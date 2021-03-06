/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: midimsgtype.c,v 1.1 2001/01/17 18:51:30 ksvalast Exp $

    Desc: 
    Lang: English
*/

#include "camd_intern.h"


/*****************************************************************************

    NAME */

	AROS_LH1(WORD, MidiMsgType,

/*  SYNOPSIS */
	AROS_LHA(MidiMsg *, msg, A0),

/*  LOCATION */
	struct CamdBase *, CamdBase, 31, Camd)

/*  FUNCTION
		Return the type of a message (see <midi/camd.h>). sysex messages
		returns -1.

    INPUTS
		msg - midimessage.

    RESULT

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

	if(msg->mm_Status==0xf0 || msg->mm_Status==0xf7) return -1;

	return MidiMsgType_status_data1(msg->mm_Status,msg->mm_Data1);

   AROS_LIBFUNC_EXIT
}

