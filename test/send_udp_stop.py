import socket
from array import array

UDP_IP = "192.168.0.59"
UDP_PORT = 1338
MESSAGE = array('c', ['\0']*128)
# MESSAGE[0] = '\1'

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
