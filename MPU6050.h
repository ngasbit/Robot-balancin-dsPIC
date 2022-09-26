#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<libpic30.h>
#include "xc.h"

#define MPU6050_ADDR 0xD0
#define MPU6050_READ 0xD1
#define CONFIG 0x1A

#define SMPLRT_DIV_REG     0x19
#define GYRO_CONFIG_REG    0x1B
#define ACCEL_CONFIG_REG   0x1C
#define TEMP_OUT_H_REG     0x41
#define PWR_MGMT_1_REG     0x6B
#define WHO_AM_I_REG       0x75


// Accelerometer DATA OUT REGISTERS.

#define ACCEL_XOUT_H       0x3B
#define ACCEL_XOUT_L       0x3C
#define ACCEL_YOUT_H       0x3D
#define ACCEL_YOUT_L       0x3E
#define ACCEL_ZOUT_H       0x3F
#define ACCEL_ZOUT_L       0x40

// GYROSCOPE DATA OUT REGISTERS.

#define GYRO_XOUT_H        0x43
#define GYRO_XOUT_L        0x44
#define GYRO_YOUT_H        0x45
#define GYRO_YOUT_L        0x46
#define GYRO_ZOUT_H        0x47
#define GYRO_ZOUT_L        0x48

//****************************Accelorometer Variables***************

int16_t Accel_X_RAW;
uint8_t tempX_H;
uint8_t tempX_L;
float Ax;

int16_t Accel_Y_RAW;
uint8_t tempY_H;
uint8_t tempY_L;
float Ay;

int16_t Accel_Z_RAW;
uint8_t tempZ_H;
uint8_t tempZ_L;
float Az;

//**************************** Gyroscope Variables***************

int16_t Gyro_X_RAW;
uint8_t tempGX_H;
uint8_t tempGX_L;
float Gx;

int16_t Gyro_Y_RAW;
uint8_t tempGY_H;
uint8_t tempGY_L;
float Gy;

int16_t Gyro_Z_RAW;
uint8_t tempGZ_H;
uint8_t tempGZ_L;
float Gz;

float angulo;
float theta;
float theta_p;

float me = 0.0;

float Gy_0 = -184.5;
float Ax_0 = 695.2;
float Ay_0 = -330.2;
float Az_0 = 13084.9;        



void START( void )
{
	I2C1CONbits.SEN = 1;				
	while (I2C1CONbits.SEN);			
}

void IDLE( void )
{
	while (I2C1STATbits.TRSTAT);		
}

void PUTC( char dato )					
{
	I2C1TRN = dato;						      //registro para enviar
	while (I2C1STATbits.TBF);			
}

 uint8_t GETC( uint8_t a)
{
    uint8_t b=a;
	I2C1CONbits.RCEN = 1;				
	I2C1STATbits.I2COV = 0;
    
	while( !I2C1STATbits.RBF );
    if(b==1)
    {
        I2C1CONbits.ACKDT = 0;
        I2C1CONbits.ACKEN = 1;
        while(I2C1CONbits.ACKEN );
    }
    if(b==0)
    {
        I2C1CONbits.ACKDT = 1;
        I2C1CONbits.ACKEN = 1;
        while(I2C1CONbits.ACKEN );
    }
    return(I2C1RCV);                          //registro para recibir
}

void WACK( void )
{
	while( I2C1STATbits.ACKSTAT );	
}

void STOP( void )
{
	I2C1CONbits.PEN = 1;				
	while (I2C1CONbits.PEN);			
}

void RST( void )
{
	I2C1CONbits.RSEN = 1;				
	while (I2C1CONbits.RSEN);	
}

void init_I2C( void )
{
	I2C1BRG = 0x0040;
	I2C1ADD = 0x88 >> 1;
	I2C1CON = 0xC220;
	I2C1CONbits.IPMIEN = 0; // IPMIEN Disabled
	I2C1CONbits.A10M = 0 >> 2; 
	I2C1CONbits.DISSLW = 1;  // SLEW disabled 
	I2C1CONbits.SMEN = 0;    // Disable SMbus
	I2C1CONbits.GCEN = 0;   // general call address disabled 
	I2C1CONbits.STREN = 0; // Disable clock switcjing 
	I2C1CONbits.I2CEN = 1;  // Enables I2C1 peripheral
	IFS1bits.SI2C1IF = 0;   // interrupt request has not ocurred
	IEC1bits.SI2C1IE = 0;   // Interrupt request not enabled
}

