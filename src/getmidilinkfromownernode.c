/*
    (C) 2001 AROS - The Amiga Research OS
    $Id: getmidilinkfromownernode.c,v 1.1 2001/01/17 18:51:30 ksvalast Exp $

    Desc: 
    Lang: English
*/


#include "camd_intern.h"


struct MidiLink *GetMidiLinkFromOwnerNode(struct MinNode *node){
	struct MidiLink dummy;
	return (struct MidiLink *)((char *)((char *)(node)-((char *)&dummy.ml_OwnerNode-(char *)&dummy)));
}

