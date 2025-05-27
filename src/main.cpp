/*
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

float value = 0;
int flag = 0;
uint8_t voltaje0 = 0;
uint8_t voltaje1 = 0;
uint8_t voltaje2 = 0;
char dato = 0;

void config_ADC(void) {
    ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADMUX |= (1 << REFS0); 

void config_USART(void) {
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); 
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); 
    UBRR0 = 103; // 9600 baudios
}

ISR(USART_RX_vect) {
    dato = UDR0;
    if (dato == 'a') {
        ADCSRA |= (1 << ADSC);
    }
}

ISR(ADC_vect) {
    uint16_t adcvalor = ADC;
    value = (adcvalor * 5.0) / 1023.0;

    voltaje0 = (uint8_t)value; // parte entera
    voltaje1 = ((uint8_t)(value * 10)) % 10;
    voltaje2 = ((uint8_t)(value * 100)) % 10;

    UCSR0B |= (1 << UDRIE0); 
}

ISR(USART_UDRE_vect) {
    switch (flag) {
        case 0:
            UDR0 = voltaje0 + '0'; // centena
            flag = 1;
            break;
        case 1:
            UDR0 = '.'; // separador decimal
            flag = 2;
            break;
        case 2:
            UDR0 = voltaje1 + '0'; // decena
            flag = 3;
            break;
        case 3:
            UDR0 = voltaje2 + '0'; // unidad
            flag = 4;
            break;
        case 4:
            UDR0 = 13; // 
            flag = 5;
            break;
        case 5:
            UDR0 = 10; // nueva línea
            flag = 0;
            UCSR0B &= ~(1 << UDRIE0); // desactiva interrupción
            break;
    }
}

int main(void) {
    config_USART();
    config_ADC();
    sei();

    while (1) {
        // 
    }
}
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t centena = 0, decena = 0, unidad = 0;

void config_USART(void) {
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
    UBRR0 = 103; 
}


int main(void) {
    DDRD |= 0xF0;
    DDRB|= 0x07; 
    config_USART();
    sei();

    while (1) {
        // Mostrar unidad
        PORTB &= ~0x07;
        PORTB |= 0x02;            
        PORTD = (PORTD & 0x0F) | (unidad << 4);
        _delay_ms(5);

        // Mostrar decena
        PORTB &= ~0x07;
        PORTB |= 0x01;           
        PORTD = (PORTD & 0x0F) | (decena << 4);
        _delay_ms(5);

        // Mostrar centena
        PORTB &= ~0x07;
        PORTB |= 0x04;            
        PORTD = (PORTD & 0x0F) | (centena << 4);
        _delay_ms(5);
    }
}

