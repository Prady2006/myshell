'''


this is a server


'''

import socket
import threading
import queue

q = queue.Queue()

def recv(conn,s):

    while True:
        data = conn.recv(1024)
        print(data)
        if data == 'quit':
            print(" Connection terminated by server ..")
            break
        else :
            data1 = q.get()
            print(data1)
            try:
                if len(data1) > 0  and data1.split()[0] == "dwl" and len(data1.split()) == 3:
                    fptr = open(data1.split()[2],"a+")
                    fptr.write(data.decode())
                    fptr.close()
                else:    
                    print(" "*30 + data.decode())
            except:
                continue
    conn.close()
    s.close()

def send(conn,s):
    while True:
        data = input()
        if data != "quit":
            q.put(data)
            conn.send(data.encode())
    conn.close()
    s.close()
if __name__ == "__main__":

    #creating socket for connection
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    s.bind(("",1234));
    s.listen(4)
    conn ,addr = s.accept()
    print("got conn from ", addr[0],addr[1])
    t1 = threading.Thread(target= send,args=(conn,s))  
    t2 = threading.Thread(target=recv,args=(conn,s))

    t1.start()
    t2.start()

    t1.join()
    t2.join()



