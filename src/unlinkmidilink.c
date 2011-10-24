/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: unlinkmidilink.c,v 1.3 2001/01/22 00:43:31 ksvalast Exp $

    Desc: 
    Lang: English
*/

#include <proto/exec.h>

#include "camd_intern.h"


/* 
	CLSemaphore must be exlusive obtained first.
*/

void UnlinkMidiLink(
	struct MidiLink *midilink,
	BOOL unlinkfromnode,
	struct CamdBase *CamdBase
){
	struct MidiCluster *cluster=midilink->ml_Location;
	int type=midilink->ml_Node.ln_Type;
	struct MyMidiCluster *mycluster=(struct MyMidiCluster *)cluster;

	if(cluster!=NULL){
		if(type==NT_USER-MLTYPE_Receiver){
			ObtainSemaphore(&mycluster->semaphore);
		}
		Remove(&midilink->ml_Node);
	}

	if(unlinkfromnode==TRUE){
		Remove((struct Node *)&midilink->ml_OwnerNode);
	}

	if(cluster!=NULL){
		if(type==NT_USER-MLTYPE_Receiver){
			ReleaseSemaphore(&mycluster->semaphore);
		}
		D(bug("    Unlinkinking from cluster -%s-\n",cluster->mcl_Node.ln_Name));
		LinkHasBeenRemovedFromCluster(cluster,CamdBase);
	}

}

