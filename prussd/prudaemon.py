#!/usr/bin/python3

import socketserver
import subprocess
import errno
import select
import os

class MyRequestHandler(socketserver.StreamRequestHandler):
    """
    The request handler class for our socket server
    Overrides the handle method of the BaseRequestHandler class to implement communication with the socket file.
    """

    def handle(self):

        # self.rfile is a file-like object created by the StreamRequestHandler, can use normal file handling methods
        self.data = self.rfile.readline().strip()
        #decode bytes into string and get command args
        request = self.data.decode("utf-8").split()
        reply = 0

        if request[0] == "probe_rproc":
            try:
                p = subprocess.Popen(["/sbin/modprobe", "pru_rproc"])
            
            except OSError as e:
                reply = e.errno

        elif request[0] == "probe_rpmsg":
            try:
                p = subprocess.Popen(["/sbin/modprobe", "pru_rpmsg"])
            
            except OSError as e:
                reply = e.errno
                    

        elif request[0] == "unprobe_rproc":
            try:
                p = subprocess.Popen(["/sbin/modprobe", "-r", "pru_rproc"])
            
            except OSError as e:
                reply = e.errno
                
        elif request[0] == "unprobe_rpmsg":
            try:
                p = subprocess.Popen(["/sbin/modprobe", "-r", "pru_rpmsg"])
            
            except OSError as e:
                reply = e.errno

        elif request[0] == "enable0":
            try:
                with open('/sys/class/remoteproc/remoteproc1/state', 'w') as fd:               
                    fd.write('start')
                fd.close()
            
            except (OSError, IOError) as e:
                reply = e.errno
        
        elif request[0] == "enable1":
            try:
                with open('/sys/class/remoteproc/remoteproc2/state', 'w') as fd:               
                    fd.write('start')
                fd.close()
            
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "disable0":
            try:
                with open('/sys/class/remoteproc/remoteproc1/state', 'w') as fd:               
                    fd.write('stop')
                fd.close()
            
            except OSError as e:
                reply = e.errno
       
        elif request[0] == "disable1":
            try:
                with open('/sys/class/remoteproc/remoteproc2/state', 'w') as fd:               
                    fd.write('stop')
                fd.close()
            
            except OSError as e:
                reply = e.errno

        elif request[0] == "getstate0":
            try:
                with open('/sys/class/remoteproc/remoteproc1/state', 'r') as fd:
                    reply = fd.readline()
                fd.close()
            
            except OSError as e:
                reply = e.errno
        
        elif request[0] == "getstate1":
            try:
                with open('/sys/class/remoteproc/remoteproc2/state', 'r') as fd:
                    reply = fd.readline()
                fd.close()
            
            except OSError as e:
                reply = e.errno

        elif request[0] == "pause0":
            try:
                p = subprocess.Popen(["/bin/sh", "-c", "echo 1 > /sys/kernel/debug/remoteproc/remoteproc1/single_step"])
                
            except OSError as e:
                reply = e.errno
        
        elif request[0] == "pause1":
            try:
                p = subprocess.Popen(["/bin/sh", "-c", "echo 1 > /sys/kernel/debug/remoteproc/remoteproc2/single_step"])
                
            except OSError as e:
                reply = e.errno
        elif request[0] == "resume0":
            try:
                p = subprocess.Popen(["/bin/sh", "-c", "echo 0 > /sys/kernel/debug/remoteproc/remoteproc1/single_step"])
                
            except OSError as e:
                reply = e.errno
        
        elif request[0] == "resume1":
            try:
                p = subprocess.Popen(["/bin/sh", "-c", "echo 0 > /sys/kernel/debug/remoteproc/remoteproc2/single_step"])
                
            except OSError as e:
                reply = e.errno

        elif request[0] == "showregs0":
            try:
                with open('/sys/kernel/debug/remoteproc/remoteproc1/regs', 'r') as fd:
                    reply = fd.read()    
                fd.close()
            
            except OSError as e:
                reply = e.errno
        
        elif request[0] == "showregs1":
            try:
                with open('/sys/kernel/debug/remoteproc/remoteproc2/regs', 'r') as fd:
                    reply = fd.read()
                fd.close()
            
            except OSError as e:
                reply = e.errno

        elif request[0] == "load0":
            try:
                p = subprocess.Popen(["cp", "/tmp/pru0", "/lib/firmware/am335x-pru0-fw"])
                reply = 0
            
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "load1":
            try:
                p = subprocess.Popen(["cp", "/tmp/pru1", "/lib/firmware/am335x-pru1-fw"])
                reply = 0
            
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "sendmsg":
            try:
                request[1] = int(request[1]) #channel number, throws ValueError if not int
                with open('/dev/rpmsg_pru'+str(request[1]), 'w') as fd:
                    fd.write(request[2]+"\n")
                fd.close()
                reply = 0
            
            except ValueError as e:
                reply = errno.EINVAL
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "getmsg":
            try:
                request[1] = int(request[1]) #channel number, throws ValueError if not int
                reply = ""
                with open('/dev/rpmsg_pru'+str(request[1])) as fd:
                    #timeout 0 in select <=> non blocking read
                    while fd in select.select([fd], [], [], 0)[0]:
                        reply += fd.readline()
                fd.close()
            
            except ValueError as e:
                reply = errno.EINVAL
            except (OSError, IOError) as e:
                reply = e.errno
            
        elif request[0] == "eventwait":
            try:
                request[1] = int(request[1])
                with open('/dev/rpmsg_pru'+ str(request[1])) as fd:
                    #if first return arg is empty, timeout has occured, send ETIMEDOUT error
                    if select.select([fd], [], [], int(request[1]))[0]: # wait till fd is ready or timeout
                        reply = 0
                    else:
                        reply = errno.ETIMEDOUT
                fd.close()
            
            except ValueError as e:
                reply = errno.EINVAL
            except (OSError, IOError) as e:
                reply = e.errno
            
        else:
            reply = "Command not found\n"

        self.wfile.write(bytes(str(reply)+"\n", "utf-8"))

if __name__ == "__main__":
    server_address = '/tmp/prusocket'
    
    #if socket exists, unlink/remove it
    if os.path.exists(server_address):
        os.unlink(server_address)
    # Create the server, binding to the socket file
    server =  socketserver.UnixStreamServer(server_address, MyRequestHandler)
    #Activate the server, serve until terminated
    server.serve_forever()

