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
volatile uint8_t flag_change = 0;

volatile uint8_t value_but0 = 0;
volatile uint8_t value_but1 = 0;
volatile uint8_t value_but2 = 0;
volatile uint8_t value_but3 = 0;
volatile uint8_t value_but4 = 0;
volatile uint8_t value_but5 = 0;
volatile uint8_t value_but6 = 0;
volatile uint8_t value_but7 = 0;
volatile uint8_t value_but8 = 0;
volatile uint8_t value_but9 = 0;
volatile uint8_t value_but10 = 0;
volatile uint8_t value_but11 = 0;

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
	usart_serial_write_packet(usart, str, strlen(str));
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

// void Button0_Handler(void){
// 	flag_but0 = 1;
// 	value_but0 = !pio_get(BUT0_PIO, PIO_INPUT, BUT0_PIO_IDX_MASK);
// }
// 
// void Button1_Handler(void){
// 	flag_but1 = 1;
// 	value_but1 = !pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK);
// }

void Button2_Handler(void){
	flag_but2 = 1;
	value_but2 = !pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK);
}

void Button3_Handler(void){
	flag_but3 = 1;
	value_but3 = !pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK);
}

// void Button4_Handler(void){
// 	flag_but4 = 1;
// 	value_but4 = !pio_get(BUT4_PIO, PIO_INPUT, BUT4_PIO_IDX_MASK);
// }
// 
// void Button5_Handler(void){
// 	flag_but5 = 1;
// 	value_but5 = !pio_get(BUT5_PIO, PIO_INPUT, BUT5_PIO_IDX_MASK);
// }
// 
// void Button6_Handler(void){
// 	flag_but6 = 1;
// 	value_but6 = !pio_get(BUT6_PIO, PIO_INPUT, BUT6_PIO_IDX_MASK);
// }
// 
// void Button7_Handler(void){
// 	flag_but7 = 1;
// 	value_but7 = !pio_get(BUT7_PIO, PIO_INPUT, BUT7_PIO_IDX_MASK);
// }
// 
// void Button8_Handler(void){
// 	flag_but8 = 1;
// 	value_but8 = !pio_get(BUT8_PIO, PIO_INPUT, BUT8_PIO_IDX_MASK);
// }
// 
// void Button9_Handler(void){
// 	flag_but9 = 1;
// 	value_but9 = !pio_get(BUT9_PIO, PIO_INPUT, BUT9_PIO_IDX_MASK);
// }
// 
// void Button10_Handler(void){
// 	flag_but10 = 1;
// 	value_but10 = !pio_get(BUT10PIO, PIO_INPUT, BUT10_PIO_IDX_MASK);
// }
// 
// void Button11_Handler(void){
// 	flag_but11 = 1;
// 	value_but11 = !pio_get(BUT11_PIO, PIO_INPUT, BUT11_PIO_IDX_MASK);
// }

void BUT_init(void){
	//TERMINAR INIT PARA TODOS OS BOTOES
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	
	pmc_enable_periph_clk(BUT3_PIO_ID);
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);

	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_EDGE, Button2_Handler);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_EDGE, Button3_Handler);
	
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 1);
	
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 1);
};

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
	BUT_init();
	
	char eof = 'X';
	char buffer[1024];
	
	while(1) {
		char button0 = 'N';
		char button1 = 'N';
		char button2 = 'N';
		char button3 = 'N';
		char button4 = 'N';
		char button5 = 'N';
		char button6 = 'N';
		char button7 = 'N';
		char button8 = 'N';
		char button9 = 'N';
		char button10 = 'N';
		char button11 = 'N';
		
		if(flag_but0) {
			if(value_but0){
				button0 = '1';
			}
			else{
				button0 = '0';
			}
			flag_but0 = 0;
			flag_change = 1;
		}
		if(flag_but1) {
			if(value_but1){
				button1 = '1';
			}
			else{
				button1 = '0';
			}
			flag_but1 = 0;
			flag_change = 1;
		}
		if(flag_but2) {
			if(value_but2){
				button2 = '1';
			}
			else{
				button2 = '0';
			}
			flag_but2 = 0;
			flag_change = 1;
		}
		if(flag_but3) {
			if(value_but3){
				button3 = '1';
			}
			else{
				button3 = '0';
			}
			flag_but3 = 0;
			flag_change = 1;
		}
		if(flag_but4) {
			if(value_but4){
				button4 = '1';
			}
			else{
				button4 = '0';
			}
			flag_but4 = 0;
			flag_change = 1;
		}
		if(flag_but5) {
			if(value_but5){
				button5 = '1';
			}
			else{
				button5 = '0';
			}
			flag_but5 = 0;
			flag_change = 1;
		}
		if(flag_but6) {
			if(value_but6){
				button6 = '1';
			}
			else{
				button6 = '0';
			}
			flag_but6 = 0;
			flag_change = 1;
		}
		if(flag_but7) {
			if(value_but7){
				button7 = '1';
			}
			else{
				button7 = '0';
			}
			flag_but7 = 0;
			flag_change = 1;
		}
		if(flag_but8) {
			if(value_but8){
				button8 = '1';
			}
			else{
				button8 = '0';
			}
			flag_but8 = 0;
			flag_change = 1;
		}
		if(flag_but9) {
			if(value_but9){
				button9 = '1';
			}
			else{
				button9 = '0';
			}
			flag_but9 = 0;
			flag_change = 1;
		}
		if(flag_but10) {
			if(value_but10){
				button10 = '1';
			}
			else{
				button10 = '0';
			}
			flag_but10 = 0;
			flag_change = 1;
		}
		if(flag_but11) {
			if(value_but11){
				button11 = '1';
			}
			else{
				button11 = '0';
			}
			flag_but11 = 0;
			flag_change = 1;
		}
		if(flag_change){
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
			flag_change = 0;
		}
		else{
			pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
		}
	}
}
