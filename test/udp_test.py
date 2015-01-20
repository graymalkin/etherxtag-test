import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 1338
MESSAGE = "Hello, World!"

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
while True:
    print "message:", MESSAGE
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
