

#include <proto/exec.h>
#include <proto/dos.h>
#include <dos.h>

#include "camd_intern.h"

#undef SysBase
#undef DOSBase

#ifndef min
#define min(a,b) ((a)<=(b)?(a):(b))
#endif


/******************************************************************************

  FUNCTION
	This functions supports the original camd driver format.

******************************************************************************/



__inline void IncBuffer_of(struct DriverData *data,ULONG **buffer){
	(*buffer)++;
	if(*buffer==data->bufferend){
		*buffer=data->buffer;
	}
}

__inline void IncBuffer_rt_of(struct DriverData *data,UBYTE **buffer_rt){
	(*buffer_rt)++;
	if(*buffer_rt==data->bufferend_rt){
		*buffer_rt=data->buffer_rt;
	}
}



// NOTE
//   Eh, I guess the function is a bit clumsy made. Well, it doesn't matter,
//   it works. And its not that important to be extremely fast, I hope. (certainly not
//   for rs-323 drivers, that is.)

ULONG ASM Transmitter_oldformat(REG(a2) struct DriverData *driverdata){
	UBYTE *buf;
	UBYTE len;
	UBYTE ret;

	driverdata->isnowtransmitting=1;

/* Decrease this ones as soon as possible.*/

	driverdata->unsent--;
	driverdata->unsent_rt--;


	if(driverdata->unsent_rt>0){
		driverdata->unsent++;
		ret=*driverdata->buffercurrsend_rt;
		IncBuffer_rt_of(driverdata,&driverdata->buffercurrsend_rt);
		if(driverdata->unsent==0 && driverdata->unsent_rt==0){
			driverdata->isnowtransmitting=0;
			putreg(REG_D1,0xff);
		}else{
			driverdata->isnowtransmitting=0;
			putreg(REG_D1,0);
		}
		return ret;
	}
	driverdata->unsent_rt++;


/*
//	The code inside here is theoretical necessarry. If a driver
// manage to use more than about 8/31200 second to notify itself to
// get a new midi-message (urk, thats much) while it is still
// picking data, the following code-lines must be included.
// The natural solution to such a problem is ofcourse to let the
// driver check if it is sending while notifiying (which I guess
// most of them (how many is there really? 3 or 4? ) does) , but
// the situation can still happen.)

	if(driverdata->unsent<0){
		driverdata->unsent=0;
		driverdata->isnowtransmitting=0;
		putreg(REG_D1,0xff);
		return 0xfd;			//Hope this one is not ever going to be used. A nice midi-nop for now. (The alternative is a buzy-loop)
	}
*/

	buf=(UBYTE *)driverdata->buffercurrsend;
	len=buf[3];

	if(driverdata->sendpos==len){
		ret=buf[len];
		if(len==0){
			driverdata->status=0;		//System common category status cancels running status
			if(ret==0xf0){					//Realtime SysX handling.
				ret=driverdata->buffer_sx[driverdata->buffercurrsend_sx];
				driverdata->buffercurrsend_sx++;
				driverdata->unsent_sx--;
				if(driverdata->unsent_sx>0){
					driverdata->unsent++;
					driverdata->isnowtransmitting=0;
					putreg(REG_D1,0);
					return ret;
				}
			}
		}
		driverdata->sendpos=0;
		IncBuffer_of(driverdata,&driverdata->buffercurrsend);
		if(driverdata->unsent==0 && driverdata->unsent_rt==0){
			driverdata->isnowtransmitting=0;
			putreg(REG_D1,0xff);
			return ret;
		}
	}else{
		driverdata->unsent++;
		if(driverdata->sendpos==0){
			if(driverdata->realtimesysx==1){		//Realtime SysX handling.
				driverdata->status=0;				//System common category status cancels running status
				ret=driverdata->buffer_sx[driverdata->buffercurrsend_sx];
				driverdata->buffercurrsend_sx++;
				driverdata->unsent_sx--;
				if(driverdata->unsent_sx>0){
					driverdata->isnowtransmitting=0;
					putreg(REG_D1,0);
					return ret;
				}else{
					if(driverdata->unsent==0 && driverdata->unsent_rt==0){
						driverdata->realtimesysx=0;
						driverdata->isnowtransmitting=0;
						putreg(REG_D1,0xff);
						driverdata->issending_sx=0;
						return ret;
					}else{
						driverdata->realtimesysx=0;
						driverdata->isnowtransmitting=0;
						driverdata->issending_sx=0;
						putreg(REG_D1,0);
						return ret;
					}
				}
			}
			ret=buf[0];
			if(ret==driverdata->status){
				ret=buf[1];
				if(len==1){
					driverdata->sendpos=0;
					IncBuffer_of(driverdata,&driverdata->buffercurrsend);
					driverdata->unsent--;
					if(driverdata->unsent==0 && driverdata->unsent_rt==0){
						driverdata->isnowtransmitting=0;
						putreg(REG_D1,0xff);
						return ret;
					}
				}else{
					driverdata->sendpos=2;
				}
			}else{
				driverdata->sendpos=1;
				if(ret>0xf0){
					driverdata->status=0;	//System common category status cancels running status
				}else{
					driverdata->status=ret;
				}
			}
		}else{
			ret=buf[1];
			driverdata->sendpos=2;
		}
	}

	driverdata->isnowtransmitting=0;
	putreg(REG_D1,0);
	return ret;
}


