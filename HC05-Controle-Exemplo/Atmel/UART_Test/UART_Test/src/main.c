/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <string.h>

// Descomente o define abaixo, para desabilitar o Bluetooth e utilizar modo Serial via Cabo
//#define DEBUG_SERIAL

#define BUT2_PIO_ID				ID_PIOC
#define BUT2_PIO				PIOC
#define BUT2_PIO_IDX			31
#define BUT2_PIO_IDX_MASK		(1 << BUT2_PIO_IDX)
#define BUT2_DEBOUNCING_VALUE   79

#define BUT3_PIO				PIOA
#define BUT3_PIO_ID				ID_PIOA
#define BUT3_PIO_IDX			19
#define BUT3_PIO_IDX_MASK		(1 << BUT3_PIO_IDX)
#define BUT3_DEBOUNCING_VALUE   79

//TERMINAR DEFINES

#ifdef DEBUG_SERIAL
#define UART_COMM USART1
#else
#define UART_COMM USART0
#endif

volatile long g_systimer = 0;

volatile uint8_t flag_but0 = 0;
volatile uint8_t flag_but1 = 0;
volatile uint8_t flag_but2 = 0;
volatile uint8_t flag_but3 = 0;
volatile uint8_t flag_but4 = 0;
volatile uint8_t flag_but5 = 0;
volatile uint8_t flag_but6 = 0;
volatile uint8_t flag_but7 = 0;
volatile uint8_t flag_but8 = 0;
volatile uint8_t flag_but9 = 0;
volatile uint8_t flag_but10 = 0;
volatile uint8_t flag_but11 = 0;


void SysTick_Handler() {
	g_systimer++;
}

void config_console(void) {
	usart_serial_options_t config;
	config.baudrate = 9600;
	config.charlength = US_MR_CHRL_8_BIT;
	config.paritytype = US_MR_PAR_NO;
	config.stopbits = false;
	usart_serial_init(USART1, &config);
	usart_enable_tx(USART1);
	usart_enable_rx(USART1);
}

void usart_put_string(Usart *usart, char str[]) {
	usart_serial_write_packet(usart, str, sizeof(str));
}

int usart_get_string(Usart *usart, char buffer[], int bufferlen, int timeout_ms) {
	long timestart = g_systimer;
	uint32_t rx;
	uint32_t counter = 0;
	
	while(g_systimer - timestart < timeout_ms && counter < bufferlen - 1) {
		if(usart_read(usart, &rx) == 0) {
			//timestart = g_systimer; // reset timeout
			buffer[counter++] = rx;
		}
	}
	buffer[counter] = 0x00;
	return counter;
}

void usart_send_command(Usart *usart, char buffer_rx[], int bufferlen, char buffer_tx[], int timeout) {
	usart_put_string(usart, buffer_tx);
	usart_get_string(usart, buffer_rx, bufferlen, timeout);
}

void usart_log(char* name, char* log) {
	usart_put_string(USART1, "[");
	usart_put_string(USART1, name);
	usart_put_string(USART1, "] ");
	usart_put_string(USART1, log);
	usart_put_string(USART1, "\r\n");
}

void hc05_config_server(void) {
	usart_serial_options_t config;
	config.baudrate = 9600;
	config.charlength = US_MR_CHRL_8_BIT;
	config.paritytype = US_MR_PAR_NO;
	config.stopbits = false;
	usart_serial_init(USART0, &config);
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
	
	 // RX - PB0  TX - PB1 
	 pio_configure(PIOB, PIO_PERIPH_C, (1 << 0), PIO_DEFAULT);
	 pio_configure(PIOB, PIO_PERIPH_C, (1 << 1), PIO_DEFAULT);
}

