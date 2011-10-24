#ifndef FUNCTIONTABLE_H
#define FUNCTIONTABLE_H
/*
    This file is generated from GenFDCLIBFuncTab.rexx, please
    edit only the "-- Add your own functions here --" part.
    For more informations see docs.
*/

#include <exec/types.h>
#include <libcore/compiler.h>

extern APTR CamdLockCAMD;extern APTR
CamdUnlockCAMD;extern APTR
CamdCreateMidiA;extern APTR
CamdDeleteMidi;extern APTR
CamdGetMidiAttrsA;extern APTR
CamdNextMidi;extern APTR
CamdFindMidi;extern APTR
CamdFlushMidi;extern APTR
CamdAddMidiLinkA;extern APTR
CamdRemoveMidiLink;extern APTR
CamdGetMidiLinkAttrsA;extern APTR
CamdNextClusterLink;extern APTR
CamdNextMidiLink;extern APTR
CamdMidiLinkConnected;extern APTR
CamdNextCluster;extern APTR
CamdFindCluster;extern APTR
CamdPutMidi;extern APTR
CamdWaitMidi;extern APTR
CamdPutSysEx;extern APTR
CamdGetSysEx;extern APTR
CamdQuerySysEx;extern APTR
CamdSkipSysEx;extern APTR
CamdGetMidiErr;extern APTR
CamdMidiMsgType;extern APTR
CamdMidiMsgLen;extern APTR
CamdParseMidi;extern APTR
CamdOpenMidiDevice;extern APTR
CamdCloseMidiDevice;extern APTR
CamdRethinkCAMD;extern APTR
CamdStartClusterNotify;extern APTR
CamdEndClusterNotify;extern APTR
CamdSetMidiAttrsA;extern APTR
CamdSetMidiLinkAttrsA;extern APTR
CamdGetMidi;extern APTR
CamdGoodPutMidi;extern APTR
CamdMidi2Driver;

APTR FuncTab [] =
{
    OpenLib,
    CloseLib,
    ExpungeLib,
    ExtFuncLib,

/* -- Add your own functions here -- */
&
CamdLockCAMD,&
CamdUnlockCAMD,&
CamdCreateMidiA,&
CamdDeleteMidi,&
CamdSetMidiAttrsA,&
CamdGetMidiAttrsA,&
CamdNextMidi,&
CamdFindMidi,&
CamdFlushMidi,&
CamdAddMidiLinkA,&
CamdRemoveMidiLink,&
CamdSetMidiLinkAttrsA,&
CamdGetMidiLinkAttrsA,&
CamdNextClusterLink,&
CamdNextMidiLink,&
CamdMidiLinkConnected,&
CamdNextCluster,&
CamdFindCluster,&
CamdPutMidi,&
CamdGetMidi,&
CamdWaitMidi,&
CamdPutSysEx,&
CamdGetSysEx,&
CamdQuerySysEx,&
CamdSkipSysEx,&
CamdGetMidiErr,&
CamdMidiMsgType,&
CamdMidiMsgLen,&
CamdParseMidi,&
CamdOpenMidiDevice,&
CamdCloseMidiDevice,&
CamdRethinkCAMD,&
CamdStartClusterNotify,&
CamdEndClusterNotify,&
CamdGoodPutMidi,&
CamdMidi2Driver,

/* -- End of your own functions   -- */

    (APTR) ((LONG) -1)
};
#endif
