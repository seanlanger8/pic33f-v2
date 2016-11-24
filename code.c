#include <p33FJ16GS402.h>
#include <libpic30.h>
#include<spi.h>


typedef enum {
    S0, S1
} STATES;               //declare variables


void initAdc1(void) 
{ 
AD1CON1bits.FORM = 3;   // Data Output Format: Signed Fraction (Q15 format) 
AD1CON1bits.SSRC = 7;   // Internal Counter (SAMC) ends sampling and starts conversion 
AD1CON1bits.ASAM = 1;   // ADC Sample Control: Sampling begins immediately after conversion. Set to 0 if sampling should begin when SAMP bit is set.
AD1CON1bits.AD12B = 0;  // 10-bit ADC operation 
AD1CON2bits.SIMSAM = 0; // Sequential sampling of channels. This may have to be AD1CON1


AD1CON2bits.CHPS = 1;   // Converts channels CH0/CH1 


AD1CON3bits.ADRC = 0;   // ADC Clock is derived from Systems Clock 
AD1CON3bits.SAMC = 0;   // Auto Sample Time = 0 * TAD 
AD1CON3bits.ADCS = 2;   // ADC Conversion Clock TAD = TCY * (ADCS + 1) = (1/40M) * 3 = 
                        // 75 ns (13.3 MHz) 
                        // ADC Conversion Time for 10-bit Tconv = 12 * TAD = 900 ns (1.1 MHz) \
                        // may have to set to 1


AD1CON1bits.ADDMABM = 1; // DMA buffers are built in conversion order mode 
AD1CON2bits.SMPI = 0;   // SMPI must be 0 


//AD1CHS0/AD1CHS123: Analog-to-Digital Input Select Register 
AD1CHS0bits.CH0SA = 0;  // MUXA +ve input selection (AIN0) for CH0 
AD1CHS0bits.CH0NA = 0;  // MUXA -ve input selection (VREF-) for CH0 


AD1CHS123bits.CH123SA = 0; // MUXA +ve input selection (AIN0) for CH1 
AD1CHS123bits.CH123NA = 0; // MUXA -ve input selection (VREF-) for CH1 


//AD1PCFGH/AD1PCFGL: Port Configuration Register 
AD1PCFGL = 0xFFFF; 
AD1PCFGH = 0xFFFF; 
AD1PCFGLbits.PCFG0 = 0; // AN0 as Analog Input 
IFS0bits.AD1IF = 0;     // Clear the Analog-to-Digital interrupt flag bit 
IEC0bits.AD1IE = 0;     // Do Not Enable Analog-to-Digital interrupt 
AD1CON1bits.ADON = 1;   // Turn on the ADC 


//main code here. When the input voltage is greater than a specified amount, blink the light
int main(void) {
    STATES state = S0; /* set initial state to S0 */
    int input;


initialize_IO_ports (); // digital ports initialization
initAdc1();             //analog ports initialization

while (1)               // Repeat continuously
{
DelayNmSec(100);            // Sample for 100 ms
AD1CON1bits.SAMP = 0;       // Start converting
while (!AD1CON1bits.DONE;   // Conversion done?
AD1CON1bits.DONE = 0);      // Clear conversion done status bit
ADCValue = ADC1BUF0;        // If yes, then get the ADC value
}                           // Repeat
    
    
    /* infinite loop */
    while (1) {
        /* reads input. Double check this */
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
    

/* Data received at SPI2 */
unsigned int datard ;
void __attribute__((__interrupt__)) _SPI1Interrupt(void)
{  
    IFS0bits.SPI1IF = 0;
} 
void __attribute__((__interrupt__)) _SPI2Interrupt(void)
{
    IFS1bits.SPI2IF = 0;
    SPI1STATbits.SPIROV = 0;  /* Clear SPI1 receive overflow
                                 flag if set */
} 
int main(void)
{
/* Holds the information about SPI configuartion */
   unsigned int SPICONValue;
/* Holds the information about SPI Enable/Disable */
   unsigned int SPISTATValue;
/*Timeout value during which timer1 is ON */
   int timeout; 
/* Turn off SPI modules */
    CloseSPI1();
    CloseSPI2();
    TMR1 = 0 ;
    timeout = 0;
    TRISDbits.TRISD0 = 0;
/* Configure SPI2 interrupt */
    ConfigIntSPI2(SPI_INT_EN &  SPI_INT_PRI_6);
/* Configure SPI1 module to transmit 16 bit timer1 value
   in master mode  */
    SPICONValue  =  FRAME_ENABLE_OFF & FRAME_SYNC_OUTPUT &
                    ENABLE_SDO_PIN & SPI_MODE16_ON &
                    SPI_SMP_ON & SPI_CKE_OFF &
                    SLAVE_ENABLE_OFF &
                    CLK_POL_ACTIVE_HIGH &
                    MASTER_ENABLE_ON &
                    SEC_PRESCAL_7_1 &
                    PRI_PRESCAL_64_1;
    SPISTATValue  = SPI_ENABLE & SPI_IDLE_CON &
                    SPI_RX_OVFLOW_CLR;
    OpenSPI1(SPICONValue,SPISTATValue );
/* Configure SPI2 module to receive 16 bit timer value in
   slave mode */
    SPICONValue  =  FRAME_ENABLE_OFF & FRAME_SYNC_OUTPUT &
                    ENABLE_SDO_PIN  & SPI_MODE16_ON &
                    SPI_SMP_OFF & SPI_CKE_OFF &
                    SLAVE_ENABLE_OFF  &
                    CLK_POL_ACTIVE_HIGH &
                    MASTER_ENABLE_OFF &
                    SEC_PRESCAL_7_1 &
                    PRI_PRESCAL_64_1;
    SPISTATValue =  SPI_ENABLE & SPI_IDLE_CON &
                    PI_RX_OVFLOW_CLR;
    OpenSPI2(SPICONValue,SPISTATValue );
    T1CON = 0X8000;
    while(timeout< 100 )
    {
        timeout = timeout+2 ;
    }
    T1CON = 0;
    WriteSPI1(TMR1);
    while(SPI1STATbits.SPITBF);
    while(!DataRdySPI2());
    datard =  ReadSPI2();
    if(datard <= 600)
    {
        PORTDbits.RD0 = 1;
    }
    
/* Turn off SPI module and clear IF bit  */
    CloseSPI1();  
    CloseSPI2();
    return 0;
}
