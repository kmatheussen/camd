/*
**      $VER: StartUp.c 37.32 (15.5.98)
**
**      Library startup-code and function table definition
**
**      (C) Copyright 1996-98 Andreas R. Kleinert
**      All Rights Reserved.
*/

#define __USE_SYSBASE        // perhaps only recognized by SAS/C

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/libraries.h>
#include <exec/execbase.h>
#include <exec/resident.h>
#include <exec/initializers.h>

#ifdef __MAXON__
#include <pragma/exec_lib.h>
#include <linkerfunc.h>
#else
#include <proto/exec.h>    // all other compilers
#endif

#include <proto/dos.h>
#include <proto/utility.h>

#define SEGLISTPTR BPTR

#include <libcore/compiler.h>
#include "libdefs.h"

#include "camd_intern.h"

extern ULONG __saveds __stdargs L_OpenLibs(struct LIBBASE *LIBBASE);
extern void  __saveds __stdargs L_CloseLibs(void);

struct LIBBASE * __saveds ASM InitLib(register __a6 struct ExecBase *sysbase GNUCREG(a6),
                                      register __a0 SEGLISTPTR      seglist  GNUCREG(a0),
                                      register __d0 struct LIBBASE  *exb     GNUCREG(d0));
struct LIBBASE * __saveds ASM OpenLib(register __a6 struct LIBBASE  *LIBBASE GNUCREG(a6));
SEGLISTPTR __saveds ASM CloseLib(register __a6 struct LIBBASE *LIBBASE GNUCREG(a6));
SEGLISTPTR __saveds ASM ExpungeLib(register __a6 struct LIBBASE *exb GNUCREG(a6));
ULONG ASM ExtFuncLib(void);


/* ------------------------------------------------------------------------
   ! LibStart:
   !
   ! If someone tries to start a library as an executable, it must return
   ! (LONG) -1 as result. That's what we are doing here.
   ------------------------------------------------------------------------
*/

LONG ASM LibStart(void)
{
    return(-1);
}


/* ------------------------------------------------------------------------
   ! Function and Data Tables:
   !
   ! The function and data tables have been placed here for traditional
   ! reasons. Placing the RomTag structure before (-> LibInit.c) would also
   ! be a good idea, but it depends on whether you would like to keep the
   ! "version" stuff separately.
   ------------------------------------------------------------------------
*/

extern APTR FuncTab [];
/*  extern struct MyDataInit DataTab;  */
extern DataTab; /* DICE fix */
                   /* Instead you may place ROMTag + Datatab directly, here */
                   /* (see LibInit.c). This may fix "Installer" version     */
                   /* checking problems, too - try it.                      */

struct InitTable                       /* do not change */
{
    ULONG              LibBaseSize;
    APTR              *FunctionTable;
    struct MyDataInit *DataTable;
    APTR               InitLibTable;
} InitTab =
{
    (ULONG)               sizeof(struct LIBBASE),
    (APTR              *) &FuncTab[0],
    (struct MyDataInit *) &DataTab,
    (APTR)                InitLib
};


#include "FunctionTable.h"

extern struct LIBBASE *LIBBASE;

/* ------------------------------------------------------------------------
   ! InitLib:
   !
   ! This one is single-threaded by the Ramlib process. Theoretically you
   ! can do, what you like here, since you have full exclusive control over
   ! all the library code and data. But due to some bugs in Ramlib V37-40,
   ! you can easily cause a deadlock when opening certain libraries here
   ! (which open other libraries, that open other libraries, that...)
   ------------------------------------------------------------------------
*/


struct LIBBASE * __saveds ASM InitLib(register __a6 struct ExecBase *sysbase GNUCREG(a6),
                                      register __a0 SEGLISTPTR      seglist  GNUCREG(a0),
                                      register __d0 struct LIBBASE  *exb     GNUCREG(d0))
{
#undef SysBase

	SysBase=sysbase;

    LIBBASE = exb;

    LIBBASE->sysbase = sysbase;
    LIBBASE->seglist = seglist;

//	return(LIBBASE);

	LIBBASE->utilitybase=OpenLibrary("utility.library", 37);

    if (!LIBBASE->utilitybase)
        return NULL;

	LIBBASE->dosbase=OpenLibrary("dos.library", 37);

    if (!LIBBASE->dosbase)
        return NULL;

#undef DosBase
#undef DOSBase
	DOSBase=LIBBASE->dosbase;
#undef UtilityBase
	UtilityBase=LIBBASE->utilitybase;

	if(InitCamd((struct CamdBase *)LIBBASE)==FALSE){
		return NULL;
	}

	return(LIBBASE);

}

/* ------------------------------------------------------------------------
   ! OpenLib:
   !
   ! This one is enclosed within a Forbid/Permit pair by Exec V37-40. Since
   ! a Wait() call would break this Forbid/Permit(), you are not allowed to
   ! start any operations that may cause a Wait() during their processing.
   ! It's possible, that future OS versions won't turn the multi-tasking
   ! off, but instead use semaphore protection for this  function.
   !
   ! Currently you only can bypass this restriction by supplying your own
   ! semaphore mechanism.
   ------------------------------------------------------------------------
*/

struct LIBBASE * __saveds ASM OpenLib(register __a6 struct LIBBASE *LIBBASE GNUCREG(a6))
{
    #ifdef __MAXON__
    GetBaseReg();
    InitModules();
    #endif

