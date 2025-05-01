#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototipos de función
void setup();
void UART_init();
void write_char(char caracter);
void write_str(char* texto);


// MAIN LOOP
int main(void)
{
	setup();
	sei();
	write_str("Lab completo");
	while (1)
	{
	}
}

// NON-Interrupt subroutines
// Funciones de Seteo
void setup()
{
	cli();
	DDRB = 0xFF;

	UART_init();
	sei();
}


void UART_init()
{
	DDRD |= (1 << DDD1);	// Configuración de pines rx y tx
	DDRD &= ~(1 << DDD0);
	UCSR0A = 0;				// Configuración del serial
	UCSR0B |= (1 << RXCIE0) | (1<< RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	UBRR0 = 103;	// BAUD RATE a 9600
}

void write_char(char caracter)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = caracter;
}

void write_str(char* texto)
{
	for(uint8_t i = 0; *(texto+i) != 'ÿ'; i++)
	{
		write_char(*(texto+i));
	}
}


// Interrupt routines
ISR(USART_RX_vect)
{
	char temporal = UDR0;
	write_str(&temporal);
}