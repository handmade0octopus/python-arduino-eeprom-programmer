import serial
import time
import os

# Opens com port and bin file to be read
ser = serial.Serial('COM6', baudrate = 9600, timeout = None)
F = open("bin1.bin", "rb")
# Saves fileSize as int that fits into a byte
fileSize = int(os.path.getsize("bin1.bin")/256)

# Wait for Arduino
time.sleep(2)

def main():
        write()
        read()

        F.close()
        ser.close()
        
def read():
        # Reads file and saves, showing what has came back
        D = open("bin2.bin", "wb")
        val = ser.read(fileSize*256)
        D.write(val)
        print(bytes(val).hex(), end=' ')
        D.close()
        

def write():
        # Sends file size and prints to confirm
        ser.write((fileSize).to_bytes(1, 'big'))
        print(256*int.from_bytes(ser.read(), 'big'))
        time.sleep(1)
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        # Sends file in 1024 byte chunks
        toSend = F.read(1024)
        while toSend:
                ser.write(bytearray(toSend))
                print(bytes(toSend).hex())
                print(int(ser.readline()))
                toSend = F.read(1024)
                
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        


if __name__ == "__main__":
        main()
