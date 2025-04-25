/*
 * Pre_Lab6.c
 *
 * Created: 25/04/2025 09:49:33
 * Author : Arnulfo Díaz
 */ 

#include <avr/io.h>
#include  <avr/interrupt.h>

void setup();
void initUART();
void writeChar(char caracter);

int main(void)
{
	setup();
	writeChar();
    while (1) 
    {
    }
}


void setup(){
	cli();
	
	initUART();
	
	sei();
}

void initUART(){
	
	DDRD |= (1 << DDD1);
	DDRD &= ~(1 << DDD0);
	
	UCSR0A = 0
	
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	
	UBRR0 = 103;
}

void writeChar(char caracter){
	
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = caracter;
}