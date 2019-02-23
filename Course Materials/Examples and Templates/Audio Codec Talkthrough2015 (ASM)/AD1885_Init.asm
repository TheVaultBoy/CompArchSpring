/*****************************************************************************************************************

(C) Copyright 2001 - Analog Devices, Inc.  All rights reserved.

File Name:		AD1885_Init.asm

Date Modified:	10/1/01		J.B		Rev 0.1
				
Purpose:	   	AD1885/ADSP-BF535 SPORT0 Initialization Driver 			
			   	Developed using the ADSP-BF535 EZ-Kit Lite Evaluation Platform		
				
				Demonstrate initialization and operation of a SPORT0-AC'97 link between the ADSP-BF535 
			   	and sets up AD1885 to send/recieve data at 48 KHz using the SPORT0 rx ISR
				for audio processing.
				
*****************************************************************************************************************/	
#include 	<defBF535.h>

/************************************************************************************************/
/*			   GLOBAL DECLARATIONS																*/	
/************************************************************************************************/ 
.SECTION	data1;

.GLOBAL 	Codec_Reset;			  
.GLOBAL		Program_SPORT0_Registers;
.GLOBAL		Program_DMA_Controller;
.GLOBAL		AD1885_Codec_Initialization;


 
/************************************************************************************************/
/*			     CONSTANT & MACRO DEFINITIONS													*/	
/************************************************************************************************/ 
/*			register				address	*/
/*         --------------		----------- */
.SECTION 	data1;
#define		REGS_RESET				0x0000
#define		MASTER_VOLUME			0x0200
#define		HEAD_VOLUME				0x0400
#define		MASTER_VOLUME_MONO		0x0600	
#define		RESERVED_REG_2			0x0800
#define		PC_BEEP_Volume			0x0A00
#define		PHONE_Volume			0x0C00
#define		MIC_Volume				0x0E00
#define		LINE_IN_Volume			0x1000
#define		CD_Volume				0x1200
#define		VIDEO_Volume			0x1400
#define		AUX_Volume				0x1600
#define		PCM_OUT_Volume			0x1800
#define		RECORD_SELECT			0x1A00
#define		RECORD_GAIN				0x1C00
#define		RESERVED_REG_3			0x1E00
#define		GENERAL_PURPOSE			0x2000
#define		THREE_D_CONTROL_REG		0x2200
#define		RESERVED_REG_4			0x2400
#define		POWERDOWN_CTRL_STAT		0x2600
#define		EXTEND_AUDIO_ID			0x2800
#define		EXTEND_AUDIO_CTL		0x2A00
#define		SAMPLE_RATE_GENERATE_1	0x2C00
#define		SAMPLE_RATE_GENERATE_0	0x3200
#define 	JACK_SENSE				0x7200
#define		SERIAL_CONFIGURATION	0x7400
#define		MISC_CONTROL_BITS		0x7600
#define		VENDOR_ID_1				0x7C00
#define		VENDOR_ID_2				0x7E00

/* Mask bit selections in Serial Configuration Register for 
   accessing registers on any of the 3 codecs */
#define		MASTER_Reg_Mask			0x1000
#define		SLAVE1_Reg_Mask			0x2000
#define		SLAVE2_Reg_Mask			0x4000
#define		MASTER_SLAVE1			0x3000
#define		MASTER_SLAVE2			0x5000
#define		MASTER_SLAVE1_SLAVE2	0x7000

/* Macros for setting Bits 15, 14 and 13 in Slot 0 Tag Phase */
#define		ENABLE_VFbit_SLOT1_SLOT2	0xE000
#define		ENABLE_VFbit_SLOT1			0xC000
#define 	ENABLE_VFbit_STEREO			0x9800

/* TDM Timeslot Definitions */
#define		TAG_PHASE				0
#define		COMMAND_ADDRESS_SLOT	2
#define		COMMAND_DATA_SLOT		4
#define		STATUS_ADDRESS_SLOT		2
#define		STATUS_DATA_SLOT		4
#define		LEFT					6
#define		RIGHT					8

