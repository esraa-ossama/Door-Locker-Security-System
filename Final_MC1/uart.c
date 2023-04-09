#include "uart.h"

#define BAUD_PRESCALE_U2X (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)      // 8 -> DOUBLE SPEED ASYNCHRONOUS
#define BAUD_PRESCALE_SYNCH (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)  // 16 -> ASYNCHRONOUS

void UART_init( UART_Config *config )
{
	/*  UCSRB REGISTER
	 *  RXC,TXC,UDR INTERRUPT ENABLE = 0,  T/R DATA BIT 8 = 0
	 */

	UCSRB|= (1<<RXEN) | (1<<TXEN) ;    // TRANSMITER ENABLE AND RECIEVER ENABLE

	/* UCSRC REGISTER
	 * USBS=0 -> 1 STOP BIT
	 * UPM0 =0 ,UPM1=1 -> EVEN PARITY BIT
	 */

	UCSRC|= (1<<URSEL); // SELECTING UCSRC BY SETTING 1 TO 'URSEL'
	UCSRC|= (1<<UCSZ0) | (1<<UCSZ1);   //CHARCTER SIZE 8 BITS IN FRAME TRANSFERED

	UCSRC|= ( (config->ParityBits) << 4); //CHOOSE PARITY BITS



		if (config->mode) //1 IF SYNCH
		{
			UCSRC |= ((config->mode) << 6);  // SYNCHRONOUS OPERATION

			UCSRC |= (1<<UCPOL); // DATA TRANSMITTED ON FALLING EDGE , AND SAMPLED ON POSITIVE EDGE

			UBRRH = (BAUD_PRESCALE_SYNCH>>8);
			UBRRL =  BAUD_PRESCALE_SYNCH;
		}

		else
		{							/* UMSEL =0 -> ASYNCHRONOUS OPERATION
		 	 	 	 	 	 	 	 * U2X =1 DOUBLE TRANSMISSION SPEED FOR ASYNCHRONOUS */
			UCSRC &= ~((config->mode) << 6);
			UCSRA |= (1<<U2X);
			UBRRH = (BAUD_PRESCALE_U2X>>8);
			UBRRL =  BAUD_PRESCALE_U2X;
		}
}

void UART_sendByte(uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR=data;
}

uint8 UART_receiveByte()
{
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;
}

void UART_sendString(const char *str)
{
	uint8 i = 0;
	while(str[i] != '\0')
	{
		UART_sendByte(str[i]);
		i++;
	}

}

void UART_receiveString(char *str)
{
	uint8 i = 0;
	str[i] = UART_receiveByte();
	while(str[i] != '#')   // keep receiving until #
	{
		i++;
		str[i] = UART_receiveByte();
	}
	str[i] = '\0';
}
