/*
 * main.c
 *
 *  Created on: Sep 29, 2016
 *      Author: parallels
 */

#include <stdarg.h>

#include "Vdm.h"
#include "Fmu.h"
#include "FmuModel.h"

#include "uart.h"
#include "adcutil.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include <util/delay.h>


#define FMI_FORWARDROTATE XX_forwardrotate_XX
#define FMI_LEFTVAL XX_leftval_XX
#define FMI_RIGHTVAL XX_rightval_XX
#define FMI_SERVOLEFTOUT XX_servoleft_XX
#define FMI_SERVORIGHTOUT XX_servoright_XX
#define FMI_BACKWARDROTATE XX_backwardrotate_XX
#define FMI_FORWARDSPEED XX_forwardspeed_XX

#define UART_BAUD_RATE      19200      


void fmuLoggerCache(void *componentEnvironment, fmi2String instanceName,
                    fmi2Status status, fmi2String category, fmi2String message,
                    ...) {}


void uart_initialization()
{
    int  num=134;
	char buffer[20];

    
    /*
     *  Initialize UART library, pass baudrate and AVR cpu clock
     *  with the macro 
     *  UART_BAUD_SELECT() (normal speed mode )
     *  or 
     *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
     */
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 
    
    /*
     * now enable interrupt, since UART library is interrupt controlled
     */
    sei();
    
    /*
     *  Transmit string to UART
     *  The string is buffered by the uart library in a circular buffer
     *  and one character at a time is transmitted to the UART using interrupts.
     *  uart_puts() blocks if it can not write the whole string to the circular 
     *  buffer
     */
    uart_puts("String stored in SRAM\n");
    
    /*
     * Transmit string from program memory to UART
     */
    uart_puts_P("String stored in FLASH\n");
    
        
    /* 
     * Use standard avr-libc functions to convert numbers into string
     * before transmitting via UART
     */     
    itoa( num, buffer, 10);   // convert interger into string (decimal format)         
    uart_puts(buffer);        // and transmit string to UART

    
    /*
     * Transmit single character to UART
     */
    uart_putc('\r');
}

void do_one_serial_echo()
{
	unsigned int c;

	c = uart_getc();
        if ( c & UART_NO_DATA )
        {
            /* 
             * no data available from UART 
             */
        }
        else
        {
            /*
             * new data available from UART
             * check for Frame or Overrun error
             */
            if ( c & UART_FRAME_ERROR )
            {
                /* Framing Error detected, i.e no stop bit detected */
                uart_puts_P("UART Frame Error: ");
            }
            if ( c & UART_OVERRUN_ERROR )
            {
                /* 
                 * Overrun, a character already present in the UART UDR register was 
                 * not read by the interrupt handler before the next character arrived,
                 * one or more received characters have been dropped
                 */
                uart_puts_P("UART Overrun Error: ");
            }
            if ( c & UART_BUFFER_OVERFLOW )
            {
                /* 
                 * We are not reading the receive buffer fast enough,
                 * one or more received character have been dropped 
                 */
                uart_puts_P("Buffer overflow error: ");
            }
            /* 
             * send received character back
             */
            uart_putc( (unsigned char)c );
        }
}



int main()
{
//DDRB = 0xff;
  DDRD = 0xff;
  PORTD = 0x00;
  //PORTB = 0x00;



  InitADC();
  uart_initialization();

  fmi2CallbackFunctions callback = {&fmuLoggerCache, NULL, NULL, NULL, NULL};
 
  //systemInit();

  //Initialize rest of the buffer.

	fmiBuffer.realBuffer[FMI_LEFTVAL] = 0.0;
	fmiBuffer.realBuffer[FMI_RIGHTVAL] = 0.0;
	fmiBuffer.realBuffer[FMI_FORWARDROTATE] = 5.0;
	fmiBuffer.realBuffer[FMI_SERVOLEFTOUT] = 0.0;
	fmiBuffer.realBuffer[FMI_SERVORIGHTOUT] = 0.0;
	fmiBuffer.realBuffer[FMI_BACKWARDROTATE] = 1.0;
	fmiBuffer.realBuffer[FMI_FORWARDSPEED] = 4.0;
  
	fmi2Component instReturn = fmi2Instantiate("this system", fmi2CoSimulation, _FMU_GUID, "", &callback, fmi2True, fmi2True);

	if(instReturn == NULL)
		return 1;

	double now = 0;
	double step = 0.01;

	int lm, rm, alive;

	alive = 0;

	while(true)
	{
	//Read switch values that indicate sensor threshold crossings.
	fmiBuffer.realBuffer[FMI_LEFTVAL] = ReadADC(0);
	fmiBuffer.realBuffer[FMI_RIGHTVAL] = ReadADC(1);

    fmi2DoStep(NULL, now, step, false);	

    now = now + step;

    // sync buffer with hardware

	lm = fmiBuffer.realBuffer[FMI_SERVOLEFTOUT] >= 4 ? 1 : 0;
	rm = fmiBuffer.realBuffer[FMI_SERVORIGHTOUT] <= -4 ? 0 : 1;

    if (lm)
	{
		PORTD &= ~(1 << PIND3);
		PORTD &= ~(1 << PIND4);
    }
	else
	{
		PORTD |= 1 << PIND3;
		PORTD |= 1 << PIND4;
    }

	if (rm)
	{
		PORTD &= ~(1 << PIND2);
		PORTD &= ~(1 << PIND5);
   }
	else
	{
		PORTD |= 1 << PIND2;
		PORTD |= 1 << PIND5;
    }


//	sprintf(buffer, "LS %4d   RS %4d   LM %4d   RM %4d          X  \r\n", (uint16_t)fmiBuffer.realBuffer[FMI_LEFTVAL], (uint16_t)fmiBuffer.realBuffer[FMI_RIGHTVAL], lm, rm);
//	sprintf(buffer, "LS %4d   RS %4d X  \r\n", (uint16_t)fmiBuffer.realBuffer[FMI_LEFTVAL], (uint16_t)fmiBuffer.realBuffer[FMI_RIGHTVAL]);

//	uart_puts(buffer);


	// alive indicator
	if(alive)
    	PORTD |= (1 << PIND6);
	else
    PORTD &= ~(1 << PIND6);

	alive = !alive;
    _delay_ms(200);

	//do_one_serial_echo();
  }


  return 0;
}
