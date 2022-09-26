#include <xc.h>
#include <libpic30.h>// liberia para el delay
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void init_motor(int L, int R ){
    P1DC1 = L;
    P1DC2 = R; 
    LATBbits.LATB13 = 0; // AIN1
    LATBbits.LATB15 = 0; // AIN2
    LATBbits.LATB4  = 0; // BIN1
    LATBbits.LATB5  = 0; // BIN2
}

void CCW(int L, int R){
    P1DC1 = L;
    P1DC2 = R;
    LATBbits.LATB13 = 0; // AIN2
    LATBbits.LATB15 = 1; // AIN1
    LATBbits.LATB4  = 1; // BIN1
    LATBbits.LATB5  = 0; // BIN2
}

void CW(int L, int R){
    P1DC1 = L;
    P1DC2 = R; 
    LATBbits.LATB13 = 1; // AIN1
    LATBbits.LATB15 = 0; // AIN2
    LATBbits.LATB4  = 0; // BIN1
    LATBbits.LATB5  = 1; // BIN2
}

void STOP_MOTOR(){
    P1DC1 = 0;
    P1DC2 = 0;
    LATBbits.LATB13 = 0; // AIN1
    LATBbits.LATB15 = 0; // AIN2
    LATBbits.LATB4  = 0; // BIN1
    LATBbits.LATB5  = 0; // BIN2
}