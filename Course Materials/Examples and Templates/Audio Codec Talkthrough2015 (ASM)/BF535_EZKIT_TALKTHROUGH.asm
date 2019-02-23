/*****************************************************************************************************************

(C) Copyright 2001 - Analog Devices, Inc.  All rights reserved.

File Name:		BF535_EZKIT_TALKTHROUGH

Date Modified:	10/10/01		JB		Rev 0.1

Purpose:	  	ADSP-BF535 EZ-Kit Lite Initialization and Main Program Shell                 
		       	Developed using the ADSP-BF535 EZ-KIT Lite Evaluation Platform                
										  
				This routine contains function calls and routines to initialize the 	  
				state of the BF535, program the DMA controller, initialize the AD1885   
				
				This project contains an audio Talkthru example for the ADSP-BF535 EZ-KIT Lite evaluation system.
				The main code module is used to configure the AC'97 interface and AC'97 CODEC (AD1885)
				in a talkthrough fashion.  The analog front end of CODEC receives the analog audio data, and 
				converts it using the A/D converter.  It is then sent to the ADSP-BF535 via SPORT0, configured
	   			for slot 16 mode AC'97 link.  
			   	On the DSP side, the data is received and placed into a buffer in memory.  The DSP transmits 
				data from this same buffer to the codec via the SPORT0 - AC'97 link.  The CODEC receives this digital 
			   	data, converts it back to an analog representation and sends the data out of the head phone 
			   	interface.

************************************************************************************************/

#include 	<defBF535.h>

/************************************************************************************************/
/*			   GLOBAL & EXTERNAL DECLARATIONS													*/	
/************************************************************************************************/    
.SECTION	data1;
.GLOBAL		_main;
.EXTERN 	Codec_Reset;
.EXTERN		Program_SPORT0_Registers;
.EXTERN		Program_DMA_Controller;
.EXTERN		AD1885_Codec_Initialization;
.EXTERN		Process_AD1885_Audio_Samples;
.EXTERN 	RX_Status;
.EXTERN		rx_buf;

/************************************************************************************************/  
/* 				Program             															*/
/************************************************************************************************/ 
.SECTION	program;

_main:	
	
	p0.l = lo(EVT8);
	p0.h = hi(EVT8);
	r1 	 = _I8HANDLER (Z);
	r1.h = _I8HANDLER;  	// Emulation Handler (Int8)
    [ P0 ] = R1;

	call Codec_Reset;	
	call Program_SPORT0_Registers;		/* Initialize SPORT0 for codec communications */
	call Program_DMA_Controller;		/* Start Serial Port 0 tx and rx DMA Transfers */
	call AD1885_Codec_Initialization;	/* Initialize & program AD1885 */


	/* Clear RX_Status flag indicating incoming RX data is audio data and can be processed  according */
	p0.l = RX_Status;
	p0.h = RX_Status;
	r0 = 0x0000 (z);
	[p0] = r0;
wait_forever:
	jump wait_forever;


_I8HANDLER:           // IVG 8 Handler
	[--SP] = (r7:0);
	[--SP] = (p5:0);
	R0 = LENGTH(rx_buf);
	p0.l = SPORT0_COUNT_RX & 0xFFFF;
	p0.h = (SPORT0_COUNT_RX >> 16) & 0xFFFF;
	R1 = W[p0];
	R0 = R0 - R1;
	CC = AZ;
	IF CC jump Roll_Over;

	R0.l = 0x8000;
	p0.l = RX_Status;
	p0.h = RX_Status;
	r1 = w[p0];
	R0 = R0 & R1;
	CC = AZ;
    IF !CC jump Roll_Over;
	call Process_AD1885_Audio_Samples;

Roll_Over:
	R0 = 0x0007;			// Clear RX Interrupts
	P0.L = SPORT0_IRQSTAT_RX & 0xFFFF;
	P0.H = (SPORT0_IRQSTAT_RX >> 16) & 0xFFFF;
	W[p0] = R0;

	(p5:0) = [SP++];  		// Restore Regs
	(r7:0) = [SP++];
	RTI;

_main.end:

