#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "sniffles.pio.h"

// Set up a PIO state machine to shift in serial data, sampling with an
// external clock, and push the data to the RX FIFO, 16 bits at a time.

#define BASE_PIN 4
// let's have base be d0
// base+1 be clk
// base+2 be lat

#define BUF_SIZE 16

int main() {
    stdio_init_all();

    // Load the clocked_input program, and configure a free state machine
    // to run the program.
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &sniffles_program);
    uint sm = pio_claim_unused_sm(pio, true);
    sniffles_program_init(pio, sm, offset, BASE_PIN);

    const char unpressed = '-';
    const char pressed [BUF_SIZE];
    strncpy(pressed,"BYsSUDLRXAlr1234",BUF_SIZE);
    uint16_t buttons = 0;
    char display[BUF_SIZE];

    while (true)
    {
	buttons = pio_sm_get(pio, sm);
	for(int x = BUF_SIZE-1;x >= 0;x--)
	{
		display[x] = (buttons & 1) ? unpressed : pressed[x];
		buttons >>= 1;
	}
        printf("%s\n",display);
        //sleep_ms(1000); // just temp to not overwhelm CDC
    }

    return 0;
}
