
import time
import serial
import serial.tools.list_ports
import sys

port = None



def send(speed, dir):
    port.write((str(speed) + ':' + str(dir) + '\n').encode())
    port.flush()

def main():
    global port
    port = None
    for p in serial.tools.list_ports.comports():
        print(p)

        print(p.description)
        if p.description == 'ttyACM0':
            print('Found arduino')
            port = serial.Serial(p.device, baudrate=115200)

    if port == None:
        print('Could not connect to Arduino')
        sys.exit(1)

    timeTotal = 0
    timeNums = 0
    real_start = time.time()
    for j in range (5):
        print(j)
        for i in range (-100, 100):
            print(i, timeNums, time.time() - real_start)
            start_time = time.time()
            send(i, 0)
            timeTotal += time.time() - start_time
            timeNums += 1
            time.sleep(.01)
        for i in range (-100, 100):
            print(i, timeNums, time.time() - real_start)
            start_time = time.time()
            send(0, i)
            timeTotal += time.time() - start_time
            timeNums += 1
            time.sleep(.01)

    print(timeTotal/timeNums)
if __name__ == '__main__':
    main()
