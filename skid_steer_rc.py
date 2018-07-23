import curses
import atexit
import time
import serial
import serial.tools.list_ports
import sys

'''
Takes curses input and drives the cars.
W increases the forward speed, and S reduces it
A makes the car turn left, D makes it turn right
E resets the turning and forward speed to 0
Q quits the program
'''
W = ord('w')
A = ord('a')
S = ord('s')
D = ord('d')
Q = ord('q')
E = ord('e')
R = ord('r')

port = None
scr = None


def send(speed, dir):
    out = 'Speed:' + str(speed) + ' Dir:' + str(dir)
    scr.addstr(out)
    scr.addstr('\t')
    port.write((str(speed) + ':' + str(dir) + '\n').encode())
    port.flush()

def main():
    global port, scr
    port = None
    for p in serial.tools.list_ports.comports():
        print(p.description)
        if p.description == 'ttyACM0':
            print('Found arduino')
            port = serial.Serial(p.device)

    if port == None:
        print('Could not connect to Arduino')
        sys.exit(1)

    scr = curses.initscr()
    curses.cbreak()
    curses.setsyx(-1, -1)
    scr.keypad(1)
    scr.addstr(0, 10, 'Skid-steer robot controller')
    scr.addstr(2, 10, 'Press Q to exit.')
    scr.refresh()
    dir = 0
    speed = 0

    while True:
        key = scr.getch()

        if key == Q:
            speed = 0
            dir = 0
            send(speed, dir)
            break
        elif key == W:
            if speed < 255:
                speed += 15
                send(speed, dir)
        elif key == S:
            if speed > -255:
                speed -= 15
                send(speed, dir)
        elif key == A:
            if dir > -255:
                dir -= 15
                send(speed, dir)
        elif key == D:
            if dir < 255:
                dir += 15
                send(speed, dir)
        elif key == E:
            speed = 0
            dir = 0
            send(speed, dir)
        elif key == R:
            port.write('release')
            port.flush()

        else:
            print("not a valid key")


    curses.endwin()

if __name__ == '__main__':
    main()
