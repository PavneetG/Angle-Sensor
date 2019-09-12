#include <hidef.h>
#include "derivative.h"
#include "SCI.h" //allows us to use serial communication

char string[20];
unsigned short val;  
void setClk(void);
void delay1ms(int delayBy);
void BCD (int bcdVal);
void BCDBar (int bcdVal);

/* DECLARE ALL YOUR GLOBAL VARIABLES BELOW*/
  /////////////////////////////////////////////////////
int counter = 0; 
int onOff = 0;  
int temp; 
unsigned short int val = 0; 
unsigned short int val1 = 0;
unsigned int bVal = 0;  
float degree; 

//---------------------OutCRLF---------------------
// Output a CR,LF to SCI to move cursor to a new line
// Input: none
// Output: none
// Toggle LED each time through the loop

void OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
  PTJ ^= 0xF0;          // toggle LED D2
}

void main(void) {
// Setup and enable ADC channel 0
// Refer to Chapter 14 in S12G Reference Manual for ADC subsystem details

/* DECLARE ALL YOUR LOCAL VARIABLES BELOW*/
  /////////////////////////////////////////////////////
 	
  /* DECLARE ALL YOUR LOCAL VARIABLES ABOVE*/  
  /////////////////////////////////////////////////////
 	
  /* ESDUINO PORT CONFIGURATION BELOW (Don't Edit) */
  ///////////////////////////////////////////////////// 
  //Set Ports
  DDRJ = 0x01;  	//set all port J as output
  DDR1AD = 0x7F;
  DDRP = 0x03; 
/*
 * The next six assignment statements configure the Timer Input Capture                                                  
 */       	
  TSCR1 = 0x90;	//Timer System Control Register 1
                    // TSCR1[7] = TEN:  Timer Enable (0-disable, 1-enable) //set to 1 
                	// TSCR1[6] = TSWAI:  Timer runs during WAI (0-enable, 1-disable)
                	// TSCR1[5] = TSFRZ:  Timer runs during WAI (0-enable, 1-disable)
        	        // TSCR1[4] = TFFCA:  Timer Fast Flag Clear All (0-normal 1-read/write clears interrupt flags) //set to 1
                	// TSCR1[3] = PRT:  Precision Timer (0-legacy, 1-precision)
                	// TSCR1[2:0] not used
 
  TSCR2 = 0x04;	//Timer System Control Register 2
                	// TSCR2[7] = TOI: Timer Overflow Interrupt Enable (0-inhibited, 1-hardware irq when TOF=1)   // 0 
                	// TSCR2[6:3] not used     //0
                	// TSCR2[2:0] = Timer Prescaler Select: See Table22-12 of MC9S12G Family Reference Manual r1.25 (set for bus/1)    // 1
 
                	
  TIOS = 0xFC; 	//Timer Input Capture or Output capture
                	//set TIC[0] and input (similar to DDR)
  PERT = 0x03; 	//Enable Pull-Up resistor on TIC[0]
 
  TCTL3 = 0x00;	//TCTL3 & TCTL4 configure which edge(s) to capture
  TCTL4 = 0x0A;	//Configured for falling edge on TIC[0]
 
/*
 * The next one assignment statement configures the Timer Interrupt Enable        	                                       
 */       	
  
  TIE = 0x01;  	//Timer Interrupt Enable
 
/*
 * The next one assignment statement configures the ESDX to catch Interrupt Requests                                                  
 */           
  IRQCR=0x00;
	EnableInterrupts;
	
	setClk(); //set the E-Clock speed
  SCI_Init(19200);
  
  ATDCTL1 = 0x4F; //configure for 12 bit resolution //from slides
  ATDCTL3 = 0x88; //right justified //one sample per sequence 
  ATDCTL4 = 0x01; //prescaler set to 1 which is calculated to be 1mhz
  ATDCTL5 = 0x27; //continous conversion on channel 7
 
 
 while(1)                    
    {
            
         if (counter%3 == 1) { //if column (A0) is GND then LED will light up
         
         val = ATDDR0;
         
              if(val > 1340 && val < 1434){ //for 0-20
                    degree = (0.1864*val -250.68);    
               }   
              if(val >= 1434 && val < 1630){ //for 25-75
                    degree = (0.2872*val -394.18);
               }
              if(val <= 1640 && val >= 1630){//for 80-90
                    degree = (1.6667*val - 2640);
                  }
              if(val >= 1638){ //beyond 90 keep it at 90 
                     degree = 90;
                  }
         
             delay1ms(100);
             SCI_OutUDec(degree);
             OutCRLF();
                          
                 if (degree >=0 && degree <= 10) { //conversion fo angle to bar
                  bVal = 1; 
                 
                } else if (degree <= 20) {
                  bVal = 2;
                }
                else if (degree <= 30) {
                   bVal = 3;
                }
                else if (degree <= 40) {
                     bVal = 4;
                }
                else if (degree <= 50) {
                       bVal = 5;
                }
                else if (degree <= 60) {
                         bVal = 6;
                }
                else if (degree <= 70) {
                  bVal = 7;
                }
                else if (degree <= 80) {
                   bVal = 8;
                } else {
                   bVal = 9;
                }
            BCDBar (bVal);
       }
      
        if (counter%3 == 2) {
              val = ATDDR0;
           
               if(val > 1340 && val < 1434){ //for 0-20
              degree = (0.1864*val -250.68);    
         }   
        if(val >= 1434 && val < 1614){ //for 25-75
              degree = (0.3087*val - 425.75);
         }
        if(val <= 1620 && val >= 1614){//for 80-90
              degree = (2.5*val - 3957.5);
            }
        if(val > 1620){ //beyond 90 keep it at 90 
               degree = 90;
            } 
               
           
             delay1ms(100); 
             SCI_OutUDec(degree);
             OutCRLF();
            
                BCD(degree); 
        
        }
     
      delay1ms(1000); 
        PTJ ^= 0x01;

      }
     
    
}

