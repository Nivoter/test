/* 
 * File:   main.c
 * Author: User
 *
 * Created on 20 ??????? 2018 ?., 12:28
 */
//#ifndef PRAGMA 
#define	PRAGMA 

// CONFIG3
#pragma config WPFP = WPFP511           // Write Protection Flash Page Segment Boundary (Highest Page (same as page 170))
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable bit (Segmented code protection disabled)
#pragma config WPCFG = WPCFGDIS         // Configuration Word Code Page Protection Select bit (Last page(at the top of program memory) and Flash configuration words are not protected)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select bit (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMOD = HS            // Primary Oscillator Select (HS oscillator mode selected)
#pragma config DISUVREG = OFF           // Internal USB 3.3V Regulator Disable bit (Regulator is disabled)
#pragma config IOL1WAY = ON             // IOLOCK One-Way Set Enable bit (Write RP Registers Once)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSCO functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary oscillator (XT, HS, EC))
#pragma config PLL_96MHZ = ON           // 96MHz PLL Disable (Enabled)
#pragma config PLLDIV = DIV12           // USB 96 MHz PLL Prescaler Select bits (Oscillator input divided by 12 (48MHz input))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-speed start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator functions are shared with PGEC1/PGED1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF              // JTAG Port Enable (JTAG port is enabled)

#define FOSC 32000000 
#define FCY FOCS/2 

#include <libpic30.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

volatile int x;
volatile int y;
volatile long r=0;;


void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
    // x[y] = U1RXREG; // Receiveing data
    LATDbits.LATD7 =1;
    while(r<1000000)
    {
        r++;
    }
    r=0;
    _U1RXIF = 0;	// manually cleared U2RX Interrupt flag
}

void __attribute__((__interrupt__, auto_psv)) _U1XISELInterrupt(void)
{
    //U1TXREG = x[y]; //Transmitting data
    LATDbits.LATD7 =0;
}

void UART_init(void)
{
     // Unlock Registers
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    // Configure Input Functions (Table 9-1))
    // Assign U1RX To Pin RP0
    RPINR18bits.U1RXR = 0;
    // Assign U1CTS To Pin RP1
    RPINR18bits.U1CTSR = 1;
    // Configure Output Functions (Table 9-2)
    // Assign U1TX To Pin RP2
    RPOR1bits.RP2R = 3;
    // Assign U1RTS To Pin RP3
    RPOR1bits.RP3R = 4;
    // Lock Registers
    __builtin_write_OSCCONL(OSCCON | 0x40);
    U1BRG = 8; //115200 boud
    U1MODEbits.UARTEN = 1;
    U1MODEbits.USIDL =0;
    U1MODEbits.IREN =0;
    U1MODEbits.RTSMD = 1;
    U1MODEbits.UEN1 = 1;
    U1MODEbits.UEN0 =0;
    U1MODEbits.WAKE = 0;
    U1MODEbits.LPBACK = 0;   
    U1MODEbits.ABAUD =0;
    U1MODEbits.RXINV = 0;
    U1MODEbits.BRGH =0;
    U1MODEbits.PDSEL =0b00;
    U1MODEbits.STSEL =0;
    U1STAbits.URXISEL1 = 1;
    U1STAbits.URXISEL0 = 0;
    IEC0bits.U1TXIE =1;
    IEC0bits.U1RXIE =1;
}

int main(void) {
    
    UART_init();
    long i =0;
    TRISDbits.TRISD7 = 0;
    TRISAbits.TRISA6 = 0;
    LATAbits.LATA6 =1;
    while (1)
    {
        i++;
        if(i>1000000)
        {
            LATAbits.LATA6 =0;
            if(i>2000000)
            {
                i=0;
            }
        }
        else
        {
            LATAbits.LATA6 =1;
        }
    }   
}