/*Uncomment the MIC macro if you are using MIC IN jumper settings on EZ-KIT*/
/*Leave the MIC macro commented if you are using LINE IN jumper settings on EZ-KIT (default setting)*/
//#define		MIC

/************************************************************************************************/
/* 				DM data 																		*/
/************************************************************************************************/
.SECTION    data1;
.align 2;
.byte2 	rx_buf[16]; 		/* receive buffer */
.GLOBAL rx_buf;
.align 2;							/* transmit buffer */ 
.byte2 	tx_buf[16] = 	ENABLE_VFbit_SLOT1_SLOT2, SERIAL_CONFIGURATION, 0x9900, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
						0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000;
.GLOBAL tx_buf;

#ifdef	MIC
.align 2;

/***********************************/
/* Codec Register Initializations */
/* - refer to AD1885 Data Sheet for register bit assignments */
//The following set of parameters is used for MIC IN jumper configuration on EZ-KIT
//Make sure the jumper configurations JP1 is set for MIC IN as this is not the default configuration
.byte2 Init_Codec_Registers[50] =       // these settings are for mic in
	REGS_RESET,				0xFFFF,
	MASTER_VOLUME,			0x0000,		/* Unmute Master Volume */
	HEAD_VOLUME,			0x0000,		/* Adjusted Headphone Vloume */
	MASTER_VOLUME_MONO,		0x8000,	
	PC_BEEP_Volume,			0x8000,
	PHONE_Volume,			0x8008,
	MIC_Volume,				0x8048,  //8008
	LINE_IN_Volume,			0x8808,
	CD_Volume,				0x8808,
	VIDEO_Volume,			0x8808,
	AUX_Volume,				0x8808,
	PCM_OUT_Volume,			0x0F0F,		/* Unmuted and attenuate by 10.5 dB */
	RECORD_SELECT,			0x0000,	//0404	/* Selected MIC IN as Record Select */
	RECORD_GAIN,			0x0a0a, //0000	/* Unmuted Record Gain */
	GENERAL_PURPOSE,		0x0000,		
	THREE_D_CONTROL_REG,	0x0000,
	POWERDOWN_CTRL_STAT,	0x0000,
	EXTEND_AUDIO_ID,		0x0001,
	EXTEND_AUDIO_CTL,		0x0001,
	SAMPLE_RATE_GENERATE_1,	0xBB80, //3b80		/*Sample Rate 14.875 KHz*/
	SAMPLE_RATE_GENERATE_0,	0xBB80, //3b80
	JACK_SENSE,				0x0000,
	MISC_CONTROL_BITS,		0x0404,
	VENDOR_ID_1,			0x4144,
	VENDOR_ID_2,			0x5360;
#else
//The following set of parameters is used for LINE IN jumper configuration on EZ-KIT (default)
.align 2;
.byte2 Init_Codec_Registers[50] =
	REGS_RESET,				0xFFFF,
	MASTER_VOLUME,			0x0000,		/* Unmute Master Volume */
	HEAD_VOLUME,			0x0000,		/* Adjusted Headphone Vloume */
	MASTER_VOLUME_MONO,		0x8000,	
	PC_BEEP_Volume,			0x8000,
	PHONE_Volume,			0x8008,
	MIC_Volume,				0x8008,
	LINE_IN_Volume,			0x8808,
	CD_Volume,				0x8808,
	VIDEO_Volume,			0x8808,
	AUX_Volume,				0x8808,
	PCM_OUT_Volume,			0x0F0F,		/* Unmuted and attenuate by 10.5 dB */
	RECORD_SELECT,			0x0404,		/* Selected LINE IN as Record Select */
	RECORD_GAIN,			0x0000,		/* Unmuted Record Gain */
	GENERAL_PURPOSE,		0x0000,		
	THREE_D_CONTROL_REG,	0x0000,
	POWERDOWN_CTRL_STAT,	0x0000,
	EXTEND_AUDIO_ID,		0x0001,
	EXTEND_AUDIO_CTL,		0x0001,
	SAMPLE_RATE_GENERATE_1,	0x3B80,		/*Sample Rate 14.875 KHz*/
	SAMPLE_RATE_GENERATE_0,	0x3B80,
	JACK_SENSE,				0x0000,
	MISC_CONTROL_BITS,		0x0404,
	VENDOR_ID_1,			0x4144,
	VENDOR_ID_2,			0x5360;
