import atexit
import time
import serial
import serial.tools.list_ports
import sys
import struct
import threading
import socket

port = None
global command_lock
global stop_eventt
global client_socket_commands
dir = 0
speed = 0
global port
button_names={ 0:'A',
        1:'B',
        2:'X',
        3:'Y',
        4:'LB',
        5:'RB',
        6:'screen',
        7:'menu',
        8:'xbox' }

analog_names={0:'js1-x',
        1:'js1-y',
        2:'LT',
        3:'js2-x',
        4:'js2-y',
        5:'RT',
        6:'DPad-x',
        7:'DPad-y'}


def send(speed, dir):
    global port
    out = 'Speed:' + str(speed) + ' Dir:' + str(dir)
    print(out)
    try:
        port.write((str(speed) + ':' + str(dir) + '\n').encode())
    except (serial.serialutil.SerialException):
        print("error, reconnecting")
        for p in serial.tools.list_ports.comports():
            print(p.description)
            if 'ttyACM' in p.description:
                print('Found arduino')
                port = serial.Serial(p.device, baudrate=576000)


def map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

def send_stuff(in_type, in_id, value):
    global speed
    global dir
    if in_type == 2:
        if in_id == 2:
            dir = map(value, -32767, 32767, -255, 255)
            send(speed, dir)
            send(speed, dir)
        elif in_id == 3:
            speed = map(value, 32767, -32767, -255, 255)
            send(speed, dir)
            send(speed, dir)
    elif in_type == 1:
        if in_id == 0:
            send(0,0)
            send(0,0)

def keep_sending():

    while True:
        global sending_lock
        sending_lock.acquire()
        global speed, dir
        send(speed, dir)
        # print('threadedd', speed, dir)
        sending_lock.release()
        time.sleep(0.05)


port = None
for p in serial.tools.list_ports.comports():
    print(p.description)
    if 'ttyACM' in p.description:
        print('Found arduino')
        port = serial.Serial(p.device, baudrate=576000)

if port == None:
    print('Could not connect to Arduino')
    sys.exit(1)

joystick_file='/dev/input/js0'
js_out=open(joystick_file, 'rb')
print(js_out)
#
sending_lock = threading.Lock()
out_stream = threading.Thread(target=keep_sending)
out_stream.setDaemon(True)
out_stream.start()
print('howdy')
try:
    while not False:
        evbuf=js_out.read(8)
        if evbuf:
            t, value, in_type, in_id=struct.unpack('IhBB', evbuf)
            print(in_type, in_id)
            send_stuff(in_type, in_id, value)

except BrokenPipeError:
    print("command connection broken, server no longer recieving")
    print(datetime.datetime.now().strftime(time_format))
    stop_ev.set()
    #
    # dir = 0
    # speed = 0
    #
    #
