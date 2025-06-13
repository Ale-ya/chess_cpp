import socket

def send_data_to_server(host='192.168.2.170', port=65432, char1=2, char2=2, num1=3, num2=3):
    """
    Invia 2 caratteri e 2 numeri interi al server TCP.
    :param host: Indirizzo IP del server.
    :param port: Porta su cui inviare.
    :param char1: Primo carattere da inviare.
    :param char2: Secondo carattere da inviare.
    :param num1: Primo numero intero da inviare.
    :param num2: Secondo numero intero da inviare.
    """
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        try:
            # Connessione al server
            client_socket.connect((host, port))
           
            # Dati da inviare (formattati come stringa)
            data = f"{char1},{char2},{num1},{num2}"
            #data = str(char1)
            # Invia i dati codificati come bytes
            client_socket.sendall(data.encode('utf-8'))
           
            print("Dati inviati con successo al server.")
        except Exception as e:
            print(f"Errore durante l'invio dei dati al server: {e}")

# Esempio di utilizzo
send_data_to_server(host='192.168.2.170', port=65432 )