#endif
.align 2;
.byte2 Codec_Init_Results[50]; 	
.align 2;
.byte2 RX_Status = 0x8000;				/* RX Processing flag */
.GLOBAL RX_Status;


.SECTION 	program;
/************************************************************************************************/
/* AD1885 HARDWARE RESET																		*/
/* Toggle flag_15 to assert and deassert 1885_RESET											 	*/
/************************************************************************************************/

Codec_Reset:


	P0.L = FIO_DIR & 0xFFFF;
	P0.H = (FIO_DIR >> 16) & 0xFFFF;
	R0.L = W[P0];
	BITSET(R0,15);					/* Configure PF I/O pin 15 as output */
	BITSET(R0,3);
	BITSET(R0,2);
	BITSET(R0,1);
	BITSET(R0,0);
	W[P0] = R0.L; ssync;

	P1.L = FIO_FLAG_S & 0xFFFF;				/* Initialize Output PFx Pins OFF */
	P1.H = (FIO_FLAG_S >> 16) & 0xFFFF;
	R0.L = 0x800C;
	W[P1] = R0.L; ssync;

	P5.L = 0x0000;		/* So we can see the lights blink */
	P5.H = 0x1000;
	LOOP blink_LEDs LC0 = P5;	/* Duration of 1 usec */
	LOOP_BEGIN blink_LEDs;
	nop;
	LOOP_END blink_LEDs;

	P1.L = FIO_FLAG_C & 0xFFFF;				/* Assert AD1885 /RESET (active low) */
	P1.H = (FIO_FLAG_C >> 16) & 0xFFFF;
	R0.L = 0x800B;
	W[P1] = R0.L; ssync;

	P5 = 0x00000140;		    /* AD1885 /Reset Active low for 1 usec assuming 160Mhz DSP Core CLK */
	LOOP hold_reset LC0 = P5;	/* Duration of 1 usec */
	LOOP_BEGIN hold_reset;
	nop;
	LOOP_END hold_reset;

	P2.L = FIO_FLAG_S & 0xFFFF;
	P2.H = (FIO_FLAG_S >> 16) & 0xFFFF;
	R0.L = 0x8004;				/* De-assert AD1885 /RESET and turn-on PF_2 (LED_10)*/
	W[P2] = R0.L; ssync;
		
	P5.L = 0xD480;	/* 170 usec to allow the Codec to recover from Reset */
	P5.H = 0x3F00;
	LOOP reset_recovery_delay LC0 = P5;
	LOOP_BEGIN reset_recovery_delay;
	nop;
	LOOP_END reset_recovery_delay;


	RTS;
Codec_Reset.end:	
	
/************************************************************************************************/
/* SPORT0 CONTROL REGISTER PROGRAMMING															*/
/* Multichannel Mode, Autobuffer DMA , Early FS, Active HI FS, Fall Edge, Data=16				*/
/************************************************************************************************/

Program_SPORT0_Registers:
	P0.H = (IMASK >> 16) & 0xFFFF;
	P0.L = IMASK & 0xFFFF;
	R2.L = W[P0];
	R0.L = 0x0100;
	R0 = R2 | R0;						/* Clear SPORT0 RX mask bit */
	W[P0] = R0.L;
	csync;
	CLI R0;								/* Disable all interrupts */
	R1.L = 0x0000;
	[P0] = R1;								/* Initially no interrupts */

