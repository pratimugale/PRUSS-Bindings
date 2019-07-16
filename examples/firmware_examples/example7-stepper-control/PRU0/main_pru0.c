#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>               // Interrupt Controller
#include "resource_table_empty.h"   // Resource  Table

#define PRU_SHARED 0x00010000       // PRU Shared Memory Address

volatile register uint32_t __R31;   // Directly access R31 to generate interrupts

#define PRU0_PRU1_EVT       (16)    // Defining PRU0 to PRU 1 Interrupt
#define PRU0_PRU1_TRIGGER   (__R31 = (PRU0_PRU1_EVT - 16) | (1 << 5))

extern void start(void);

/* INTC configuration
 * We are going to map User event 16 to Host 1
 * PRU1 will then wait for r31 bit 31 (designates Host 1) to go high
 * */

void configIntc(void)
{
	/* Clear any pending PRU-generated events */
	__R31 = 0x00000000;
	/* Map event 16 to channel 1 */
	CT_INTC.CMR4_bit.CH_MAP_16 = 1; 
	/* Map channel 1 to host 1 */
	CT_INTC.HMR0_bit.HINT_MAP_1 = 1; 
	/* Ensure event 16 is cleared */
	CT_INTC.SICR = 16; 
	/* Enable event 16 */
	CT_INTC.EISR = 16; 
	/* Enable Host interrupt 1 */
	CT_INTC.HIEISR |= (1 << 0); 
	// Globally enable host interrupts
	CT_INTC.GER = 1; 
}

void main(void){
    configIntc();

    PRU0_PRU1_TRIGGER;

    start();
    // All Pulses have now been sent

    // Configure GPI and GPO as Mode0 - Direct Connect
    CT_CFG.GPCFG0 = 0x0000;


    __halt();
}
