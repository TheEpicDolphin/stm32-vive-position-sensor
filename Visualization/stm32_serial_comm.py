import serial
import struct

ser = serial.Serial(
    port='COM5',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=None)

print("connected to: " + ser.portstr)

#Wait for start message before reading print statements
while True:
    line = ser.readline()
    if(line[-6:].decode() == "start\n"):
        break;
    
while True:
    line = ser.read(4)

    #16 bit int
    #print(struct.unpack("h", line[:2])[0])

    #16 bit uint
    #print(struct.unpack('H', line[:2])[0])

    #32 bit int
    #print(struct.unpack('i', line[:4])[0])
    
    #32 bit uint
    #print(int.from_bytes(line[:4], "little"))

    #32 bit float
    print(struct.unpack('f', line[:4])[0])
    

ser.close()