/* RESET SPORT0 & DMA */
	P0.H = (SPORT0_TX_CONFIG >> 16) & 0xFFFF;
	P0.L = SPORT0_TX_CONFIG & 0xFFFF;
	R1.L = 0x0000;
	W[P0] = R1.L; ssync;
  	P0.L = SPORT0_RX_CONFIG &0xFFFF;
	W[P0] = R1.L; ssync;
  	P0.L = SPORT0_MCMC1 & 0xFFFF;
	W[P0] = R1.L; ssync;				
	
	P0.L = SPORT0_IRQSTAT_RX & 0xFFFF;
	R1.L = 0x0003;
	W[P0] = R1.L; ssync;
	P0.L = SPORT0_IRQSTAT_TX & 0xFFFF;
	W[P0] = R1.L;  ssync;

/* Set SPORT0 frame sync divisor */
  	P0.L = SPORT0_RFSDIV & 0xFFFF;			/* AC'97 48Khz Frame Sync with 12.288Mhz Input Clock */
	R1.L = 0x00FF;
	W[P0] = R1.L; ssync;

/* Set SPORT0 frame sync divisor */
  	P0.L = SPORT0_TFSDIV & 0xFFFF;			/* AC'97 48Khz Frame Sync with 12.288Mhz Input Clock */
	R1.L = 0x00FF;
	W[P0] = R1.L; ssync;


	P0.L = SPORT0_RSCLKDIV & 0xFFFF;			/* AC'97 48Khz Frame Sync with 12.288Mhz Input Clock */
	R1.L = 0x0005;
	W[P0] = R1.L; ssync;

/* Set: Internal RFS, 16-bits receive, External clock */
  	P0.L = SPORT0_RX_CONFIG & 0xFFFF;
	R1 = 0x03E0;
	W[P0] = R1; ssync;

/* Set: 16-bits transmit */
  	P0.L = SPORT0_TX_CONFIG & 0xFFFF;
	R1 = 0x01E0;
	W[P0] = R1; ssync;

/* Enable MCM Transmit and Receive Channels */
  	P0.L = SPORT0_MTCS0 & 0xFFFF;							/* Enable 16 Channels for TX */
	R1.L = 0xFFFF;
	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS1 & 0xFFFF;
	R1.L = 0x0000;							/* ...disable the remaining channels */								
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS1 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS2 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS3 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS4 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS5 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS6 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MTCS7 & 0xFFFF;
  	W[P0] = R1.L; ssync;

  	P0.L = SPORT0_MRCS0 & 0xFFFF;							/* Enable 16 Channels for RX */
	R1.L = 0xFFFF;
	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS1 & 0xFFFF;
	R1.L = 0x0000;							/* ...disable the remaining channels */								
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS1 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS2 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS3 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS4 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS5 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS6 & 0xFFFF;
  	W[P0] = R1.L; ssync;

	P0.L = SPORT0_MRCS7 & 0xFFFF;
  	W[P0] = R1.L; ssync;
	

/* Set MCM Configuration Reg 1 - MCM enalbled , 1 frame delay, Win size = 16 Channels */
  	P0.L = SPORT0_MCMC1 & 0xFFFF;
	R1.L = 0x0023;
	W[P0] = R1.L; ssync;

/* Set MCM Configuration Reg 2 */
  	P0.L = SPORT0_MCMC2 & 0xFFFF;
	R1.L = 0x000C;
  	W[P0] = R1.L; ssync;

/* SPORT0 Interrupts Unmasked */
//Check revision of silicon 
	P0.L = 0x48C0;
	P0.H = 0xFFC0;
	R1 = [P0];
	R2.H = 0xf000;
	R2.L = 0x0000;
	R1 = R1 & R2;

	CC=AZ;
	if !CC jump new;
old:
//Unmask the PF Interrupt A in System Interrupt Mask Register
	p0.l = SIC_IMASK & 0xffff;
	p0.h = SIC_IMASK >> 16;
	R1 = [p0];
	BITCLR(R1,4);
	[ P0 ] = R1;
	RTS;
new:
	p0.l = SIC_IMASK & 0xffff;
	p0.h = SIC_IMASK >> 16;
	R1 = [p0];
	BITSET(R1,4);   // any rev of 1.0 or later will have system interrupts enabled by writing a 1 to the appropriate
	[ P0 ] = R1;     // bit
	RTS;

