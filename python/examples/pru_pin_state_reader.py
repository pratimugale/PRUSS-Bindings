import pypruss

pru = pypruss.PRU(1, '../examples/firmware_examples/pru_pin_state_reader')
pru.send_msg("S")
pru.get_msg()
