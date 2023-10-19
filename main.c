

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>	// for printf/fgets
#include <stdlib.h>	// for atoi()
#include <string.h> // for strlen()
#include <ctype.h>  // for toupper()
#include "mBuffer.h"

#define BUFFSIZE 64

uint8_t txBuffer[BUFFSIZE];
uint8_t rxBuffer[BUFFSIZE];

MBUFFER TXQ;
MBUFFER RXQ;

// see page 146 of avr-libc-user-manual
// for info on why int and char are used rather than uint8_t
// and for more info on FDEV_SETUP_STREAM()

int uart_putchar(char c, FILE* stream);
int uart_getchar(FILE* stream);

FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE mystdin  = FDEV_SETUP_STREAM(NULL,uart_getchar, _FDEV_SETUP_READ);

void uart_9600();

int uart_putchar(char c, FILE* stream)
{
	cli();
	enqueue(&TXQ, c);
	sei();
	UCSR0B |= 1<<UDRIE0;
	return 0;
}

int uart_getchar(FILE* stream)
{
	uint8_t t;
	while (isEmpty(&RXQ))	// while empty, spin
	;
	cli();
	t = dequeue(&RXQ);
	sei();
	return t;
}

ISR (USART0_UDRE_vect)
{
	char t;
	if (isEmpty(&TXQ))
	{
		UCSR0B &= ~(1<<UDRIE0);
	}
	else
	{
		t = dequeue(&TXQ);	// could say UDR0 = dequeue(&TXQ);
		UDR0 = t;
	}
}

ISR (USART0_RX_vect)
{
	char t;
	uint8_t count;
	count = available(&RXQ);
	if (count > 3)
	{
		t = UDR0;
		if (t=='\r')
		{
			enqueue(&RXQ,'\r');
			enqueue(&RXQ,'\n');
			uart_putchar('\r',stdout);
			uart_putchar('\n',stdout);
		}
		else
		{
			enqueue(&RXQ,t);
			uart_putchar(t,stdout);
		}
	}
	else
	uart_putchar('\a',stdout);
}

void uart_9600()
{
	// 9600, 8, no parity, 1 stop
	UBRR0 = 103;
	UCSR0B = 1<<RXCIE0 | 0<<TXCIE0 | 1<<UDRIE0 | 1<<RXEN0 | 1<<TXEN0 | 0<<UCSZ02 ;
	UCSR0C = 0b00<<UPM00 | 0b00<<UPM00 | 0<<USBS0 | 0b11<<UCSZ00;
}

void InitPWM(){
	TCCR0B = (0b0<<WGM02)|(0b011<<CS00);//Pre-Scalar
	TCCR1B = (0b0<<WGM12)|(0b011<<CS10);//Pre-Scalar
	TCCR2B = (0b0<<WGM22)|(0b011<<CS20);//Pre-Scalar
	// Red
	TCCR0A = (0b11<<WGM00)|(0b10<<COM0A0);// Fast PWM
	DDRD |= (1<<6); // PD6
	// Green
	TCCR1A = (0b01<<WGM10)|(0b10<<COM1A0);// Fast PWM
	DDRB |= (1<<1); // PB1
	// Blue
	TCCR2A = (0b11<<WGM20)|(0b10<<COM2A0);// Fast PWM
	DDRB |= (1<<3); // PB3
}
void SetLED(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0A = 255-r;
	OCR1A = 255-g;
	OCR2A = 255-b;
}


int main()
{
	
	InitPWM();
	DDRC = 0xFF;
	PORTC = 0xFF;
	char cmd[40];
	int val;
	bufInit(&TXQ, txBuffer, BUFFSIZE);
	bufInit(&RXQ, rxBuffer, BUFFSIZE);
	stdout = &mystdout;
	stdin = &mystdin;
	uart_9600();
	int cmdStatus = 0;
	printf("Welcome to COLOR Display");
	
	
	
	
	//_delay_ms(1000);
	/* Replace with your application code */
	while (1)
	{
		volatile int cmdStatus = 0;
		printf("\r\nEnter your command: ");
		fgets(cmd, 40-1, stdin);
		
		cmd[strlen(cmd)-2] = '\0'; // get rid of crlf
		
		flush(&TXQ);
		if(strcmp(cmd,"r")==0) {cmdStatus = 1;}
		
		if(strcmp(cmd,"g")==0) {cmdStatus = 2;}
		
		if(strcmp(cmd,"b")==0) {cmdStatus = 3;}
		
		if(strcmp(cmd,"w")==0) {cmdStatus = 4;}
		
		if(strcmp(cmd,"s")==0) {cmdStatus = 5;}
		
		if(strcmp(cmd,"?")==0) {cmdStatus = 6;}
			
		
		switch(cmdStatus)
		{
			
			case 1:
				printf("\r\nEnter your value for r: ");
				scanf("%d", &val);
				printf("\r\nYour value for r is: %d\r\n", val);
				int r = val;
				
				break;
				
			case 2:
				printf("\r\nEnter your value for g: ");
				scanf("%d", &val);
				printf("\r\nYour value for g is: %d", val);
				int g = val;
				
				break;
				
			case 3:
				printf("\r\nEnter your value for b: ");
				scanf("%d", &val);
				printf("\r\nYour value for b is: %d\r\n", val);
				int b = val;
				
			
				break;
				
			case 4:
				printf("\r\nWriting values to LED");
				SetLED(r,g,b);
				
				break;
				
			case 5:
				printf("\r\nr = %d, g = %d, b = %d", r, g, b);
				
				break;
				
			case 6:
			    printf("\r\n r - set red value \r\n g - set green value \r\n b - set blue value");
				flush(&RXQ);
				printf("\r\n w - write value to LED \r\n s - status \r\n ? - display commands");
				break;
			
				
		} 
		
	}
}


