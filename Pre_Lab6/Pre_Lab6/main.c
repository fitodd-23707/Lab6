/*
 * Pre_Lab6.c
 *
 * Created: 25/04/2025 09:49:33
 * Author : Arnulfo Díaz
 * Link Github : https://github.com/fitodd-23707/Lab6
 */ 

#include <avr/io.h>
#include  <avr/interrupt.h>
#define F_CPU 16000000

void setup();
void initUART();
void writeChar(char caracter);
void write_str(char* texto);
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

void write_str(char* texto)
{
	for(uint8_t i = 0; *(texto+i) != 'ÿ'; i++)
	{
		writeChar(*(texto+i));
	}
}

ISR(USART_RX_vect){
	
	char temp = UDR0;
	PORTB = temp;
	
}

ISR(ADC_vect){
	char ent = ADCH;
	write_str(ent);
	ADCSRA |= (1 << ADSC);
}