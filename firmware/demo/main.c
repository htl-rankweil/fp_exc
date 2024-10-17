

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "../lib/adc/adc.h"
#include "../lib/spi/spi.h"
#include "../lib/twi/twi.h"
#include "../lib/uart/uart.h"
#include "../lib/lcd/lcd.h"

static void system_init()
{
	PORTB = 0x0F;	// Enable Pullup
	
	DDRC = 0xFF;	// PORT as Output
	PORTC = 0xAA;	// LEDs on
}

int main(void)
{
	system_init();
	adc_init();
	spi_init(SPI_Master, SPI_MSB, SPI_Rising, SPI_Falling);
	twi_init(TWI_Master);
	uart_init();
	lcd_init();
	
    while (1) 
    {
		PORTC ^= 0xFF;
		
		lcd_home();
		lcd_string("EXC Example");
		lcd_cursor(0,1);
		lcd_string("Demo");
		
		adc_channel(ADC_CH0);
		unsigned int adc0 = adc_read();
		
		adc_channel(ADC_CH1);
		unsigned int adc1 = adc_read();
		
		printf("EXC Example, ADC0: %4u, ADC1: %4u\r", adc0, adc1);
		
		unsigned char data = spi_transfer(PINB);
		
		lcd_char(' ');
		lcd_ul2ascii((0x0F & ~data), LCD_Binary, 8);
		
		_delay_ms(500);
    }
}

