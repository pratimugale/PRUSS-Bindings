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

//This Firmware Code serves as an example to read and deserialize the data written in the message buffer.

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

/* Host-1 Interrupt sets bit 31 in register R31 */
#define HOST_INT			((uint32_t) 1 << 31)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * In this example, PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST			18
#define FROM_ARM_HOST			19

/*
 * Using the name 'rpmsg-client-sample' will probe the RPMsg sample driver
 * found at linux-x.y.z/samples/rpmsg/rpmsg_client_sample.c
 *
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */

#define CHAN_NAME			"rpmsg-pru"

#define CHAN_DESC			"Channel 31"
#define CHAN_PORT			31

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4

// RPMSG_BUF_SIZE = 512 bytes; pru_rpmsg_hdr header is of 16 bytes (minus the data[0] part); so maximum message length is of 496 bytes.
void* payload[RPMSG_BUF_SIZE];

void main(void)
{
	struct pru_rpmsg_transport transport;
	uint16_t src, dst, len;
	volatile uint8_t *status;

	/* AM335x must enable OCP(Open Core Protocol) master port access in order for the PRU to
	 * read external memories. RPMsg basically reads the external memories.
	 */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;


        // Pointer to uint8_t to store the actual wave sampled values.
        volatile uint8_t* sram_pointer_8bit = (volatile uint8_t *) PRU_SRAM;

        // Pointer to uint32_t to store noOfSamples whose size is greater than 1 byte
        volatile uint32_t* sram_pointer_32bit = (volatile uint32_t *) PRU_SRAM;

        // Counter to keep track of address offsets.
        int i = 0;


	// Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us 
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	// Make sure the Linux drivers are ready for RPMsg communication 
	status = &resourceTable.rpmsg_vdev.status;
	while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

	// Initialize the RPMsg transport structure 
	pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

	// Create the RPMsg channel between the PRU and ARM user space using the transport structure. 
	while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
	while (1) {
		// Check bit 31 of register R31 to see if the ARM has kicked us 
		if (__R31 & HOST_INT) {
			// Clear the event status 
			CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

			// Receive one message
			while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len)==PRU_RPMSG_SUCCESS){
                        
                            if (i == 0){
                                // Cast the void pointer to uint32_t* and then dereference it.
                                uint32_t noOfSamples = *(uint32_t *) payload;

                                // Store the incoming noOfSamples into the PRU SRAM at locations: 0x00010000 - 0x00010003.
                                *(sram_pointer_32bit) = noOfSamples; 

                                // Increment 'i' by 4 because the next waveform sample values are 1-byte uint8_t values i.e. increment by 32/8 = 4.
                                i+=4;
                            }
                            else{
                                // Cast the void pointer to uint8_t* and then dereference it.
                                uint8_t samples = *(uint8_t *) payload;

                                // Store the incoming wave sampled data into the PRU SRAM starting from 0x00010004.
                                *(sram_pointer_8bit+i) = samples; 

                                // Increment offset.
                                i++;
                            }
                        }
		}
	}
}
