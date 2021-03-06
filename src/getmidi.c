/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: getmidi.c,v 1.1 2001/01/17 18:51:30 ksvalast Exp $

    Desc: 
    Lang: English
*/

#include <proto/exec.h>

#include "camd_intern.h"

/*****************************************************************************

    NAME */

	AROS_LH2(BOOL, GetMidi,

/*  SYNOPSIS */
	AROS_LHA(struct MidiNode *, midinode, A0),
	AROS_LHA(MidiMsg *, msg, A1),

/*  LOCATION */
	struct CamdBase *, CamdBase, 24, Camd)

/*  FUNCTION
		Gets a message from a midinodes buffer.

    INPUTS
		midinode - pointer to midinode
		msg - The message is removed from the internal buffer and copied into msg.

    RESULT
		TRUE if message was copied, FALSE if buffer was empty.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
		WaitMidi

    INTERNALS

    HISTORY

	2001-01-12 ksvalast first created

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct CamdBase *,CamdBase)

	struct MyMidiNode *mymidinode=(struct MyMidiNode *)midinode;
	MidiMsg *mymsg;

	if(mymidinode->unpicked==0){
		return FALSE;
	}
	mymsg=mymidinode->in_curr_get;

	msg->mm_Msg=mymsg->mm_Msg;
	msg->mm_Time=mymsg->mm_Time;

	mymidinode->unpicked--;

	mymidinode->in_curr_get++;
	if(mymidinode->in_curr_get==mymidinode->in_end){
		mymidinode->in_curr_get=mymidinode->in_start;
	}

	mymidinode->lastreadstatus=msg->mm_Status;

	if(msg->mm_Status==0xf0)
		mymidinode->sysex_nextis0=FALSE;

	return TRUE;

   AROS_LIBFUNC_EXIT
}