interrupt  VectorNumber_Vtimch0 void ISR_Vtimch0(void) //Timer channel 0 which is Vtimch 0 based on TIE 0x01
{
  /* DECLARE ALL YOUR LOCAL VARIABLES BELOW*/
  /////////////////////////////////////////////////////
  unsigned int temp; //DON'T EDIT THIS
  counter = counter +1; 
  PTJ ^= 0x01;
  temp = TC0; 
}

void setClk(void)
{
 
  CPMUCLKS = 0x80; // choose PLL = 1
  CPMUOSC = 0x00; // source clock chosen
  CPMUPROT = 0x26; // protection of clock config is enable
  // now frequency_ref = 1MHz
  CPMUSYNR = 0x0F; // VCOFRQ = 00, SYNDIN = 15 = F; 32MHz = VCOCLK = 2*1MHz*(1+SYNDIV);
  CPMUFLG = 0x00; // LOCK = 0
  // PLLCLK = 32MHz/4 = 8MHz and BUSCLK = PLLCLK/2 = 4MHz! bus speed set to 4MHz  
  CPMUPROT = 1; //enable clock protection 
}

//Delay Function
void delay1ms(int delayBy)
{
    int a;
    TSCR1 = 0x90; // Enable time and clear fast timer flag
    TSCR2 = 0x00; // Disable timer interrupt, set prescaler to 1        
    
    TIOS = 0x02;
    TC1 = TCNT + 4000; // Delay by 1/4mhz * 4000 = 1ms
    
    for (a = 0; a < delayBy; a++) {
      
      while(!(TFLG1_C1F)); 
      
      TC1 += 4000;
    }
    
}
  
void BCD (unsigned int bcdVal){ //Converting to BCD values
    
    unsigned int ones;
    unsigned int tens;
    unsigned int hundreds;
    unsigned int temp;
    unsigned int ored; 
    
    temp = bcdVal;
    temp = temp%100;
    tens = temp/10;
    ones = temp%10;
    
    ored = ((tens<<4)|ones)&(0b01111111); 
    PT1AD = ored;
    
    if (bcdVal >= 80) {
        PTP = 0b00000001;
    } else {
       PTP = 0b00000000;
    }
      
}

void BCDBar (unsigned int bcdVal) //Converting BCD Bar values 
{
   
   if(bcdVal == 1) 
   {
      PT1AD = 0b00000001;
      PTP = 0b00000000;
   } 
   else if(bcdVal == 2)  
   {
      PT1AD = 0b00000011;
      PTP = 0b00000000;
   }
   else if(bcdVal == 3)  
   {
      PT1AD = 0b00000111;
      PTP = 0b00000000;
   }
   else if(bcdVal == 4)  
   {
      PT1AD = 0b00001111;
      PTP = 0b00000000;
   }
   else if (bcdVal == 5) 
   {
      PT1AD = 0b00011111;
      PTP = 0b00000000;
   }
   else if(bcdVal == 6)  
   {
      PT1AD = 0b00111111;
      PTP = 0b00000000;
   }
   else if(bcdVal == 7)  
   {
      PT1AD = 0b01111111;
      PTP = 0b00000000;
   }
   else if(bcdVal == 8)  
   {
      PT1AD = 0b01111111;
      PTP = 0b00000001;
   }
   else if(bcdVal == 9)  
   {
      PT1AD = 0b01111111;
      PTP = 0b00000011;
   }
   else 
   {
      PT1AD = 0x00;
      PTP = 0x00;
   }
    
}
  