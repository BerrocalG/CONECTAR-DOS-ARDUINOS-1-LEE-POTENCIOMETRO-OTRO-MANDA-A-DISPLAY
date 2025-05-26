#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
float value=0;
int flag=0;
char dato =0;
uint8_t unidad =0;
uint8_t decena =0;
uint8_t centena =0;
uint8_t voltaje0 =0;
uint8_t voltaje1 =0;
uint8_t voltaje2 =0;

void config_ADC(void){
  ADCSRA|=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  ADMUX|=(1<<REFS0);
}

void config_USART(void){
  UCSR0B|=(1<<RXEN0)|(1<<TXEN0)| (1<<RXCIE0);
  UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
  UBRR0=103;//9600
}

ISR(USART_RX_vect){
  dato=UDR0;
   if(dato=='a'){
    ADCSRA|=(1<<ADSC);
   }
}

ISR(ADC_vect){
  
  uint16_t adcvalor = ADC; 
    value = (adcvalor * 5.0) / 1023.0;

  uint16_t voltaje = (uint16_t)(value * 100);
    unidad  = voltaje % 10;
    decena  = (voltaje / 10) % 10;
    centena = (voltaje / 100) % 10;


    // Para USART
    voltaje0 = (uint8_t)value;             // Parte entera
    voltaje1= ((uint8_t)(value * 10)) % 10; // Primera decimal
    voltaje2 =((uint8_t)(value * 100)) % 10; // Segunda decimal

  UCSR0B|=(1<<UDRIE0);
}

ISR(USART_UDRE_vect){
  switch(flag){
    case 0:
    UDR0=voltaje0+'0';
    flag=1;
    break;
    case 1:
    UDR0=(unsigned char)(46);
    flag=2;
    break;
    case 2:
    UDR0=voltaje1+'0';
    flag=3;
    break;
    case 3:
    UDR0=voltaje2+'0';
    flag=4;
    break;
    case 4:
    UDR0=(unsigned char)(13);
    flag=5;
    break;
    case 5:
    UDR0=(unsigned char)(10);
    flag=0;
    UCSR0B&=~(1<<UDRIE0);
   _delay_ms(100);
    break;
  }
}


int main(void){
    DDRD |= 0xF0; 
    DDRB |= 0x07; 

  config_USART();
  config_ADC();
  sei();
  while(1){
    //de codigos pasados
    // UNIDAD
         PORTB &= ~0x07;
        PORTB |= 0x02;
        PORTD &= ~0xF0;
        PORTD = (PORTD & 0x0F) | (unidad << 4);
        _delay_ms(5);


        // DECENA
        PORTB &= ~0x07;
        PORTB |= 0x01;
        PORTD &= ~0xF0;
        PORTD = (PORTD & 0x0F) | (decena << 4);
        _delay_ms(5);


        // CENTENA
         PORTB &= ~0x07;
        PORTB |= 0x04;
        PORTD &= ~0xF0;
        PORTD = (PORTD & 0x0F) | (centena << 4);
        _delay_ms(5);

   
  }
}


