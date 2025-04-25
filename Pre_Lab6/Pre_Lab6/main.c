/*
 * Pre_Lab6.c
 *
 * Created: 25/04/2025 09:49:33
 * Author : Arnulfo Díaz
 */ 

#include <avr/io.h>
#include  <avr/interrupt.h>
#define F_CPU 16000000

void setup();
void initUART();
void writeChar(char caracter);
void initADC();

int main(void)
{
	setup();
	writeChar('h');
    while (1) 
    {
    }
}


void setup(){
	cli();
	
	initUART();
	initADC();
	
	ADCSRA |= (1 << ADSC);
	
	DDRB = 0xFF;
	
	sei();
}

void initUART(){
	
	DDRD |= (1 << DDD1);
	DDRD &= ~(1 << DDD0);
	
	UCSR0A = 0;
	
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	
	UBRR0 = 103;
}

void initADC(){
	
	ADMUX = 0;
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	ADCSRA = 0;
	ADCSRA = (1 << ADPS1) | (1 << ADPS0) | (1 << ADIE) | (1 << ADEN);
}

void writeChar(char caracter){
	
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = caracter;

}

ISR(USART_RX_vect){
	
	char temp = UDR0;
	writeChar('r');
	PORTB = temp;
	
}

ISR(ADC_vect){
	char ent = ADCH;
	writeChar(ent);
	ADCSRA |= (1 << ADSC);
}