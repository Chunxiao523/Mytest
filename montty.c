#include <stdio.h>
#include <stdlib.h>
#include <terminals.h>
#include <threads.h>
#include <hardware.h>

#define MAX_BUFF_LEN 1024;

/* Echo buffer */
char echo_buff[NUM_TERMINALS][MAX_BUFF_LEN];
/* Echo Buffer address */
int echo_buff_addr[NUM_TERMINALS];

/* Input buffer */
char input_buff[NUM_TERMINALS][MAX_BUFF_LEN];
/* Input buffer address */
int input_buff_addr[NUM_TERMINALS];

/* Output buffer, pointer to the next character address */
int output_buff_addr[NUM_TERMINALS];


/* State array*/
bool state[NUM_TERMINALS];
#define WAITING 0;
#define READING 1;
#define WRITING 2;

/* Interrupt handler1
 * When the receipt of a new character from a keyboard completes,
 * the terminal hardware signals a receive interrupt.
 */
void ReceiveInterrupt(int term) {
	Declare_Monitor_Entry_Procedure();
	char character = ReadDataRegister(term);
	int input_addr = input_buff_addr[term];
	int echo_addr = echo_buff_addr[term];
	/* put this character into the input buffer */
	if (character == '\b') {
		if (input_buff_addr[term] != 0) { 
			input_buff_add[term] --;
		}
		echo_buff[term][echo_addr] = '\b \b';
	} else {
		if (character == '\r') {
			input_buff[term][input_addr] = '\n';
			input_buff_addr[term] ++;
			echo_buff[term][echo_addr] = '\r\n';
			echo_buff_addr[term] ++;
		}
	}	
	WriteDataRegister(term, echo_buffer[echo_addr]);
}
/*Interrupt handler2
when the transmission of a character to a terminal completes, the terminal hardware signals a transmit interrupt.*/
void TransmitInterrupt(int term) {
}

/* 
 * Read characters from terminal and place into buf until buflen chars
 * have been read or a '\n' is read. 
 */
int ReadTerminal(int term, char *buf, int buflen){
	if (buflen < 0) return -1;
	read_buff_add[term] = buf;
	if (*read_buff_add[term] == '\n' || read_buff_add[term] == (buf + buflen) {
		//do something
	} else {
		*buf
	}
	return 0;
}

/* 
 * Copy the character from term into buf
 * Called by some user process (thread) whenever it wants to output something. Roughly equivalent to printf.
 * term: terminal number
 */
int WriteTerminal(int term, char *buf, int buflen) {
	Declare_Monitor_Entry_Procedure();

	if (buflen < 0) return 0;
	if (buf + buflen > MAX_BUFF_LEN) {
		return -1;
	}
	if (output_buff_addr[term] > (buflen+buf)) {
		return buflen;
	} else if (state[term] == WAITING){
		state[term] = WRITING;
		output_buff_addr[term] ++;
		WriteDataRegister(term, *(output_buff_addr[term]));
	//the characters must be transmitted by your terminal device driver to the terminal one at a time
	}	
	return buflen;
}

int InitTerminal(int term) {
	if (InitHardware(term) == 0) {
		echo_buff[term] = 0;
		echo_buff_addr[term] = 0;

		input_buff[term] = 0;
		input_buff_addr[term] = 0;

		output_buff_addr[term] = 0;

		state[term] = 0;
		return 0;
	} else {
		return -1;
	}
	
}

int InitTerminalDriver() {
	return 0;
}

int TerminalDriverStatistics(struct termstat *stats) {
	return 0;
}