int hc05_server_init(void) {
	char buffer_rx[128];
	usart_send_command(USART0, buffer_rx, 1000, "AT", 1000);
	usart_send_command(USART0, buffer_rx, 1000, "AT", 1000);	
	usart_send_command(USART0, buffer_rx, 1000, "AT+NAMEFolCos", 1000);
	usart_log("hc05_server_init", buffer_rx);
	usart_send_command(USART0, buffer_rx, 1000, "AT", 1000);
	usart_send_command(USART0, buffer_rx, 1000, "AT+PIN1337", 1000);
	usart_log("hc05_server_init", buffer_rx);
}

void BUT_init(void){
	//TERMINAR INIT PARA TODOS OS BOTOES
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	
	pmc_enable_periph_clk(BUT3_PIO_ID);
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);

	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_FALL_EDGE, Button2_Handler);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_FALL_EDGE, Button3_Handler);
	
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 1);
	
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 1);
};

void Button0_Handler(void){
	flag_but0 = 1;
}

void Button1_Handler(void){
	flag_but1 = 1;
}

void Button2_Handler(void){
	flag_but2 = 1;
}

void Button3_Handler(void){
	flag_but3 = 1;
}

void Button4_Handler(void){
	flag_but4 = 1;
}

void Button5_Handler(void){
	flag_but5 = 1;
}

void Button6_Handler(void){
	flag_but6 = 1;
}

void Button7_Handler(void){
	flag_but7 = 1;
}

void Button8_Handler(void){
	flag_but8 = 1;
}

void Button9_Handler(void){
	flag_but9 = 1;
}

void Button10_Handler(void){
	flag_but10 = 1;
}

void Button11_Handler(void){
	flag_but11 = 1;
}

int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000); // 1 ms
	config_console();
	
	#ifndef DEBUG_SERIAL
	usart_put_string(USART1, "Inicializando...\r\n");
	usart_put_string(USART1, "Config HC05 Server...\r\n");
	hc05_config_server();
	hc05_server_init();
	#endif
	
	char button0 = '0';
	char button1 = '0';
	char button2 = '0';
	char button3 = '0';
	char button4 = '0';
	char button5 = '0';
	char button6 = '0';
	char button7 = '0';
	char button8 = '0';
	char button9 = '0';
	char button10 = '0';
	char button11 = '0';
	char eof = 'X';
	char buffer[1024];
	
	while(1) {
		if(flag_but0) {
			button0 = '1';
			flag_but0 = 0;
		} else {
			button0 = '0';
		}
		if(flag_but1) {
			button1 = '1';
			flag_but1 = 0;
			} else {
			button1 = '0';
		}
		if(flag_but2) {
			button2 = '1';
			flag_but2 = 0;
			} else {
			button2 = '0';
		}
		if(flag_but3) {
			button3 = '1';
			flag_but3 = 0;
			} else {
			button3 = '0';
		}
		if(flag_but4) {
			button4 = '1';
			flag_but4 = 0;
			} else {
			button4 = '0';
		}
		if(flag_but5) {
			button5 = '1';
			flag_but5 = 0;
			} else {
			button5 = '0';
		}
		if(flag_but6) {
			button6 = '1';
			flag_but6 = 0;
			} else {
			button6 = '0';
		}
		if(flag_but7) {
			button7 = '1';
			flag_but7 = 0;
			} else {
			button7 = '0';
		}
		if(flag_but8) {
			button8 = '1';
			flag_but8 = 0;
			} else {
			button8 = '0';
		}
		if(flag_but9) {
			button9 = '1';
			flag_but9 = 0;
			} else {
			button9 = '0';
		}
		if(flag_but10) {
			button10 = '1';
			flag_but10 = 0;
			} else {
			button10 = '0';
		}
		if(flag_but11) {
			button11 = '1';
			flag_but11 = 0;
			} else {
			button11 = '0';
		}
		
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button0);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button1);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button2);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button3);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button4);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button5);		
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button6);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button7);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button8);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button9);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button10);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, button11);
		while(!usart_is_tx_ready(UART_COMM));
		usart_write(UART_COMM, eof);
	}
}
