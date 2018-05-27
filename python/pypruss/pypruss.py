import os
import sys
import subprocess
import io
import select
import mmap
import struct

#State definitions
PRU_OFFLINE = 0
PRU_STOPPED = 1
PRU_RUNNING = 2
PRU_HALTED  = 3

#Memory definitions
PRU_ICSS     = 0x4A300000
PRU_ICSS_LEN = 512*1024
PRU_DRAM0    = 0x00000000
PRU_DRAM1    = 0x00002000
PRU_SRAM     = 0x00010000

class PRU(object):
    """
    The main class which encapsulates a PRU

    :param number: The PRU number, 0/1 corresponding to PRU0 and PRU1 in the PRUSS
    :type number: int
    :param fw: Optional argument. If specified, load the firmware image onto the PRU with the given number
    :type fw: str
    """

    def __init__(self, number=0, fw=None):
        if number != 0 and number != 1:
            sys.exit("Invalid PRU number (Can be 0 or 1 only)")
        self.number = number
        self.modprobe()
        self.reset()
        if fw is not None:
            self.load(fw)
		
    @classmethod 
    def modprobe(cls):
        """ Check if the Remoteproc Driver is loaded. If not, Load the driver module. """
        if subprocess.call("/sbin/modprobe pru_rproc", shell=True) :
    	    sys.exit("Probing Remoteproc Driver Failed")
    
    @classmethod
    def modunprobe(cls):
        """ Remove the Remoteproc Driver module. """
        if subprocess.call("/sbin/modprobe -r pru_rproc", shell=True) :
            sys.exit("Failed removing the Remoteproc Driver")

    def enable(self):
        """ Enable/Start the PRU. """
        try:
            with open('/sys/class/remoteproc/remoteproc'+str(self.number+1)+'/state', 'w') as fd:
                fd.write('start')
                fd.close()
            self.state = PRU_RUNNING
        except OSError:
            pass 

    def disable(self):
        """ Disable/Stop the PRU. """
        try:
            with open('/sys/class/remoteproc/remoteproc'+str(self.number+1)+'/state', 'w') as fd:
                fd.write('stop')
                fd.close();
            self.state = PRU_STOPPED
        except OSError:
            pass
	
    def reset(self):
        """ Reset the PRU. """
        self.disable()
        self.enable()
    	
    def load(self, filepath):
        """
        Load the firmware image specified by @filepath and restarts the PRU

        :param filepath: The firmware image to be loaded with the path to the file
        :type filepath: str
        """
        self.disable()
        if subprocess.call('cp '+filepath+' /lib/firmware/am335x-pru'+str(self.number)+'-fw', shell=True):
            print("Error loading firmware on PRU"+str(self.number))
        self.enable()

        # RPMsg functions
    def send_msg(self, message, channel=None):
        """
        Send a message to the PRU through the @channel

        :param message: The message to be sent to the PRU
        :type message: str
        :param channel: The channel which is created by the RPMsg driver and specified in the firmware
                    If no channel is specified, send the message to channel 30/31 depending on the PRU number
        :type channel: int
        """
        if channel is None:
            channel = '3'+str(self.number)
        devpath = '/dev/rpmsg_pru'+str(channel)
        if os.path.exists(devpath):
            with open(devpath, 'w') as fd:
                fd.write(message+'\n');
                fd.close()
        else:
            print("Error Sending Message on "+channel+": Channel not found")

    def get_msg(self, channel=None):
        """
        Recieve the message from the PRU through the @channel

        :param channel: The channel through which the PRU will send the message. It will be created by the RPMsg driver
                    and specified in the firmware. If no channel is specified, Recieve the message on 
                    channel 30/31 for PRU0/PRU1 respectively
        :type channel: int
        """
        if channel is None:
            channel = '3'+str(self.number)
        devpath = '/dev/rpmsg_pru'+str(channel)
        if os.path.exists(devpath):
            with io.open(devpath, 'r') as fd:
                data = fd.readline().strip()
                fd.close()
            return data
        else:
            print("Error Recieving Message from "+channel+": Channel not found")

    def wait_for_event(self, channel=None):
        """
        Wait for an event from the PRU on the @channel. The function blocks until a signal/message is recieved on the
        channel

        :param channel: The channel on which the event will be signaled by the PRU. If no If no channel is specified, the 
                    wait for event on channel 30/31 for PRU0/PRU1 respectively
        :type channel: int
        """
        if channel is None:
            channel = '3'+str(self.number)
        devpath = '/dev/rpmsg_pru'+str(channel)
        if os.path.exists(devpath):
            with open(devpath, 'r') as fd:
                fd.read(1);
                fd.close()
        else:
            print("Error : Channel not found")
            
	#Memory functions
    def mem_writeint(self, address, data, shared=False):
        """
        Write @data, and integer(4 bytes) to the memory offset specified by @address of the PRU Data/Shared Memory.

        :param shared: If True, write to the PRU Shared Memory, else write to the respective Data Memories.
                    Defaults to False.
        :param data: An int, the data to be written.
        :param address: The address offset to the Respective Data Memory/Shared Memory
        """
        base = PRU_SRAM if shared else (PRU_DRAM1 if self.number else PRU_DRAM0)
        with open('/dev/mem', 'r+b') as fd:
            pru_mem = mmap.mmap(fd.fileno(), PRU_ICSS_LEN, offset=PRU_ICSS)
            pru_mem[base+address: base+address+4] = struct.pack('L', data)
            pru_mem.close()
            fd.close()
	
    def mem_readint(self, address, shared=False):
        """
        Read an int(4 bytes) from the memory offset specified by @address of the PRU Data/Shared Memory.

        :param shared: If True, read from the PRU Shared Memory, else Read from the respective Data Memories.
                    Defaults to False.
        :type shared: boolean
        :param address: The address offset to the Respective Data Memory/Shared Memory.
        :return: An int, the data read from the memory.
        """
        base = PRU_SRAM if shared else (PRU_DRAM1 if self.number else PRU_DRAM0)
        with open('/dev/mem', 'r+b') as fd:
            pru_mem = mmap.mmap(fd.fileno(), PRU_ICSS_LEN, offset=PRU_ICSS)
            data = struct.unpack('L', pru_mem[base+address: base+address+4])[0]
            pru_mem.close()
            fd.close()
        return data
    	
    def mem_writebyte(self, address, data, shared=False):
        """
        Write @data, a byte to the memory offset specified by @address of the PRU Data/Shared Memory.

        :param shared: If True, write to the PRU Shared Memory, else write to the respective Data Memories.
                    Defaults to False.
        :type shared: boolean
        :param data: A byte, the Data to be written.
        :param address: The address offset to the Respective Data Memory/Shared Memory.
        """
        base = PRU_SRAM if shared else (PRU_DRAM1 if self.number else PRU_DRAM0)
        with open('/dev/mem', 'r+b') as fd:
            pru_mem = mmap.mmap(fd.fileno(), PRU_ICSS_LEN, offset=PRU_ICSS)
            pru_mem[base+address: base+address+1] = struct.pack('B', data)
            pru_mem.close()
            fd.close()
	
    def mem_readbyte(self, address, shared=False):
        """
        Read a byte from the memory offset specified by @address of the PRU Data/Shared Memory.

        :param shared: If True, read from the PRU Shared Memory, else read from the respective Data Memories.
                    Defaults to False.
        :type shared: boolean
        :param address: The address offset to the Respective Data Memory/Shared Memory.
        :return: A byte, the data read from the memory.
        """
        base = PRU_SRAM if shared else (PRU_DRAM1 if self.number else PRU_DRAM0)
        with open('/dev/mem', 'r+b') as fd:
            pru_mem = mmap.mmap(fd.fileno(), PRU_ICSS_LEN, offset=PRU_ICSS)
            data = struct.unpack('B', pru_mem[base+address: base+address+1])[0]
            pru_mem.close()
            fd.close()
        return data
