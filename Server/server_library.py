import socket
import threading
import concurrent.futures
import pickle



# FORMAT = "utf-8" # decoding and encoding format
CONSOLE_ARDUINO_PORT = 12345 # console receving port
BOX_ARDUINO_PORT = 12346 # box recieving port

SERVER = socket.gethostbyname(socket.gethostname()) # the host server ip
CONSOLE_ADDR = (SERVER, CONSOLE_ARDUINO_PORT) # tuple for ip and port 
BOX_ADDR = (SERVER, BOX_ARDUINO_PORT)
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # making server object for udp comm
server.bind(CONSOLE_ADDR) # binding console
# server.bind(BOX_ADDR) # binding box



def reading(data, lenght):
    '''
        inputs:
            data: the data received in bytes
            lenght: lenght of array
        outputs:
            return array if lenght 3(Sensors) or 5(Forces)
    '''
    if lenght == 3:
        msg = pickle.loads(data) 
        return msg
    elif lenght == 5:
        msg = pickle.loads(data)
        return msg


def send_to_gui(data, lenght):
    '''
        inputs:
            data: reciving data
            length: length of array
        outputs:
            print values on the gui
    '''
    data = reading(data, lenght) #get the decode value


def start():
    '''
        Start receiving
    '''
    print("Waiting")
    with concurrent.futures.ThreadPoolExecutor() as excutor: # to handle more than one operation at same time
        while True:
            data, addr = server.recvfrom(4096)
            length = (len(pickle.loads(data))) # load data from bytes to original format
            data = excutor.submit(reading, data, length)
            data = data.result()
            #gui = excutor.submit(send_to_gui, data, length)
            '''
            # just for printing values
            if length == 3:
                print(f"Sensors are: {data}")
            if length == 5:  
                print(f"Forces are: {data}")
            '''