void MPU6050_Init()										
{		
	// power management register 0x6B we should write all 0'S to wake the sensor up
    
    START();
    PUTC(MPU6050_ADDR);  	
    IDLE();
    WACK();
    PUTC(PWR_MGMT_1_REG);
    IDLE();
    WACK();
    PUTC(0x00);
    IDLE();
    WACK();
    STOP();
    
    // set DATA RATE of 1 KHz by writing SMPLRT_DIV_REGISTER
    
    START();
    PUTC(MPU6050_ADDR);  	
    IDLE();
    WACK();
    PUTC(SMPLRT_DIV_REG);
    IDLE();
    WACK();
    PUTC(0x07);
    IDLE();
    WACK();
    STOP();
    
    
    // set Accelerometer configuration in ACCEL_CONFIG Register
    // XA_ST =0, YA_ST=0, ZA_ST=0, FS_SEL = 0 -> +/- 2g.
    
    START();
    PUTC(MPU6050_ADDR);  	
    IDLE();
    WACK();
    PUTC(ACCEL_CONFIG_REG);
    IDLE();
    WACK();
    PUTC(0x00);
    IDLE();
    WACK();
    STOP();
    
    // set Gyroscope configuration in GYRO_CONFIG Register
    // XG_ST =0, YG_ST=0, ZG_ST=0, FS_SEL = 0 -> +/- 250 °/s.
    
    START();
    PUTC(MPU6050_ADDR);  	
    IDLE();
    WACK();
    PUTC(GYRO_CONFIG_REG);
    IDLE();
    WACK();
    PUTC(0x00);
    IDLE();
    WACK();
    STOP();
}

void MPU6050_Read_Accel(void)
{
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(ACCEL_XOUT_H);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempX_H = GETC(0);
    STOP();
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(ACCEL_XOUT_L);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempX_L = GETC(0);
    STOP();
    
    // Reading Y_ACCEL
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(ACCEL_YOUT_H);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempY_H = GETC(0);
    STOP();
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(ACCEL_YOUT_L);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempY_L = GETC(0);
    STOP();
    
    // Reading Z_ACCEL
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(ACCEL_ZOUT_H);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempZ_H = GETC(0);
    STOP();
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(ACCEL_ZOUT_L);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempZ_L = GETC(0);
    STOP();

    // RAW DATA
    
    Accel_X_RAW = (int16_t)(tempX_H << 8 | tempX_L);
    Accel_Y_RAW = (int16_t)(tempY_H << 8 | tempY_L);
    Accel_Z_RAW = (int16_t)(tempZ_H << 8 | tempZ_L);
    
    // SCALED DATA
    
    Ax = (Accel_X_RAW - Ax_0)/16384.0;
    Ay = (Accel_Y_RAW - Ay_0)/16384.0;
    Az = (Accel_Z_RAW + Az_0)/16384.0;
}

void MPU6050_Read_Gyro()
{
    // Reading X_GYRO
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(GYRO_XOUT_H);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempGX_H = GETC(0);
    STOP();
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(GYRO_XOUT_L);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempGX_L = GETC(0);
    STOP();
    
    // Reading Y_GYRO
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(GYRO_YOUT_H);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempGY_H = GETC(0);
    STOP();
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(GYRO_YOUT_L);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempGY_L = GETC(0);
    STOP();
    
    // Reading Z_GYRO
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(GYRO_ZOUT_H);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempGZ_H = GETC(0);
    STOP();
    
    START();
	PUTC(MPU6050_ADDR);	
	IDLE();				
    PUTC(GYRO_ZOUT_L);			
	IDLE();
	RST();
	PUTC(MPU6050_READ);	
    IDLE();
    tempGZ_L = GETC(0);
    STOP();

    // RAW DATA
    
    Gyro_X_RAW = (int16_t)(tempGX_H << 8 | tempGX_L);
    Gyro_Y_RAW = (int16_t)(tempGY_H << 8 | tempGY_L);
    Gyro_Z_RAW = (int16_t)(tempGZ_H << 8 | tempGZ_L);
    
    // SCALED DATA
    
    Gx = Gyro_X_RAW/131.0;
    Gy = (Gyro_Y_RAW - Gy_0)/131.0;
    Gz = Gyro_Z_RAW/131.0;
}

float angle(){
    MPU6050_Read_Accel();
    MPU6050_Read_Gyro();
    angulo  = (180 / 3.141593)*-atan(Ax/sqrt((Ay*Ay) + (Az*Az)));
    theta = (((0.99)*(theta_p + ( Gy * 0.001))) + (0.01 * angulo));
    theta_p = theta;
    return theta;
}