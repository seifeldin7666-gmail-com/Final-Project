import socket
import concurrent.futures
import time


FORMAT = "utf-8" # decoding and encoding format
CONSOLE_ARDUINO_PORT = 12345 # console receving port
BOX_ARDUINO_PORT = 12346 # box recieving port

SERVER = socket.gethostbyname(socket.gethostname()) # the host server ip
CONSOLE_ADDR = (SERVER, CONSOLE_ARDUINO_PORT) # tuple for ip and port 
BOX_ADDR = (SERVER, BOX_ARDUINO_PORT)
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # making server object for udp comm
server.bind(CONSOLE_ADDR) # binding console
# server.bind(BOX_ADDR) # binding box



def reading(data, length):
    '''
        inputs:
            data: the data received in bytes
            length: length of array
        outputs:
            return array if length 3(Sensors) or 5(Forces)
    '''
    # if length == 3:
    #     msg = pickle.loads(data) 
    #     return msg
    # elif length == 5:
    #     msg = pickle.loads(data)
    #     return msg
    out = []
    if length == 9:
        accX = (-1 ** data[2])*(data[0] + data[1]/100)
        out.append(accX)
        accY = (-1 ** data[5])*(data[3] + data[4]/100)
        out.append(accY)
        pressure = data[6] * 255 + data[7]
        out.append(pressure)
        depth = data[8]
        out.append(depth)
    return out
    # for i in data:
    #     out.append(i)
    # return out


def send_to_gui(data, length):
    '''
        inputs:
            data: reciving data
            length: length of array
        outputs:
            print values on the gui
    '''
    data = reading(data, length) #get the decoded value


def start():
    '''
        Start receiving
    '''
    print("Waiting")
    with concurrent.futures.ThreadPoolExecutor() as excutor: # to handle more than one operation at same time
        while True:
            data, addr = server.recvfrom(4096)
            length = len(data)
            print(f"Length: {length}")
            data = excutor.submit(reading, data, length)
            data = data.result()
            print(f"Reading: {data}")
            time.sleep(1)
            #gui = excutor.submit(send_to_gui, data, length) #sending data to gui
            '''
            # just for printing values
            if length == 3:
                print(f"Sensors are: {data}")
            if length == 5:  
                print(f"Forces are: {data}")
            '''
# def test():
#     print("Waiting")
#     data, addr = server.recvfrom(4096)