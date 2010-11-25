;********************************************************************
;*
;*               FILE PURPOSE AND DESCRIPTION
;*
;*		This file implements an Oversampling and averaging
;*		technique used to improve the S/N and number of
;*		useful bits in an A/D converter, PIC18 version.
;*
;********************************************************************
;*
;* FileName:        18ADOver.asm
;* Dependencies:
;* Processor:       PIC18xxx
;* Compiler:        MPLAB 5.70.02
;* Company:         Microchip Technology, Inc.
;
;* Software License Agreement
;
;* The software supplied herewith by Microchip Technology Incorporated
;* (the "Company") for its PICmicro® Microcontroller is intended and
;* supplied to you, the Company's customer, for use solely and
;* exclusively on Microchip PICmicro Microcontroller products. The
;* software is owned by the Company and/or its supplier, and is
;* protected under applicable copyright laws. All rights are reserved.
;* Any use in violation of the foregoing restrictions may subject the
;* user to criminal sanctions under applicable laws, as well as to
;* civil liability for the breach of the terms and conditions of this
;* license.
;
;* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
;* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
;* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
;* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
;* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
;* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;
;* Author               Date            Comment
;*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;* Caio Gubel      October 23, 2002    Initial Release (V1.0)
;*
;********************************************************************/

#include <p18cxxx.inc>

#define CLOCK_FREQ      .4000000
#define ADOVEREXTRABITS 2								; Set the Number of extra bits

#define ADOVERROUND	(0x01<<(ADOVEREXTRABITS-1))

X=(CLOCK_FREQ/.1000000)
#if (X<0x04)
    #define ADOVERLOWFREQ         
	#define	ADOVERTADWAIT   0
#else
    #define ADOVERHIGHFREQ         
	#define	ADOVERTADWAIT   (X-5)
#endif	

_ADOVERDATA       UDATA

vADOverReadLow				RES		1						
vADOverReadHigh				RES		1					

vADOverValueLow				RES		1					;used to add and average values from conversions (3 bytes)
vADOverValueHigh			RES		1					
vADOverValueUpper			RES		1					

vADOverSampleCounterLow		RES		1					;samples counters
vADOverSampleCounterHigh	RES		1					

vADOverSampleSizeHigh		RES		1					;number of samples
vADOverSampleSizeLow		RES		1					

vADOverLow					RES		1					;Higher Precision A/D Calculated Values
vADOverHigh					RES		1					
vADOverUpper				RES		1					
vADOverSaveINTCON			RES		1					;original INTCON

_vADOverAcqTime             RES     1

	GLOBAL	vADOverLow, vADOverHigh	 	

ADOverCode   CODE

;****************************************************************************
; Function: ADOverInit
;
; PreCondition: None
;
; Overview:
;       This function initializes all variables of the A/D Oversampling process
;	It takes the number of extra bits desired by the user and calculate the
;	number of samples necessary to achieve such precision --> (4**N) where
;	N is the number of extra bits.
;
; Input: None
;
; Output: None
;
; Side Effects: Databank changed
;
; Stack requirement:
;
;****************************************************************************

ADOverInit:

	GLOBAL      ADOverInit

	banksel	    ADCON0							;force the A/D to work from RC oscillator
	bsf	    	ADCON2,ADCS2
	bcf	    	ADCON2,ADCS1
	bsf	    	ADCON2,ADCS0
	bsf	    	ADCON0,ADON						;enable AD module
	banksel     ADCON2						
	bsf	    	ADCON2,ADFM						;force Right Justified Format
	
;	banksel	    PIE1			             	;enable A/D Interrupt Flag without enabling GIE!
;	bsf	    	PIE1,ADIE

	banksel	    vADOverSampleCounterLow
	movlw	    ADOVEREXTRABITS					;takes the constant with the number of extra bits
	movwf	    vADOverSampleCounterLow			;loads the counter to be used just to calculate 4**N
	clrf	    vADOverSampleSizeLow			;clear number of samples
	clrf	    vADOverSampleSizeHigh
	bsf	    	vADOverSampleSizeLow,0			;start the number of samples with one
