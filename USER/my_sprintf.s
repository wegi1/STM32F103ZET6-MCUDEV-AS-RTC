
    .syntax unified
    .cpu cortex-m3
    .fpu softvfp
    .thumb

    .thumb_func
    .global my_htoa32
	.global my_itoa
	.global my_utoa
	.type   my_htoa32, %function
    .type   my_itoa, %function
	.type   my_utoa, %function

    .text
//----------------

//======================================================
// HEX 32BIT value convert to string
//====================================
       // INPUT:
       //    R0, =  ; POINTER TO OUTPUT char DATA BUFFER
       //    R1, =  ; VALUE TO HEXADECIMAL CONVERT

       // output: no output, data in buffer are output
//=======================================================
//=========================
my_htoa32:

        MOV    R3, 0x7830     // "0x"
        STRH   R3, [R0]       // STORE "0x" TO BUFFER
        MOVS   R3, 2          // output data 2 bytes after start buffer
//--
LP01:
        LSRS   R2, R1, #28    // R2 = OUR DATA TO STRING CONVERT 1 BYTE = NYBBLE 0..F
        LSLS   R1, R1, #4     // R1 * 16 = R1 << 4
        ADDS   R2, R2, #0x30  // CONVERT TO NUMBER
        CMP    R2, #0x3A
        BCC.N  LP01A          // if data < than 0x3a don't  add #7
        ADDS   R2,R2, 0x07    // CONVER TO LETTER
LP01A:
        STRH   R2, [R0, R3]   // STRH store byte and zero terminated
        ADDS   R3, R3, #1
        CMP    R3, #10
        BNE.N  LP01
//---
        BX     LR
//=============================================================
//========================================================
// DECIMAL CONVERSION OF ANY SIGNED VALUE UP TO 32 BIT
//========================================================
       // INPUT:
       //    R0, =  ; POINTER TO OUTPUT char DATA BUFFER
       //    R1, =  ; VALUE TO DECIMAL CONVERT


       // OUTPUT: R0 = LENGTH OF PRODUCED STRING BYTES
//========================================================
my_itoa:
      CMP   R1, 0       // IS OUR VALUE NEGATIVE?
      BGE.N my_utoa     // IF IS POSSITIVE GO TO my_sprintf
      PUSH  {LR}        // PC address go back to the main function

      MOVS  R2, '-'
      STRB  R2, [R0]    // STORE "-" SIGN AT THE START TEXT BUFFER
      ADDS  R0, R0, #1  // MOVE UP 1 BYTE POINTER TO OUTBUFFER

      NEG   R1, R1      // CHANGE VALUE TO POSSITIVE NUMBER
      BL    my_utoa     // CONVERT VALUE TO DECIMAL STRING

                        // now in R0 is datalength from my_sprintf
      ADDS  R0, R0, #1  // INCREASSE LENGTH OF STRING MORE 1 BYTE OF SIGN "-"
      POP   {PC}        // GO BACK TO THE MAIN FUNCTION
//======================================================

//========================================================
// DECIMAL CONVERSION OF ANY UNSIGNED VALUE UP TO 32 BIT
//========================================================
       // INPUT:
       //    R0, =  ; POINTER TO OUTPUT char DATA BUFFER
       //    R1, =  ; VALUE TO DECIMAL CONVERT


       // OUTPUT: R0 = LENGTH OF PRODUCED BYTES OF STRING
//========================================================
my_utoa:

        PUSH {R4,R5,LR}  // store registers


        MOV   R5, #0  // LENGTH OF STRING
        STR   R5, [R0] // CLEAR BUFFER
        STR   R5, [R0, #4]
        STR   R5, [R0, #8]
CALC01:
        //------------------
        //- R4 = R1 DIV 10 -
        //------------------
        LDR   R3, = 0xCCCCCCCD // MAGIC VALUE (!!!)
        UMULL R4,R3,R3,R1      // this three lines looks a bit strange
        LSRS  R4,R3, #3        // but here we gotta divide by 10 (seriously!)
        //----------------------------------------------
        MOVS  R2, R4         // R2 = R1 div by 10 without the rest
        MOVS  R3, #0x0A      // R3 = 10
        MUL   R4, R4, R3     // R4 = R4 * 10

        // mod 10 calculate
        // R4 = R1 mod 10
        SUBS  R4, R1, R4     // CALCULATE THE REST r4 = r1 - r4

        ORR   R4, R4, #0x30  // CHANGE TO ASCII NUMBER VALUE "0..9"
        STRH  R4, [R0,R5]    // store next decimal row and ZERO terminate string
        ADDS  R5, R5, #1     // R5 = length of string
        MOV   R1, R2         // R1 = before stored value in R2 = (R1 div 10)
        CMP   R1, #0         // R1 = 0? (that was last one operation?)
        BNE.N CALC01         // if R1 != 0 then continue CALC01 loop

        PUSH  {R5}           // TEMPORARY STORE LENGTH OF STRING
        SUBS  R5, R5, #1     // SET OFFSET AT THE END OF STRING (BACKWARD POSSITION)

        // R0 = POINTER TO OUTPUT NULL TERMINATED STRING
        // R5 = OFFSET TO THE END OF STRING (BACKWARD POSSITION)
        // R1 = OFFSET TO THE START OF STRING (FORWARD POSSITION) R1 = 0 AT THE END CALC01 ROUTINE
        // R4 = BACKWARD BYTE (BYTE FROM "RIGHT SIDE")
        // R2 = FORWARD  BYTE (BYTE FROM "LEFT  SIDE")
CALC02:
        LDRB  R4, [R0, R5] // GET DATA FROM THE END (FROM RIGT SIDE)
        LDRB  R2, [R0, R1] // GET DATA FROM THE START (LEFT SIDE)
        STRB  R2, [R0, R5] // GET DATA FROM THE "LEFT  SIDE INTO THE RIGHT SIDE"
        STRB  R4, [R0, R1] // GET DATA FROM THE "RIGHT SIDE INTO THE LEFT  SIDE"
        ADDS  R1, R1, #1   // ACTUALIZE STRING FORWARD POSSITION
        SUBS  R5, R5, #1   // ACTUALIZE STRING BACKWARD POSSITION
        CMP   R5, R1       // R5 =< R1 ?
        BEQ.N END_CALC     // if R5 = R1 go to finish
        BGT.N CALC02       // if R5 > R1 continue loop - otherway finish (R5 < R1)

END_CALC:
        // acording declaration this functin is "int" and
        // acordind AAPCS should be returned int value in R0
        // simple "return datalength;" in C
        // OUTPUT: R0 = LENGTH OF PRODUCED BYTES OF STRING R0 = R5

        POP {R0, R4, R5, PC}        // restore registers // ADIOS :)
        // OUTPUT: R0 = LENGTH OF PRODUCED STRING BYTES
//=====================================================
