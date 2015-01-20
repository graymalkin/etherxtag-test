#! /usr/bin/python

# Echo client program
import random
import socket
import string
import sys
import time
import datetime as dt

def id_generator(k, size=6, chars=string.ascii_uppercase + string.digits):
    #return ('%c'%k) + (''.join(random.choice(chars) for _ in range(size-1)))
    return ''.join(random.choice(chars) for _ in range(size-1))


HOST = '192.168.0.37'
PORT = 80

def main(s):
    byte_count = 512
    loop_count = 0
    start = time.time()

    print "Connected."
    while 1:
        loop_count = loop_count+1
        # Create a random string
        data = id_generator(loop_count, byte_count)
        s.send(data)
        
        # Sleep to allow the target to catch up
        # time.sleep(0.04)
        # Read the response
        retVal = s.recv(byte_count)
        end = time.time()

        # If the response is empty
        if not retVal: 
            print "No return value."
            break
        # If the response doesn't match what was sent
        if retVal != data: 
            print "Data mismatch."
            print ', '.join(retVal)
            break
        #print ', '.join(data)
        #print ', '.join(retVal)
        rate = ((byte_count*loop_count) / ((end - start))) / 1000
        sys.stdout.write("\rData rate: %.2f kB/s [%d iterations] " % (rate,loop_count))
        sys.stdout.write("[%d bytes] [%d total seconds]" % (byte_count*loop_count, ((end - start))))
        sys.stdout.flush()

if __name__ == "__main__":
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    try:
        main(s)
    except:
        s.close()
        print "\nExit."
