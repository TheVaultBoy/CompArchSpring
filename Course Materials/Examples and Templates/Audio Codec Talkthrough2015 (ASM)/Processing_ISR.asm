/*****************************************************************************************************************

(C) Copyright 2001 - Analog Devices, Inc.  All rights reserved.

File Name:		Processing_ISR.asm

Date Modified:	10/10/01		JB		Rev 0.1
				
Purpose:	   AD1885 SPORT0 RX INTERRUPT SERVICE ROUTINE

Receives LINEIN input from the AD1885 via SPORT0 and then transmits the audio data back out to the 
AD1885 Stereo DACs/Head-phone Outputs

This ISR version assumes the use of the default 48kHz pro audio sample rate, in which data is valid for
every audio frame.  Therefore, TAG slot info and ADC valid bit synchronization is not as critical, 
since the tag bits and ADC valid bits are being set by the AD1885 and the DSP every time there is a 
new audio frame (and thus a new interrupt) Therefore, the RX Interrupt can be used for audio processing 
after codec initialization.  This makes it somewhat easier to initialize the codec, while saving 
the user the extra overhead and code space for programming the codec to use it's variable sample rate 
features.

***************************************************************************************************** 

   Serial Port 0 Receive Interrupt Service Routine performs arithmetic computations on SPORT0 receive 
   data buffer (rx_buf) and sends results to SPORT0 transmit data buffer (tx_buf) 			     		
										     		
 rx_buf[16] - DSP SPORT recieve buffer
 Slot # Description                         	DSP Data Memory Address
 ------ --------------------------------------	-------------------------------------------------
 0		AD1885 Tag Phase						DM(rx_buf + 0) = DM(rx_buf + TAG_PHASE) 
 1      Status Address Port   					DM(rx_buf + 1) = DM(rx_buf + STATUS_ADDRESS_SLOT)
 2      Status Data Port   						DM(rx_buf + 2) = DM(rx_buf + STATUS_DATA_SLOT)
 3      Master PCM Capture (Record) Left Chan.	DM(rx_buf + 3) = DM(rx_buf + LEFT)
 4      Master PCM Capture Right Channel		DM(rx_buf + 4) = DM(rx_buf + RIGHT)
 5 - 15 NOT USED

 tx_buf[16] - DSP SPORT transmit buffer
 Slot # Description                   				DSP Data Memory Address
 ------ --------------------------------------  --------------------------------------------------
 0		ADSP-219x Tag Phase						DM(tx_buf + 0) = DM(tx_buf + TAG_PHASE) 
 1      Command Address Port   					DM(tx_buf + 1) = DM(rx_buf + COMMAND_ADDRESS_SLOT) 
 2      Command Data Port  						DM(tx_buf + 2) = DM(rx_buf + COMMAND_DATA_SLOT) 
 3      Master PCM Playback Left Channel		DM(tx_buf + 3) = DM(rx_buf + LEFT) 
 4      Master PCM Playback Right Channel		DM(tx_buf + 4) = DM(rx_buf + RIGHT) 
 5 - 15 NOT USED

*****************************************************************************************************/

#include 	<defBF535.h>

/*****************************************************************************************************
   CONSTANT & MACRO DEFINITIONS
*****************************************************************************************************/
/* AD1885 TDM Timeslot Definitions */
//.SECTION	constdata;
#define		TAG_PHASE		0
#define		COMMAND_ADDRESS_SLOT	2
#define		COMMAND_DATA_SLOT	4
#define		STATUS_ADDRESS_SLOT	2
#define		STATUS_DATA_SLOT	4
#define		LEFT			6
#define		RIGHT			8

/* Left and Right ADC valid Bits used for testing of valid audio data in current TDM frame */
#define		M_Left_ADC		12
#define		M_Right_ADC		11

/*****************************************************************************************************
   GLOBAL & EXTERNAL DECLARATIONS
*****************************************************************************************************/
.SECTION 	data1;
.GLOBAL		Process_AD1885_Audio_Samples;
.extern		rx_buf;
.extern		tx_buf;

