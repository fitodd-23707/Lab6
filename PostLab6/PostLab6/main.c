/*
 * PostLab6.c
 *
 * Created: 30/04/2025 08:47:52
 * Author : Arnulfo Díaz
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototipos de función
void setup();
void UART_init();
void write_char(char caracter);
void write_str(char* texto);
void initADC();
void UART_write_number(uint16_t num);

volatile char str[10];
volatile char entrada = '0';
volatile char valor = '0';
volatile uint8_t expecting_value = 0; // Nueva variable de estado

int main(void)
{
	setup();
	while (1)
	{
		write_str("\n Bienvenido seleccione una opción: \n 1. Leer POT \n 2. Enviar caracter \n Elección: ");
		while (entrada == '0'){}
		switch (entrada){
			case '1':
			write_str("\n Valor de POT: ");
			ADCSRA |= (1 << ADSC);
			entrada = '0';
			break;
			case '2':
			write_str("\nIngrese valor:");
			expecting_value = 1; // Activar espera del valor
			while (expecting_value == 1) {} // Esperar hasta recibir el valor
			PORTB = valor; // Actualizar PORTB con el valor recibido
			entrada = '0'; // Reiniciar para nuevas selecciones
			break;
		}
	}
}

// Funciones de Seteo
void setup()
{
	cli();
	DDRB = 0xFF;

	UART_init();
	initADC();
	
	sei();
}

void UART_init()
{
	DDRD |= (1 << DDD1);
	DDRD &= ~(1 << DDD0);
	UCSR0A = 0;
	UCSR0B |= (1 << RXCIE0) | (1<< RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = 103;
}

void initADC(){
	ADMUX = 0;
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	ADCSRA = (1 << ADPS1) | (1 << ADPS0) | (1 << ADIE) | (1 << ADEN);
}

void write_char(char caracter)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = caracter;
}

void write_str(char* texto)
{
	for(uint8_t i = 0; *(texto+i) != '\0'; i++)
	{
		write_char(*(texto+i));
	}
}

void UART_write_number(uint16_t pot)
{
	itoa(pot, str, 10);
	write_str(str);
}

// Interrupt routines
ISR(ADC_vect){
	uint16_t ent = ADCH;
	UART_write_number(ent);
}

ISR(USART_RX_vect){
	char received = UDR0; // Leer el carácter recibido

	if (expecting_value) {
		valor = received; // Almacenar el valor para PORTB
		expecting_value = 0; // Desactivar espera
		write_char(received); // Eco del valor recibido
		} 
		else {
		if (entrada == '0') {
			entrada = received; // Capturar selección del menú
		}
		write_char(received); // Eco de la selección
	}
}