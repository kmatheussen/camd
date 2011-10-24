
CPU=68000
OPTIMIZE=optimize optimizetime optimizerinline optglobal optimizercomplexity=100 optimizerdepth=100 optimizerinlocal optimizerpeephole optimizerrecurdepth=100 optloop

OPT=nolink CPU=$(CPU) debug=line nostackcheck parm=reg strict ansi ignore=51 ignore=105 ignore=220  define __USE_SYSBASE=1 $(OPTIMIZE)

# define DEBUG
LOPT=LIB LIB:debug.lib LIB:amiga.lib
# LIB:sc.lib
CC=sc
LINKER=slink
LIBBER=oml

OBJ=StartUp.o LibInit.o PutMidi.o CreateMidiA.o LockCAMD.o UnlockCAMD.o NextCluster.o clusters.o AddMidiLinkA.o GetMidi.o init.o PutSysEx.o MidiMsgType.o ReceiverProc.o MidiToDriver.o GetMidiErr.o MidiLinkConnected.o MidiMsgLen.o NextClusterLink.o NextMidi.o NextMidiLink.o RemoveMidiLink.o WaitMidi.o MidiFromDriver.o GetSysEx.o QuerySysEx.o SkipSysEx.o SetMidiLinkAttrsA.o drivers.o SetMidiAttrsA.o FlushMidi.o DeleteMidi.o GetMidiAttrsA.o GetMidiLinkAttrsA.o OpenMidiDevice.o CloseMidiDevice.o Strings.o UnlinkMidiLink.o MidiDistr.o SysExDistr.o GetMidiLinkFromOwnerNode.o MidiMsgType2.o FindCluster.o FindMidi.o ParseMidi.o RethinkCAMD.o StartClusterNotify.o EndClusterNotify.o camdwait.o goodputmidi.o midi2driver.o miditodriver_oldformat.o

all: camd_test.library

clean:
	delete *.o *.library
	smake

camd_test.library: $(OBJ) smakefile.smk makefile.with
	$(LINKER) $(LOPT) TO camd_test.library with makefile.with
	copy camd_test.library libs:camd.library

#minicamd$(CPU).lib: $(OBJ) makefile.with
#	$(LIBBER) -v minicamd68000.lib r @makefile.with

#minicamd$(CPU).lib r $(OBJ)

main.o: main.c
	$(CC) $(OPT) main.c

PutMidi.o: PutMidi.c
	$(CC) $(OPT) PutMidi.c disassemble=PutMidi.dis

CreateMidiA.o: CreateMidiA.c
	$(CC) $(OPT) CreateMidiA.c

LockCAMD.o: LockCAMD.c
	$(CC) $(OPT) LockCAMD.c

NextCluster.o: NextCluster.c
	$(CC) $(OPT) NextCluster.c

clusters.o: clusters.c
	$(CC) $(OPT) clusters.c disassemble=clusters.dis

AddMidiLinkA.o: AddMidiLinkA.c
	$(CC) $(OPT) AddMidiLinkA.c

GetMidi.o: GetMidi.c
	$(CC) $(OPT) GetMidi.c disassemble=GetMidi.dis

init.o: init.c
	$(CC) $(OPT) init.c

camdtest.o: camdtest.c
	$(CC) $(OPT) camdtest.c

PutSysEx.o: PutSysEx.c
	$(CC) $(OPT) PutSysEx.c

MidiMsgType.o: MidiMsgType.c
	$(CC) $(OPT) MidiMsgType.c

ReceiverProc.o: ReceiverProc.c
	$(CC) $(OPT) ReceiverProc.c disassemble=receiverproc.dis

MidiToDriver.o: MidiToDriver.c
	$(CC) $(OPT) MidiToDriver.c disassemble=miditodriver.dis

GetMidiErr.o: GetMidiErr.c
	$(CC) $(OPT) GetMidiErr.c

MidiLinkConnected.o: MidiLinkConnected.c
	$(CC) $(OPT) MidiLinkConnected.c

MidiMsgLen.o: MidiMsgLen.c
	$(CC) $(OPT) MidiMsgLen.c

NextClusterLink.o: NextClusterLink.c
	$(CC) $(OPT) NextClusterLink.c