/************************************************************************************************/
/* 				DM data 																		*/
/************************************************************************************************/
/* AD1885 stereo-channel data holders - used for DSP processing of audio data recieved from codec */
.byte2 		Left_Channel;
.byte2 		Right_Channel;

/************************************************************************************************/  
/* 				Program memory code 															*/
/************************************************************************************************/ 
.section program;
Process_AD1885_Audio_Samples:

        R7.L = 0x8000;
		p0.l = tx_buf;
		p0.h = tx_buf;
		p0 += TAG_PHASE;	/* Clear all AC97 link Audio Output Frame slots */
		nop;
		nop;
		nop;
		W[P0] = R7;						/* and set Valid Frame bit in SLOT '0' TAG phase  */
        R7 = 0x0000;
        p0 += -TAG_PHASE;
		p0 += COMMAND_ADDRESS_SLOT;
		nop;
		nop;
		nop;
		W[P0] = R7;
		p0 += -COMMAND_ADDRESS_SLOT;
		p0 += COMMAND_DATA_SLOT;
		nop;
		nop;
		nop;
		W[P0] = R7;
		p0 += -COMMAND_DATA_SLOT;

Check_ADCs_For_Valid_Data:
        p0.l = rx_buf;
		p0.h = rx_buf;
		p0 += TAG_PHASE;          		/* Get ADC valid bits from tag phase slot*/
		nop;
		nop;
		nop;
		R5 = W[P0];
		R1 = 0x1800;                				/* Mask other bits in tag */
        R7 = R5 & R1;	

Set_TX_Slot_Valid_Bits:
        p0.l = tx_buf;
		p0.h = tx_buf;
		p0 += TAG_PHASE;           	/* Frame/Addr/Data valid bits */
		nop;
		nop;
		nop;
		R1 = W[P0];
		R1 = R1 | R7;            					/* Set TX valid bits based on Recieve TAG info */
		W[P0] = R1;

Check_AD1885_ADC_Left:
	
	CC = BITTST (R5, M_Left_ADC);					/* Check Left ADC valid bit */
	IF !CC JUMP Check_AD1885_ADC_Right;   			/* If valid data then save ADC sample */
	p0.l = rx_buf;
	p0.h = rx_buf;
	p0 += LEFT;						/* Get AD1885 Left channel input sample */
	nop;
	nop;
	nop;
	R1 = W[P0];
	P1.l = Left_Channel;						/* Save to data holder for processing */
	p1.h = Left_Channel;
	nop;
	nop;
	nop;
	W[P1] = R1;

Check_AD1885_ADC_Right:                        				
	CC = BITTST (R5, M_Right_ADC);					/* Check Right ADC valid bit */
	IF !CC JUMP Valid_Frame;     					/* If valid data then save ADC sample */
  	p0.l = rx_buf;
	p0.h = rx_buf;
	p0 += RIGHT;						/* Get AD1885 Right channel input sample */
	nop;
	nop;
	nop;
	R1 = W[P0];
	P0.l = Right_Channel; 						/* Save to data holder for processing */
	p0.h = Right_Channel;
	nop;
	nop;
	nop;
	W[P0] = R1;


user_dsp_applic:

/***** DSP processing is finished, now playback results to AD1885 *****/
Playback_Audio_Data:
	R6 = W[P1];		/* Fetch Left Channel */
	R7 = W[P0]; 	/* Fetch Right Channel */
		
/*****************************************************************************************************
* Insert DSP Algorithms Here                                                                         *						 					    						
*****************************************************************************************************/

	p2.l = tx_buf;
	p2.h = tx_buf;
	p2 += LEFT;
	W[P2] = R6;		/* ...output Left data to Slot 3 */		


	p2 += -LEFT;
	p2 += RIGHT;	/* ...output Right data to Slot 4 */
	W[P2] = R7;

/* ...house keeping prior to RTI */
Valid_Frame:
	RTS;
	
Process_AD1885_Audio_Samples.end:	
