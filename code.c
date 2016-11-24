#include <p33FJ16GS402.h>
#include <libpic30.h>


typedef enum {
    S0, S1
} STATES; //declare variables


void initAdc1(void) 
{ 
AD1CON1bits.FORM = 3; // Data Output Format: Signed Fraction (Q15 format) 
AD1CON1bits.SSRC = 7; // Internal Counter (SAMC) ends sampling and starts conversion 
AD1CON1bits.ASAM = 1; // ADC Sample Control: Sampling begins immediately after conversion. Set to 0 if sampling should begin when SAMP bit is set.
AD1CON1bits.AD12B = 0; // 10-bit ADC operation 
AD1CON2bits.SIMSAM = 0; // Sequential sampling of channels. This may have to be AD1CON1


AD1CON2bits.CHPS = 1; // Converts channels CH0/CH1 


AD1CON3bits.ADRC = 0; // ADC Clock is derived from Systems Clock 
AD1CON3bits.SAMC = 0; // Auto Sample Time = 0 * TAD 
AD1CON3bits.ADCS = 2; // ADC Conversion Clock TAD = TCY * (ADCS + 1) = (1/40M) * 3 = 
// 75 ns (13.3 MHz) 
// ADC Conversion Time for 10-bit Tconv = 12 * TAD = 900 ns (1.1 MHz) \
// may have to set to 1


AD1CON1bits.ADDMABM = 1; // DMA buffers are built in conversion order mode 
AD1CON2bits.SMPI = 0; // SMPI must be 0 


//AD1CHS0/AD1CHS123: Analog-to-Digital Input Select Register 
AD1CHS0bits.CH0SA = 0; // MUXA +ve input selection (AIN0) for CH0 
AD1CHS0bits.CH0NA = 0; // MUXA -ve input selection (VREF-) for CH0 


AD1CHS123bits.CH123SA = 0; // MUXA +ve input selection (AIN0) for CH1 
AD1CHS123bits.CH123NA = 0; // MUXA -ve input selection (VREF-) for CH1 


//AD1PCFGH/AD1PCFGL: Port Configuration Register 
AD1PCFGL = 0xFFFF; 
AD1PCFGH = 0xFFFF; 
AD1PCFGLbits.PCFG0 = 0; // AN0 as Analog Input 
IFS0bits.AD1IF = 0; // Clear the Analog-to-Digital interrupt flag bit 
IEC0bits.AD1IE = 0; // Do Not Enable Analog-to-Digital interrupt 
AD1CON1bits.ADON = 1; // Turn on the ADC 


void initDma0(void) 
{ 
DMA0CONbits.AMODE = 0; // Configure DMA for Register indirect with post increment 
DMA0CONbits.MODE = 2; // Configure DMA for Continuous Ping-Pong mode 


DMA0PAD = (int)&ADC1BUF0; 
DMA0CNT = (NUMSAMP-1); 


DMA0REQ = 13; 


DMA0STA = __builtin_dmaoffset(BufferA); 
DMA0STB = __builtin_dmaoffset(BufferB); 


IFS0bits.DMA0IF = 0; //Clear the DMA interrupt flag bit 
IEC0bits.DMA0IE = 1; //Set the DMA interrupt enable bit 


DMA0CONbits.CHEN = 1;
}


//main code here. When the input voltage is greater than a specified amount, blink the light
int main(void) {
    STATES state = S0; /* set initial state to S0 */
    int input;


    initialize_IO_ports (); // digital ports initialization
initDma0(); //direct memory access initialization
initAdc1(); //analog ports initialization


    /* infinite loop */
    while (1) {
        /* read input. Double check this */
       input  = PORTAbits.RA0 ^ 1;


        /* determine next state */
        switch (state) {
            case S0:
                switch (input > ???) { //set voltage here
                    case 0: state = S0;
                        break;
                    case 1: state = S1;
                        break;
                };
                break;
            case S1:
                switch (input > ????) { //same voltage
                    case 0: state = S1;
                        break;
                    case 1: state = S0;
                        break;
                };
                break;
        }


        /* set output. Figure out how this works as SPI */
        switch (state) {
            case S0: LATAbits.LATA1 = 0;
                break;
            case S1: LATAbits.LATA1 = 1;
                break;
        }


            __delay_ms(1000); //Blink speed
        }

        }
