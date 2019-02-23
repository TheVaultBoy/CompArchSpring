****************************************************************************

ADSP-BF535 EZ-KIT Lite Assembly Based AD1885 TALKTHRU

Analog Devices, Inc.
DSP Division
Three Technology Way
P.O. Box 9106
Norwood, MA 02062

Date Created:	1/30/02

_______________________________________________________________________________

This directory contains an example ADSP-BF535 subroutine that demonstrates the 
initialization of the AC'97 communication link between SPORT0 on the ADSP-BF535 
and the AD1885 stereo Codec.  
This is done by initializing the link and implementing a simple talk-through 
routine.  

Files contained in this directory:

readme.txt						this file
BF535_EZKIT_TALKTHROUGH.asm		assembly file for the talkthrough program
AD1885_Init.asm					assembly file for the codec initialization routines
Processing_ISR.asm				assembly file for processing the interrupt service routine
Blackfin_Talkthrough.dpj		VisualDSP++ project file
Blackfin_Talkthrough.dxe		executable for this project

__________________________________________________________


CONTENTS

I.	FUNCTIONAL DESCRIPTION
II.	IMPLEMENTATION DESCRIPTION
III.	OPERATION DESCRIPTION


I.    FUNCTIONAL DESCRIPTION

The Talkthru demo demonstrates the initialization of SPORT to establish
an AC'97 link between the ADSP-BF535 and the AD1885 Codec.
 
This example simply exercises the SPORT0 interface.  SPORT0 is setup as a 
Slot-16 AC'97 interface to communicate with the AD1885.  Audio samples received
from the AD1885 are moved into the DSP's receive buffer, using DMA.  The samples are processed
by the ADSP-BF535 and place in the transmit buffer.  In turn, the transmit buffer is 
used to transmit data to the AD1885.  This results in a simple
talk-through where data is moved in and out of the DSP with out performing
any computations on the data.


II.   IMPLEMENTATION DESCRIPTION

The Initialization module initializes:

1. The transmit and receive Multi-channel configuration. 
2. The transmit and receive DMA control registers and Auto-buffer registers.
3. The SPORT0 control registers.
4. The AD1885 Codec by writing to its control registers. 


III.  OPERATION DESCRIPTION

- Open the project "BLACKFIN_TALKTHROUGH.dpj" in the VisualDSP Integrated Development Environment (IDDE).
- Under the "Project" tab, select "Build Project".
- Open a ADSP-BF535 EZ-KIT Lite session in the VisualDSP Debugger.
- Load "BLACKFIN_TALKTHROUGH.DXE" into the ADSP-BF535.
- Setup an input source (such as a radio) to the "LINE IN" and an output source (such as
  headphones) to the "LINE OUT" connectors of the EZ-KIT Lite.  See the ADSP-BF535 EZ-KIT Lite
  User's Manual for more information on setting up the hardware.
- Select "RUN" from the "Debug" tab on the menu bar.
- Listen to the operation of the talk-through.