NextMidi.o: NextMidi.c
	$(CC) $(OPT) NextMidi.c

NextMidiLink.o: NextMidiLink.c
	$(CC) $(OPT) NextMidiLink.c

RemoveMidiLink.o: RemoveMidiLink.c
	$(CC) $(OPT) RemoveMidiLink.c

WaitMidi.o: WaitMidi.c
	$(CC) $(OPT) WaitMidi.c

MidiFromDriver.o: MidiFromDriver.c
	$(CC) $(OPT) MidiFromDriver.c disassemble=MidiFromDriver.dis

GetSysEx.o: GetSysEx.c
	$(CC) $(OPT) GetSysEx.c

QuerySysEx.o: QuerySysEx.c
	$(CC) $(OPT) QuerySysEx.c

SkipSysEx.o: SkipSysEx.c
	$(CC) $(OPT) SkipSysEx.c

SetMidiLinkAttrsA.o: SetMidiLinkAttrsA.c
	$(CC) $(OPT) SetMidiLinkAttrsA.c

drivers.o: drivers.c
	$(CC) $(OPT) drivers.c disassemble=drivers.dis

SetMidiAttrsA.o: SetMidiAttrsA.c
	$(CC) $(OPT) SetMidiAttrsA.c

FlushMidi.o: FlushMidi.c
	$(CC) $(OPT) FlushMidi.c

DeleteMidi.o: DeleteMidi.c
	$(CC) $(OPT) DeleteMidi.c

GetMidiAttrsA.o: GetMidiAttrsA.c
	$(CC) $(OPT) GetMidiAttrsA.c

GetMidiLinkAttrsA.o: GetMidiLinkAttrsA.c
	$(CC) $(OPT) GetMidiLinkAttrsA.c

OpenMidiDevice.o: OpenMidiDevice.c
	$(CC) $(OPT) OpenMidiDevice.c

CloseMidiDevice.o: CloseMidiDevice.c
	$(CC) $(OPT) CloseMidiDevice.c

Strings.o: Strings.c
	$(CC) $(OPT) Strings.c

StartUp.o: StartUp.c FunctionTable.h
	$(CC) $(OPT) StartUp.c disassemble=StartUp.dis DEFINE FUNCTIONTABLE

LibInit.o: LibInit.c
	$(CC) $(OPT) LibInit.c

UnlinkMidiLink.o: UnlinkMidiLink.c
	$(CC) $(OPT) UnlinkMidiLink.c

MidiDistr.o: MidiDistr.c
	$(CC) $(OPT) MidiDistr.c disassemble=MidiDistr.dis

SysExDistr.o: SysExDistr.c
	$(CC) $(OPT) SysExDistr.c

GetMidiLinkFromOwnerNode.o: GetMidiLinkFromOwnerNode.c
	$(CC) $(OPT) GetMidiLinkFromOwnerNode.c

MidiMsgType2.o: MidiMsgType2.c
	$(CC) $(OPT) MidiMsgType2.c

FindCluster.o: FindCluster.c
	$(CC) $(OPT) FindCluster.c

UnlockCAMD.o: UnlockCAMD.c
	$(CC) $(OPT) UnlockCAMD.c

FindMidi.o: FindMidi.c
	$(CC) $(OPT) FindMidi.c

ParseMidi.o: ParseMidi.c
	$(CC) $(OPT) ParseMidi.c

RethinkCAMD.o: RethinkCAMD.c
	$(CC) $(OPT) RethinkCAMD.c

StartClusterNotify.o: StartClusterNotify.c
	$(CC) $(OPT) StartClusterNotify.c

EndClusterNotify.o: EndClusterNotify.c
	$(CC) $(OPT) EndClusterNotify.c

camdwait.o: camdwait.c
	$(CC) $(OPT) camdwait.c

goodputmidi.o: goodputmidi.c
	$(CC) $(OPT) goodputmidi.c


midi2driver.o: midi2driver.c
	$(CC) $(OPT) midi2driver.c


miditodriver_oldformat.o: miditodriver_oldformat.c
	$(CC) $(OPT) miditodriver_oldformat.c disassemble=dis.dis


#.o: .c
#	$(CC) $(OPT) .c