Program_SPORT0_Registers.end:

/************************************************************************************************/
/* 			DMA Controller Programming For SPORT0 			    								*/
/************************************************************************************************/

Program_DMA_Controller:	

/* SPORT0 DMA AUTOBUFFER XMIT */
	P0.L = SPORT0_CONFIG_DMA_TX & 0xFFFF; 						/* Set Autobuffer */
	R3.L = 0x0010;
	W[P0] = R3.L; ssync;
  	
	P0.L = SPORT0_START_ADDR_LO_TX & 0xFFFF;
	p1.h = tx_buf;
	p1.L = tx_buf;						/* SPORT0 TX DMA Internal Memory Address */
	r3 = p1;
	W[P0] = r3.L; ssync;

	P0.L = SPORT0_START_ADDR_HI_TX & 0xFFFF;
	W[P0] = r3.H; ssync;

	P0.L = SPORT0_COUNT_TX & 0xFFFF;
	R3 = LENGTH(tx_buf);				/* SPORT0 TX DMA Internal Memory Count */
	W[P0] = R3; ssync;

/* SPORT0 DMA AUTOBUFFER RCV */
	P0.L = SPORT0_CONFIG_DMA_RX & 0xFFFF; 						/* Set Autobuffer */
	R3.L = 0x0010;
	W[P0] = R3.L;   ssync;
  	
	P0.L = SPORT0_START_ADDR_LO_RX & 0xFFFF;
	p1.L = rx_buf;						/* SPORT0 RX DMA Internal Memory Address */
	p1.h = rx_buf;
	r3 = p1;
	W[P0] = r3.L; ssync;

	P0.L = SPORT0_START_ADDR_HI_RX & 0xFFFF;
	W[P0] = r3.H; ssync;

	P0.L = SPORT0_COUNT_RX & 0xFFFF;
	R3.L = LENGTH(rx_buf);				/* SPORT0 RX DMA Internal Memory Count */
	W[P0] = R3.L; ssync;
  	

/* ENABLE SPORT0 DMA and DIRECTION IN DMA CONFIGURATION REGISTER */
	P0.L = SPORT0_CONFIG_DMA_TX & 0xFFFF;
	R3.L = 0x0011;						/* Enable TX DMA */
	W[P0] = R3.L; ssync;

	P0.L = SPORT0_CONFIG_DMA_RX & 0xFFFF;
	R3.L = 0x0017;						/* Enable RX DMA, RX Direction (mem write), RX Interrupts */
	W[P0] = R3.L; ssync;

	RTS;
	
Program_DMA_Controller.end:	

/************************************************************************************************/
/* 				AD1885 Codec Initialization 			  	  									*/
/************************************************************************************************/

AD1885_Codec_Initialization:

	P0.L = SPORT0_RX_CONFIG & 0xFFFF;	/* Enable SPORT0 RX */
	R3.L = W[P0];
	BITSET (R3, 0);
	W[P0] = R3.L;  ssync;

	P0.L = SPORT0_TX_CONFIG & 0xFFFF;	/* Enable SPORT0 TX */
	R3.L = W[P0];
	BITSET (R3, 0);
	W[P0] = R3.L;  ssync;

 	STI R0;							/* Enable Interrupts */
	ssync; nop; nop;

	idle; ssync;								/* Wait for a few RX interrupts to ensure SPORT is up and running */
	idle; ssync;
 


	R3.L = 0x8000;
  	R2.h = 0x0000;
  	R2.l = 0x0000;
	R3.h = 0x0000;

Wait_Codec_Ready:
	idle; ssync;
	P0.l = rx_buf;
	p0.h = rx_buf;
  	R4.L = W[P0];				/* Get status bit 15 from AD1885 Tag - SLOT '0' */
	R4.h = 0x0000;

	R2 = R3 & R4;
	CC = AZ;
	if CC jump Wait_Codec_Ready;
	
	R5.L = 0x020C;						/* Assert PF 3 (LED 11 on 2191 EZKIT) indicating codec-DSP communication established  */
	P0.L = FIO_FLAG_S & 0xFFFF;
	p0.h = (FIO_FLAG_S >> 16) & 0xFFFF;
	W[P0] = R5.L;



