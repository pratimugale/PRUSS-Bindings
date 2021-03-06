/*
 * Copyright (C) 2016-2018 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//This Firmware Code serves as an input tool to tell the PRU0 about the next command sent by the ARM to control the stepper motor.
//This Firmware Code also serves as an example to read and deserialize the data written in the message buffer.

#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#include <string.h>
#include "resource_table_1.h"

volatile register uint32_t __R31;

#define PRU_SRAM 0x00010000

// Host-1 Interrupt sets bit 31 in register R31 - Host-1 Interrupt is used for RPMsg 
#define HOST_INT			((uint32_t) 1 << 31)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * In this example, PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST			18
#define FROM_ARM_HOST			19

/*
 * Using the name 'pruss-api' will probe the pruss_api.ko driver found
 * /lib/modules/$(uname -r)
 */

#define CHAN_NAME			"pruss-api"

#define CHAN_DESC			"Channel 1"
#define CHAN_PORT			1

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4

// RPMSG_BUF_SIZE = 512 bytes; pru_rpmsg_hdr header is of 16 bytes (minus the data[0] part); so maximum message length is of 496 bytes.
void* payload[RPMSG_BUF_SIZE];

// FOR INTERRUPT FROM PRU0
// Host-0 Interrupt sets bit 30 in register R31 - Host-0 Interrupt is used for interrupt from PRU0
#define HOST0_MASK              ((uint32_t) 1 << 30)   
#define PRU0_PRU1_EVT           (16)        // PRU-ICSS System Event 16 for PRU0-PRU1 interrupt.

void main(void)
{
	struct pru_rpmsg_transport transport;
	uint16_t src, dst, len;
	volatile uint8_t *status;

	/* AM335x must enable OCP(Open Core Protocol) master port access in order for the PRU to
	 * read external memories. - As RPMsg basically reads that memory.
	 */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

        volatile uint32_t* sram_pointer = (volatile uint32_t *) PRU_SRAM;
        int i = 0;
        char* outgoingMessage = "done\n";

	/* Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us */
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Make sure the Linux drivers are ready for RPMsg communication */
	status = &resourceTable.rpmsg_vdev.status;
	while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

	/* Initialize the RPMsg transport structure */
	pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

	/* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
	while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
	while (1) {
		/* Check bit 31 of register R31 to see if the ARM has kicked us */
		if (__R31 & HOST_INT) {
			/* Clear the event status */
		    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	            // Receive one message at a time
		    while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len)==PRU_RPMSG_SUCCESS){
                     
                        uint32_t x = *(uint32_t *) payload;  // Type cast void* payload to get the 4 byte raw integer

                        *(sram_pointer+i) = x;      // Write to address-offset 'i' in shared memory
                        i++;                        // Next integer location i.e. address-offset+=4


                        // After Writing all three integers to the PRU SRAM at i=0, 1, 2 offsets,
                        // Check for interrupt from PRU0
                        if (i == 3){
                            CT_CFG.GPCFG0 = 0x0000;

                            // Loop until interrupt on Host1 is detected i.e. check bit 30 of R31.
                            while (1){
                                if (__R31 & HOST0_MASK){
                                    // Clear interrupt event 
                                    CT_INTC.SICR = 16; 
                                    // Delay to ensure the event is cleared in INTC
                                    __delay_cycles(5);
                
                                    // Notify the ARM that PRU0 has completed its work of sending pulses.
                                    pru_rpmsg_send(&transport, dst, src, outgoingMessage, strlen(outgoingMessage));
                                    break;
                                }
                            }
                        }
                    }
		}
	}
}
