import socket

HOST = '127.0.0.1'  # Indirizzo loopback
PORT = 6000        # Porta su cui ascoltare




def sbustamento(messg):
    
    lista_messaggi = messg.split(";")
    print(lista_messaggi)


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Server Python in ascolto su {HOST}:{PORT}")
    while True:
        conn, addr = s.accept()
        with conn:
            print(f"Connesso da {addr}")
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                decoded_data = data.decode('utf-8')
                print(f"Ricevuto dal client C++: {decoded_data}")
                # Invia una risposta
                conn.sendall(b"Hello from Python server")
                sbustamento(decoded_data)

print("Server chiuso")