/* Write Various registers of AD1885 to configre it - will read back these same registers to ensure they are properly configured */
Initialize_1885_Registers:
	P0.l = Init_Codec_Registers;
	P0.h = Init_Codec_Registers;
	i3 = p0;      				/* Pointer to codec initialization commands */
	l3 = 0;

	P5 = 25;
	R6.L = ENABLE_VFbit_SLOT1_SLOT2;
	p0.h = tx_buf;
	p0.l = tx_buf;
	p0 += TAG_PHASE;
	nop;
	nop;
	nop;
	W[P0] = R6.L;					/* Set 'Valid Slot' bits in TAG phase for SLOTS '0', '1' , '2' */
	LOOP Codec_Init LC0 = P5;
	LOOP_BEGIN Codec_Init;
		P0.l = Init_Codec_Registers;
		P0.h = Init_Codec_Registers;
		nop;
		nop;
		nop;
		R6.L = W[i3++];                			/* Fetch next AD1885 register address */
		p1.l = tx_buf;
		p1.h = tx_buf;
		p1 += COMMAND_ADDRESS_SLOT;
		nop;
		nop;
		W[P1] = R6;	/* ...put fetched codec register address into TX SLOT '1' */
		R6.L = W[i3++];							/* Fetch register data contents */
		p1 += -COMMAND_ADDRESS_SLOT;
		p1 += COMMAND_DATA_SLOT;
		nop;
		nop;
		nop;
		W[P1] = R6;		/* ...put fetched codec register data into TX SLOT '2' */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		idle; ssync; 
		idle; ssync;									/* ...wait for 2 TDM frames... */
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		LOOP_END Codec_Init;

/* Verify AD1885 register writes */
verify_reg_writes:
	p0.l = Init_Codec_Registers;
	p0.h = Init_Codec_Registers;
	i3 = p0;
	l3 = 0;
	p0.l = Codec_Init_Results;
	p0.h = Codec_Init_Results;
	i2 = p0;
	l2 = 0;

	P5 = 25;
	R6.L = ENABLE_VFbit_SLOT1;
	p0.l = tx_buf;
	p0.h = tx_buf;
	p0 += TAG_PHASE;					
	nop;
	nop;
	nop;
	W[P0] = R6.L;					/* Set 'Valid Slot' bits in TAG phase for SLOTS '0', '1' */
	LOOP ad1885_register_status LC0 = P5;
	LOOP_BEGIN ad1885_register_status;
		R4.L = W[i3++];				/* Get next AD1885 register address that is to be read */
		i3 += 2;
		R3.L = 0x8000;								/* Set bit #15 to signify AC'97 'Read-Request' in Command address word */
		R4 = R4 | R3;
		p1.l = tx_buf;
		p1.h = tx_buf;
		p1 += COMMAND_ADDRESS_SLOT;
		nop;
		nop;
		nop;
		W[P1] = R4.L;		/* TX value out of Command address time slot */
		idle; ssync; 										/* Wait for 5 AC'97 frames to go by, latency in getting data from AD1885 */
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
		idle; ssync;
 	    p0.l = rx_buf;
		p0.h = rx_buf;
		p0 += STATUS_ADDRESS_SLOT;
		nop;
		nop;
		nop;
		R4 = W[P0];
		w[i2++] = R4.L;
		p0 += -STATUS_ADDRESS_SLOT;
		p0 += STATUS_DATA_SLOT;
		R4 = W[P0];		
 		w[i2++] = R4.L;								/* Store to results buffer */
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	LOOP_END ad1885_register_status;

	RTS; 					/* End of AD1885 Initialization */
	
AD1885_Codec_Initialization.end:	
/************************************************************************************************/

