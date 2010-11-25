#include <p18cxxx.inc>

BIN2BCD		UDATA_ACS
TempH 			res	1
TempL 			res 1
BCDNumDig1		res 1
BCDNumDig2 		res 1
BCDNumDig3 		res 1
BCDNumDig4 		res	1
BCDNumDig5 		res 1

	GLOBAL	BCDNumDig1, BCDNumDig2, BCDNumDig3, BCDNumDig4, BCDNumDig5 ;TempL, TempH, 
	
	EXTERN	adH,adL
	
			CODE
			
;***************************************************************************
; Function:     Bin16_2BCD													
;																			
;***************************************************************************
; Function:     Bin16_2BCD													
; PreCondition: None														
; Overview:     Converts 16 - bit binary number into BCD					
;																			
; Input:        TempL = LSB of the value to be converted					
;               TempH = MSB of the value to be converted					
; Output:       BCDNumDig1 - ASCII equivalent of 1's digit					
;               BCDNumDig2 - ASCII equivalent of 10's digit					
;               BCDNumDig3 - ASCII equivalent of 100's digit				
;               BCDNumDig4 - ASCII equivalent of 1000's digit				
;               BCDNumDig5 - ASCII equivalent of 10000's digit				
;																			
; Side Effects: Databank changed											
; Stack requirement: 1														
	
Bin16_2BCD
	GLOBAL	Bin16_2BCD
	
	movff	adL,TempL
	movff	adH,TempH
		
    banksel TempH
    swapf   TempH,W
    andlw   0x0F
    addlw   0xF0
    banksel BCDNumDig4
    movwf   BCDNumDig4
    addwf   BCDNumDig4,F
    addlw   0xE2
    movwf   BCDNumDig3
    addlw   0x32
    movwf   BCDNumDig1  

    banksel TempH
    movf    TempH,W
    andlw   0x0F
    banksel BCDNumDig3        
    addwf   BCDNumDig3,F
    addwf   BCDNumDig3,F
    addwf   BCDNumDig1,F
    addlw   0xE9
    movwf   BCDNumDig2
    addwf   BCDNumDig2,F
    addwf   BCDNumDig2,F

    banksel TempL
    swapf   TempL,W
    andlw   0x0F
    banksel BCDNumDig2
    addwf   BCDNumDig2,F
    addwf   BCDNumDig1,F

    rlcf    BCDNumDig2,F
    rlcf    BCDNumDig1,F
    comf    BCDNumDig1,F
    rlcf    BCDNumDig1,F

    banksel TempL
    movf    TempL,W
    andlw   0x0F
    banksel BCDNumDig1
    addwf   BCDNumDig1,F
    rlcf    BCDNumDig4,F

    movlw   0x07
    movwf   BCDNumDig5

    movlw   0x0A                             ; Ten
Lb1:
    decf    BCDNumDig2,F
    addwf   BCDNumDig1,F
    btfss   STATUS,C
    bra     Lb1
Lb2:
    decf    BCDNumDig3,F
    addwf   BCDNumDig2,F
    btfss   STATUS,C 
    bra     Lb2 
Lb3: 
    decf    BCDNumDig4,F 
    addwf   BCDNumDig3,F
    btfss   STATUS,C
    bra     Lb3 
Lb4: 
    decf    BCDNumDig5,F
    addwf   BCDNumDig4,F
    btfss   STATUS,C 
    bra     Lb4 
        
	swapf	BCDNumDig4,w		
	andlw	0xF0				;digit 1 & 2 are combined as well as 3 & 4
	movwf	BCDNumDig4			; this allows the the MSB to be removed which is 0 always
	movf	BCDNumDig3,w
	andlw	0x0F
	iorwf	BCDNumDig4,f
			
	swapf	BCDNumDig2,w
	andlw	0xF0
	movwf	BCDNumDig2
	movf	BCDNumDig1,w
	andlw	0x0F
	iorwf	BCDNumDig2,f
	
    retlw   0

Bin16_2BCD4LCD
	GLOBAL	Bin16_2BCD4LCD
	
	movff	adL,TempL
	movff	adH,TempH
		
    banksel TempH
    swapf   TempH,W
    andlw   0x0F
    addlw   0xF0
    banksel BCDNumDig4
    movwf   BCDNumDig4
    addwf   BCDNumDig4,F
    addlw   0xE2
    movwf   BCDNumDig3
    addlw   0x32
    movwf   BCDNumDig1  

    banksel TempH
    movf    TempH,W
    andlw   0x0F
    banksel BCDNumDig3        
    addwf   BCDNumDig3,F
    addwf   BCDNumDig3,F
    addwf   BCDNumDig1,F
    addlw   0xE9
    movwf   BCDNumDig2
    addwf   BCDNumDig2,F
    addwf   BCDNumDig2,F

    banksel TempL
    swapf   TempL,W
    andlw   0x0F
    banksel BCDNumDig2
    addwf   BCDNumDig2,F
    addwf   BCDNumDig1,F

    rlcf    BCDNumDig2,F
    rlcf    BCDNumDig1,F
    comf    BCDNumDig1,F
    rlcf    BCDNumDig1,F

    banksel TempL
    movf    TempL,W
    andlw   0x0F
    banksel BCDNumDig1
    addwf   BCDNumDig1,F
    rlcf    BCDNumDig4,F

    movlw   0x07
    movwf   BCDNumDig5

    movlw   0x0A                             ; Ten
Lb1l:
    decf    BCDNumDig2,F
    addwf   BCDNumDig1,F
    btfss   STATUS,C
    bra     Lb1l
Lb2l:
    decf    BCDNumDig3,F
    addwf   BCDNumDig2,F
    btfss   STATUS,C 
    bra     Lb2l 
Lb3l: 
    decf    BCDNumDig4,F 
    addwf   BCDNumDig3,F
    btfss   STATUS,C
    bra     Lb3l 
Lb4l: 
    decf    BCDNumDig5,F
    addwf   BCDNumDig4,F
    btfss   STATUS,C 
    bra     Lb4l 


    
    retlw	0
    
	end
