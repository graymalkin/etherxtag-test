import socket
from array import array

UDP_IP = "192.168.0.58"
UDP_PORT = 1338
MESSAGE = array('c', ['\0']*128)


print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
while(True) :
	sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
