#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>               // Interrupt Controller
#include "resource_table_empty.h"   // Resource  Table

#define PRU_SHARED 0x00010000       // PRU Shared Memory Address

volatile register uint32_t __R31;   // Directly access R31 to generate interrupts

#define PRU0_PRU1_EVT       (16)    // Defining PRU0 to PRU 1 Interrupt
#define PRU0_PRU1_TRIGGER   (__R31 = (PRU0_PRU1_EVT - 16) | (1 << 5))

extern void start1(void);
extern void start2(void);
extern void start3(void);

/* INTC configuration
 * We are going to map User event 16 to Host 0
 * PRU1 will then wait for r31 bit 30 (designates Host 0) to go high
 * */

void configIntc(void)
{
	/* Clear any pending PRU-generated events */
	__R31 = 0x00000000;
	/* Map event 16 to channel 0 */
	CT_INTC.CMR4_bit.CH_MAP_16 = 0; 
	/* Map channel 0 to host 0 */
	CT_INTC.HMR0_bit.HINT_MAP_0 = 0; 
	/* Ensure event 16 is cleared */
	CT_INTC.SICR = 16; 
	/* Enable event 16 */
	CT_INTC.EISR = 16; 
	/* Enable Host interrupt 1 */
	CT_INTC.HIEISR |= (0 << 0); 
// DON'T DO ->        __R31 |= ((uint32_t) 1 << 30);
	// Globally enable host interrupts
	CT_INTC.GER = 1; 
}

void main(void){

    // Configure GPI and GPO as Mode0 - Direct Connect
    CT_CFG.GPCFG0 = 0x0000;

    configIntc();

    start1();
    start2();
    start3();
    // All Pulses have now been sent
    
    PRU0_PRU1_TRIGGER;

    __halt();
}
