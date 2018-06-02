#!/usr/bin/python3

import socketserver
import subprocess
import sys
import errno

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

        if request[0] == "probe":
            try:
                p = subprocess.Popen(["/sbin/modprobe", request[1]])
                reply = 0
            except OSError as e:
                reply = e.errno

        elif request[0] == "unprobe":
            try:
                p = subprocess.Popen(["/sbin/modprobe", "-r", request[1]])
                reply = 0
            except OSError as e:
                reply = e.errno

        elif request[0] == "enable":
            try:
                with open('/sys/class/remoteproc/remoteproc'+request[1]+'/state', 'w') as fd:               
                    fd.write('start')
                fd.close()
                reply = 0
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "disable":
            try:
                with open('/sys/class/remoteproc/remoteproc'+request[1]+'/state', 'w') as fd:               
                    fd.write('stop')
                fd.close()
                reply = 0
            except OSError as e:
                reply = e.errno

        elif request[0] == "load":
            try:
                p = subprocess.Popen(["cp", request[1], request[2]])
                reply = 0
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "sendmsg":
            try:
                with open('/dev/'+request[1]) as fd:
                    fd.write(request[2]+"\n")
                fd.close()
                reply = 0
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "getmsg":
            try:
                reply = ""
                with open('/dev/'+ request[1]) as fd:
                    #timeout 0 in select <=> non blocking read
                    while fd in select.select([fd], [], [], 0)[0]:
                        reply += fd.readline()+"\n"
                fd.close()
            except (OSError, IOError) as e:
                reply = e.errno

        elif request[0] == "eventwait":
            try:
                with open('/dev/'+ request[1]) as fd:
                    #if first return arg is empty, timeout has occured, send ETIMEDOUT error
                    if select.select([fd], [], [], int(request[1]))[0]: # wait till fd is ready or timeout
                        reply = 0
                    else:
                        reply = errno.ETIMEDOUT
                fd.close()
            except (OSError, IOError) as e:
                reply = e.errno
            
        else:
            reply = "Command not found\n"

        self.wfile.write(bytes(str(reply)+"\n", "utf-8"))

if __name__ == "__main__":
    server_address = '/tmp/prusocket'

    # Create the server, binding to the socket file
    server =  socketserver.UnixStreamServer(server_address, MyRequestHandler)
    #Activate the server, serve until terminated
    server.serve_forever()
