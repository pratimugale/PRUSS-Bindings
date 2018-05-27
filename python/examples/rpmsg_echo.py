import pypruss

pru = pypruss.PRU(0, './firmware_examples/rpmsg_echo/gen/rpmsg_echo.out')
pru.send_msg("Hello PRU0")
pru.get_msg()
