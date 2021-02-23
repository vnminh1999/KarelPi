import socket
import sys
import karel_functions
from ctypes import *
import inspect

SERVER = "192.168.4.1"
PORT = 2300

class Response(Structure):
    _fields_ = [("result", c_uint32)]

def main():
    karel_funcs_list = inspect.getmembers(karel_functions, inspect.isfunction)
    karel_funcs = dict()
    for func in karel_funcs_list:
        karel_funcs[func[0]] = func[1]
    
    server_addr = (SERVER, PORT)
    ssock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket created")

    try:
        # bind the server socket and listen
        ssock.bind(server_addr)
        print("Bind done")
        ssock.listen(3)
        print("Server listening on port {:d}".format(PORT))

        while True:
            csock, client_address = ssock.accept()
            print("Accepted connection from {:s}".format(client_address[0]))

            buff = csock.recv(512)
            while buff:
                func_name = buff.decode()

                res = karel_funcs[func_name]()
                response = Response(res)
                csock.send(response)
                buff = csock.recv(512)

            print("Closing connection to client")
            csock.close()

    except AttributeError as ae:
        print("Error creating the socket: {}".format(ae))
    except socket.error as se:
        print("Exception on socket: {}".format(se))
    except KeyboardInterrupt:
        ssock.close()
    finally:
        print("Closing socket")
        ssock.close()

if __name__ == "__main__":
    main()