BOOL Midi2Driver_rt_oldformat(struct DriverData *driverdata,ULONG msg){

	Disable();
		if(
			driverdata->unsent_rt>=OUTBUFFERSIZE_RT-2 ||
			(driverdata->unsent_rt<=1 && driverdata->isnowtransmitting!=0)
		){
			Enable();
			return FALSE;
		}
		*driverdata->buffercurr_rt=msg;
		driverdata->unsent_rt++;

		IncBuffer_rt_of(driverdata,&driverdata->buffercurr_rt);

		ObtainSemaphore(&driverdata->sendsemaphore);

		Enable();

		(*driverdata->midiportdata->ActivateXmit)(driverdata,driverdata->portnum);

	ReleaseSemaphore(&driverdata->sendsemaphore);

	return TRUE;
}





BOOL Midi2Driver_internal_oldformat(
	struct DriverData *driverdata,
	ULONG msg,
	ULONG maxbuff
){

	if(msg>=0xf8000000) return Midi2Driver_rt_oldformat(driverdata,msg);

	if(driverdata->unsent>=min(maxbuff,OUTBUFFERSIZE-2)){
		return FALSE;
	}

	Disable();
		if(
			driverdata->unsent>=OUTBUFFERSIZE-2 ||
			(driverdata->unsent<=1 && driverdata->isnowtransmitting!=0)
		){
			Enable();
			return FALSE;
		}

		*driverdata->buffercurr=(msg & 0xffffff00) | GetMsgLen(msg);
		driverdata->unsent++;

		IncBuffer_of(driverdata,&driverdata->buffercurr);

		ObtainSemaphore(&driverdata->sendsemaphore);

		Enable();

		(*driverdata->midiportdata->ActivateXmit)(driverdata,driverdata->portnum);

	ReleaseSemaphore(&driverdata->sendsemaphore);


	return TRUE;
}

extern int GetSysXLen(UBYTE *buffer);

BOOL SysEx2Driver_oldformat(struct DriverData *driverdata, UBYTE *buffer){

	int len=GetSysXLen(buffer);

	int realtime= (buffer[1]==0x7f ? 1 : 0);

	if(
		(realtime==0 && driverdata->unsent==OUTBUFFERSIZE-1) ||
		(realtime==1 && driverdata->realtimesysx==1) ||
		driverdata->unsent_sx!=0
	){
		return FALSE;
	}

	Disable();

		if(
			(realtime==0 && driverdata->unsent==OUTBUFFERSIZE-1) ||
			(realtime==1 && driverdata->realtimesysx==1) ||
			driverdata->unsent_sx!=0
		){
			Enable();
			return FALSE;
		}

		driverdata->buffer_sx=buffer;
		driverdata->unsent_sx=len;
		driverdata->buffercurrsend_sx=0;

		if(realtime==0){
			*driverdata->buffercurr=0xf0000000;
			driverdata->unsent++;
			IncBuffer_of(driverdata,&driverdata->buffercurr);
		}else{
			driverdata->realtimesysx=1;
		}

		ObtainSemaphore(&driverdata->sendsemaphore);

		Enable();

		(*driverdata->midiportdata->ActivateXmit)(driverdata,driverdata->portnum);

	ReleaseSemaphore(&driverdata->sendsemaphore);

	return TRUE;
}