    LIBBASE->library.lib_OpenCnt++;

    LIBBASE->library.lib_Flags &= ~LIBF_DELEXP;

    return(LIBBASE);
}

/* ------------------------------------------------------------------------
   ! CloseLib:
   !
   ! This one is enclosed within a Forbid/Permit pair by Exec V37-40. Since
   ! a Wait() call would break this Forbid/Permit(), you are not allowed to
   ! start any operations that may cause a Wait() during their processing.
   ! It's possible, that future OS versions won't turn the multi-tasking
   ! off, but instead use semaphore protection for this function.
   !
   ! Currently you only can bypass this restriction by supplying your own
   ! semaphore mechanism.
   ------------------------------------------------------------------------
*/

SEGLISTPTR __saveds ASM CloseLib(register __a6 struct LIBBASE *LIBBASE GNUCREG(a6))
{
    LIBBASE->library.lib_OpenCnt--;

    if(!LIBBASE->library.lib_OpenCnt)
    {
        if(LIBBASE->library.lib_Flags & LIBF_DELEXP)
        {
            return( ExpungeLib(LIBBASE) );
        }
    }

    return(NULL);
}

/* ------------------------------------------------------------------------
   ! ExpungeLib:
   !
   ! This one is enclosed within a Forbid/Permit pair by Exec V37-40. Since
   ! a Wait() call would break this Forbid/Permit(), you are not allowed to
   ! start any operations that may cause a Wait() during their processing.
   ! It's possible, that future OS versions won't turn the multi-tasking
   ! off, but instead use semaphore protection for this function.
   !
   ! Currently you only could bypass this restriction by supplying your own
   ! semaphore mechanism - but since expunging can't be done twice, one
   ! should avoid it here.
   ------------------------------------------------------------------------
*/

SEGLISTPTR __saveds ASM ExpungeLib(register __a6 struct LIBBASE *exb GNUCREG(a6))
{
    struct LIBBASE *LIBBASE = exb;
    SEGLISTPTR seglist;

    if(!LIBBASE->library.lib_OpenCnt)
    {
        ULONG negsize, possize, fullsize;
        UBYTE *negptr = (UBYTE *) LIBBASE;

        seglist = LIBBASE->seglist;

			UninitCamd((struct CamdBase *)LIBBASE);

        Remove((struct Node *)LIBBASE);

	    if(LIBBASE->utilitybase==NULL){
		    CloseLibrary((struct Library *)LIBBASE->utilitybase);
		    LIBBASE->utilitybase = NULL;
	    }

	    if(LIBBASE->dosbase==NULL){
		    CloseLibrary((struct Library *)LIBBASE->dosbase);
		    LIBBASE->dosbase = NULL;
	    }

        negsize  = LIBBASE->library.lib_NegSize;
        possize  = LIBBASE->library.lib_PosSize;
        fullsize = negsize + possize;
        negptr  -= negsize;

        FreeMem(negptr, fullsize);

        #ifdef __MAXON__
        CleanupModules();
        #endif
      
        return(seglist);
    }

    LIBBASE->library.lib_Flags |= LIBF_DELEXP;

    return(NULL);
}

/* ------------------------------------------------------------------------
   ! ExtFunct:
   !
   ! This one is enclosed within a Forbid/Permit pair by Exec V37-40. Since
   ! a Wait() call would break this Forbid/Permit(), you are not allowed to
   ! start any operations that may cause a Wait() during their processing.
   ! It's possible, that future OS versions won't turn the multi-tasking
   ! off, but instead use semaphore protection for this function.
   !
   ! Currently you only can bypass this restriction by supplying your own
   ! semaphore mechanism - but since this function currently is unused, you
   ! should not touch it, either.
   ------------------------------------------------------------------------
*/

ULONG ASM ExtFuncLib(void)
{
    return(NULL);
}

struct LIBBASE *LIBBASE = NULL;

struct DosLibrary *DOSBase=NULL;
struct Library *UtilityBase=NULL;

/* ------------------------------------------------------------------------
   ! __SASC stuff:
   !
   ! This is only for SAS/C - its intention is to turn off internal
   ! CTRL-C handling for standard C functions and to avoid calls to exit()
   ! et al.
   ------------------------------------------------------------------------
*/

#ifdef __SASC

#ifdef ARK_OLD_STDIO_FIX

ULONG XCEXIT       = NULL; /* These symbols may be referenced by    */
ULONG _XCEXIT      = NULL; /* some functions of sc.lib, but should  */
ULONG ONBREAK      = NULL; /* never be used inside a shared library */
ULONG _ONBREAK     = NULL;
ULONG base         = NULL; /* Note, that XCEXIT/ONBREAK actually    */
ULONG _base        = NULL; /* should have been defined as functions */
ULONG ProgramName  = NULL; /* and not as ULONGs...                  */
ULONG _ProgramName = NULL;
ULONG StackPtr     = NULL;
ULONG _StackPtr    = NULL;
ULONG oserr        = NULL;
ULONG _oserr       = NULL;
ULONG OSERR        = NULL;
ULONG _OSERR       = NULL;

#endif /* ARK_OLD_STDIO_FIX */

void __regargs __chkabort(void) { }  /* a shared library cannot be    */
void __regargs _CXBRK(void)     { }  /* CTRL-C aborted when doing I/O */

#endif /* __SASC */
