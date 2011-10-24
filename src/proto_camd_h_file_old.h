#ifndef CAMD_INTERN_PROTO_CAMD_H_FILE_H
#define CAMD_INTERN_PROTO_CAMD_H_FILE_H

#ifdef _AROS
#  include <proto/camd.h>
#  define CAMDBASEADD
#else

#  include <proto/camd.h>
#  define CAMDBASEADD
#if 0
#	include "camd_intern.h"

#  define CAMDBASEADD ,CamdBase

	AROS_LH2(BOOL, SetMidiAttrsA,
	AROS_LHA(struct MidiNode *, midinode, A0),
	AROS_LHA(struct TagItem *, tags, A1),
	struct CamdBase *, CamdBase, 14, Camd);

	AROS_LH2(BOOL, GetMidi,
	AROS_LHA(struct MidiNode *, midinode, A0),
	AROS_LHA(MidiMsg *, msg, A1),
	struct CamdBase *, CamdBase, 28, Camd);

	AROS_LH2(BOOL, SetMidiLinkAttrsA,
	AROS_LHA(struct MidiLink *, midilink, A0),
	AROS_LHA(struct TagItem *, tags, A1),
	struct CamdBase *, CamdBase, 21, Camd);

	AROS_LH1(void, CloseMidiDevice,
	AROS_LHA(struct MidiDeviceData *, mididevicedata, A0),
	struct CamdBase *, CamdBase, 35, Camd);

	AROS_LH1(struct MidiDeviceData *, OpenMidiDevice,
	AROS_LHA(UBYTE *, name, A0),
	struct CamdBase *, CamdBase, 34, Camd);

	AROS_LH3(APTR, GoodPutMidi,
	AROS_LHA(struct MidiLink *, link, A0),
	AROS_LHA(ULONG, msg, D0),
	AROS_LHA(ULONG, maxbuff, D1),
	struct CamdBase *, CamdBase, 23, Camd);
#endif
#endif

#endif