adOverLoopPower
	bcf	    	STATUS,C
	rlcf	    vADOverSampleSizeLow,f			;for each extra bit multiply the number of samples by 4
	rlcf	    vADOverSampleSizeHigh,f
	bcf	    	STATUS,C
	rlcf	    vADOverSampleSizeLow,f
	rlcf	    vADOverSampleSizeHigh,f
	decfsz	    vADOverSampleCounterLow,f		        ;decrement bits counter
	goto	    adOverLoopPower
    movff       vADOverSampleSizeLow,vADOverSampleCounterLow    
    movff       vADOverSampleSizeHigh,vADOverSampleCounterHigh
	return

;****************************************************************************
; Function: ADOverGetData
;
; PreConditions:	-ADOverInit must have been executed.
;			-A/D channel is preselected.
;			-The necessary acquisition time was waited before calling
;			 the function.
;
;
; Overview:
;       This function gets the necessary number of samples from the A/D and
;	then averages the samples in order to obtain the higher precision
;	conversion value, which is then returned.
;
;
; Input: None
;
; Output: None
;
; Side Effects:
;              	-Databank changed.
;		-This function is blocking
;		-The part is put to sleep, waking-Up at each A/D Cycle
;		-The processing time of this function depends on the
;  		 the number of samples and RC time.
;               -During execution the ints. are blocked, restablished after completion
;
; Stack requirement:
;
;****************************************************************************

ADOverGetData:

    GLOBAL      ADOverGetData

	clrf	     vADOverValueLow			;clear auxiliar variables to prepare for the next sampling
	clrf	     vADOverValueHigh
	clrf	     vADOverValueUpper

	banksel	    INTCON                     	;Save INTCON
	movf	    INTCON,w
	banksel	    vADOverSaveINTCON
	movwf	    vADOverSaveINTCON
	banksel	    INTCON
	bcf	    	INTCON,GIE					;disable all ints. during the A/D oversampling Process
	bsf	    	INTCON,PEIE
ADOverGetNextSample
 	banksel	    ADCON0
	bsf	    	ADCON0,GO					;start the conversion process
ADOverPutSleep
;	btfsc		ADCON0,GO					;wait till conversion is over
;	bra			ADOverPutSleep
	sleep									;sleep until conversion is over
	nop											
	banksel	    PIR1                	
	bcf	    	PIR1,ADIF					;clear interrupt flag
	
#ifdef			AD8BITS						;if 8 bits A/D is used gets only one byte
	banksel	    ADRES
   	movff       ADRES,vADOverReadLow
	clrf	    vADOverReadHigh
#else										;otherwise read two bytes of data
	banksel	    ADRESL						;save conversion registers
	movf	    ADRESL,w
	banksel	    vADOverReadLow
	movwf	    vADOverReadLow
	banksel	    ADRESH
	movf	    ADRESH,w
	banksel	    vADOverReadHigh
	movwf	    vADOverReadHigh
#endif

ADOverCheckSamples
	movf	    vADOverReadLow,w			        ;add the measured value to the sum (24 bits)
	addwf	    vADOverValueLow,f
	movf	    vADOverReadHigh,w
	addwfc	    vADOverValueHigh,f
	btfsc	    STATUS,C
	incf	    vADOverValueUpper,f
    decfsz      vADOverSampleCounterLow,f    
	goto	    ADOverGetNextSample		        ;if it didn't get enough samples goto get more
	movf	    vADOverSampleCounterHigh,w
	btfsc	    STATUS,Z
	goto	    ADOverAveraging			        ;else goto average to produce the higher precision data
	decf	    vADOverSampleCounterHigh,f
	goto	    ADOverGetNextSample

ADOverAveraging
	movlw	    ADOVEREXTRABITS					;use this counter to set the number of rotates in averaging
	movwf	    vADOverSampleCounterLow
ADOverAveragingLoop
	bcf	     	STATUS,C						;ADValue/=(2^EXTRABITS)
	rrcf	    vADOverValueUpper,f
	rrcf	    vADOverValueHigh,f
	rrcf	    vADOverValueLow,f
	decfsz	    vADOverSampleCounterLow,f
	goto	    ADOverAveragingLoop
	movff       vADOverSampleSizeLow,vADOverSampleCounterLow
	movff       vADOverSampleSizeHigh,vADOverSampleCounterHigh
    movff       vADOverValueLow,vADOverLow
    movff       vADOverValueHigh,vADOverHigh
	banksel	    vADOverSaveINTCON
	movf	    vADOverSaveINTCON,w
	banksel	    INTCON
	movwf	    INTCON
	return

	end
