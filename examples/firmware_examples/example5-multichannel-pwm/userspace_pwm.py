# this program function must be added to the prussd.py daemon and then a seperate userspace program will be written in cpp

import struct

frequency = input("Frequency of PWM in Hz (from 1Hz to 1Mhz): ")
multiplier = 1000000.0/frequency

duty_cycle = input("Duty Cycle of PWM ")

on_samples = duty_cycle * 100
total_samples = 100

on_samples *= multiplier
total_samples *= multiplier

print("On cycles: ", int(on_samples))
print("Total cycles: ", int(total_samples))

data = [int(on_samples), int(total_samples)]

if isinstance(data[0], int):
    with open('/dev/rpmsg_pru31', 'wb') as f: 
        f.write(struct.pack('i'*2, *data))
        f.close()
elif isinstance(value, str):
    with open('/dev/rpmsg_pru31', 'w') as f:
        f.write(data)
        f.close()
else:
    raise TypeError('Can only write string or int')
