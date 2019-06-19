import pruss
from pruss import *

pss = PRUSS.get()
p1 = pss.pru1
p1.enable()

s = input("Enter a string to send to rpmsg_pru31 \n")
p1.sendMsg(s)

print("Message received: ")
print (p1.getMsg())
p1.disable()
pss.shutDown()
