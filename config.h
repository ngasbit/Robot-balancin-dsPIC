
// DSPIC33FJ32MC302 Configuration Bit Settings

// 'C' source line config statements

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
#pragma config IESO = ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR1             // POR Timer Value (1ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
#pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = ON              // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<libpic30.h>
#include <xc.h>
#define Speed 40     //MIPS deseados (maximo 40 para el dspic33fj32mc204)
#define Crystal_Externo 20   //valor del cristal en MHZ       
#define Freq Speed*1000000 //40Mhz
#define delay_ms(x) __delay32((Freq/1000)*x) //delay en milisegundos
#define delay_us(x) __delay32(Speed*x) //delay en microsegundos

void PLL(void){
    PLLFBD = 41;
    CLKDIVbits.PLLPOST=0; 
    CLKDIVbits.PLLPRE=0; 
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    while (OSCCONbits.COSC != 0b001);
    while(OSCCONbits.LOCK != 1) {};
}

void initIO(void){
    AD1PCFGL = 0xffff;
    TRISBbits.TRISB15 = 0; //Motor Derecho+
    TRISBbits.TRISB13 = 0; //Motor Derecho-
    TRISBbits.TRISB4  = 0; //Motor Izquierdo+
    TRISBbits.TRISB5  = 0; //Motor Izquierdo-
    TRISAbits.TRISA4  = 0; //Motor Izquierdo-
    
    TRISAbits.TRISA0 = 1; //Motor Derecho-
    TRISAbits.TRISA1  = 1; //Motor Izquierdo+
    TRISBbits.TRISB2  = 1; //Motor Izquierdo-
    TRISAbits.TRISA2  = 1; //Motor Izquierdo-
    
    
    
    
}

void init_PWM(void){
    P1TCONbits.PTMOD = 0b00; // PWM de forma continua
    P1TCONbits.PTCKPS = 0b11; // PWM time base input clock period is TCY (1:1 prescale) 
    P1TCONbits.PTOPS = 0b00; // PWM time base output post scale is 1:1 
    P1TMRbits.PTMR = 0;//base de tiempo en bits 
    P1TPER = 414;
    PWM1CON1bits.PMOD1 = 1; // 0 Activa inversion en PW1
    PWM1CON1bits.PMOD2 = 1; // 0 Activa inversion en PW2
    PWM1CON1bits.PEN1H = 1; // Activa el PWM1H1
    PWM1CON1bits.PEN2H = 1; // Activa el PWM1H2
    PWM1CON1bits.PEN1L = 0; // Activa el PWM1L1
    PWM1CON1bits.PEN2L = 0; // Activa el PWM1L2
    PWM1CON2bits.IUE = 1;
    PWM1CON2bits.UDIS = 0;
    P1TCONbits.PTEN = 1; // 1 Activa pulso de generacion PWM
    P1DC1 = 0; // 1000 50% duty
    P1DC2 = 0; 
}
