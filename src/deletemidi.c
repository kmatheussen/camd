/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: deletemidi.c,v 1.2 2001/01/22 00:43:31 ksvalast Exp $

    Desc: 
    Lang: English
*/

#include <proto/exec.h>

#include "camd_intern.h"

/*****************************************************************************

    NAME */

	AROS_LH1(void, DeleteMidi,

/*  SYNOPSIS */
	AROS_LHA(struct MidiNode *, midinode, A0),

/*  LOCATION */
	struct CamdBase *, CamdBase, 8, Camd)

/*  FUNCTION
		First deletes all midilinks attached to the midinode, then
		frees all buffers, before it frees itself.

    INPUTS

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

	struct MyMidiNode *mymidinode=(struct MyMidiNode *)midinode;
	struct MinNode *node,*temp;
	struct MidiLink *midilink;

	D(bug("starting to remove node -%s-\n",midinode->mi_Node.ln_Name));

	ObtainSemaphore(CB(CamdBase)->CLSemaphore);

		if( ! (IsListEmpty((struct List *)&midinode->mi_OutLinks))){
			node=midinode->mi_OutLinks.mlh_Head;
			while(node->mln_Succ!=NULL){
				temp=node->mln_Succ;

				midilink=GetMidiLinkFromOwnerNode(node);
				D(bug("  removing out link: %s \n",midilink->ml_Node.ln_Name));
				UnlinkMidiLink(midilink,FALSE,CamdBase);
				D(bug("  link removed\n"));

				if(midilink->ml_ParserData!=NULL) FreeMem(midilink->ml_ParserData,sizeof(struct DriverData));
				FreeMem(midilink,sizeof(struct MidiLink));

				node=temp;
			}
		}
		if( ! (IsListEmpty((struct List *)&midinode->mi_InLinks))){
			node=midinode->mi_InLinks.mlh_Head;
			while(node->mln_Succ!=NULL){
				temp=node->mln_Succ;

				midilink=GetMidiLinkFromOwnerNode(node);

				D(bug("  removing in link: %s \n",midilink->ml_Node.ln_Name));
				UnlinkMidiLink(midilink,FALSE,CamdBase);
				D(bug("  link removed\n"));

				if(midilink->ml_ParserData!=NULL) FreeMem(midilink->ml_ParserData,sizeof(struct DriverData));
				FreeMem(midilink,sizeof(struct MidiLink));

				node=temp;
			}
		}

		Remove(&midinode->mi_Node);

	ReleaseSemaphore(CB(CamdBase)->CLSemaphore);

	D(bug("Node -%s- removed\n",midinode->mi_Node.ln_Name));

	if(mymidinode->in_start!=NULL){
		FreeVec(mymidinode->in_start);
	}
	if(mymidinode->sysex_start!=NULL){
		FreeVec(mymidinode->sysex_start);
	}
	FreeMem(midinode,sizeof(struct MyMidiNode));

   AROS_LIBFUNC_EXIT

}



