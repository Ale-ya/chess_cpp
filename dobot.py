# Lista delle posizioni

listaposizioni_A = [
    (296.39, -66.31), (276.64, -67.56), (257.72, -67.72), (237.76, -69.3),
    (217.95, -69.73), (197.09, -70.74), (177.15, -71.37), (155.98, -72.68)
]

c_outx = 188.40
c_outy = -227.98
xinput = 1
yinput = 1
xinputf = 2
yinputf = 2
take = False


def start_tcp_server(host='192.168.2.170', port=65432):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((host, port))
        server_socket.listen(1)
        print("Server in ascolto su:" + host + "...")

        while True:
            conn, addr = server_socket.accept()
            with conn:
                print("Connessione stabilita con " + str(addr))
                data = conn.recv(1024)
                if not data:
                    break

                # Decodifica i dati ricevuti
                try:
                    data_decoded = data.decode('utf-8')  # Usare UTF-8 per la decodifica
                    char1, char2, num1, num2 = data_decoded.split(',')

                    print("Ricevuto: " + str(char1) + str(char2) + str(num1) + str(num2))
                    break
                except Exception as e:
                    print("Errore nella decodifica dei dati:", e)
                    return 0, 0, 0, 0  # Restituisci valori di default in caso di errore

    return int(char1), int(char2), int(num1), int(num2)


def move():
    global xinput, xinputf, yinput, yinputf
    print(xinput)
    print(xinputf)
    print(yinput)
    print(yinputf)
    c = True

    x, y = listaposizioni_A[yinput]

    for j in range(0, xinput + 1, 1):
        if j == xinput:
            y -= 2
            current_pose = dType.GetPose(api)
            dType.SetPTPCmdEx(api, 2, x, y, 68, current_pose[3], 1)

            current_pose = dType.GetPose(api)
            dType.SetPTPCmdEx(api, 2, x, y, 27, current_pose[3], 1)

            catch(c, x, y)
            c = not c
            break
        elif j > 4:
            x -= 1
            y += 22.95
        else:
            x -= 0.4
            y += 22.95

    x, y = listaposizioni_A[yinputf]

    for j in range(0, xinputf + 1, 1):
        if j == xinputf:
            y -= 2
            x += 0.5
            current_pose = dType.GetPose(api)
            dType.SetPTPCmdEx(api, 2, x, y, 68.2932, current_pose[3], 1)

            current_pose = dType.GetPose(api)
            dType.SetPTPCmdEx(api, 2, x, y, 27, current_pose[3], 1)

            catch(c, x, y)
            c = not c
            break
        elif j > 4:
            x -= 1
            y += 22.95
        else:
            x -= 0.4
            y += 22.95


def catch(boolvalue, x, y):
    forza_pinza(6, boolvalue)
    print("bool e " + str(boolvalue))
    current_pose = dType.GetPose(api)
    dType.SetPTPCmdEx(api, 2, x, y, 68.2932, current_pose[3], 1)


def move_out(x, y):
    global c_outx, c_outy

    dType.dSleep(1000)
    dType.SetEndEffectorGripperEx(api, 1, 1)
    dType.dSleep(600)
    dType.SetEndEffectorGripperEx(api, 0, 1)
    dType.dSleep(1000)

    current_pose = dType.GetPose(api)
    dType.SetPTPCmdEx(api, 2, x, y, 68, current_pose[3], 1)

    current_pose = dType.GetPose(api)
    dType.SetPTPCmdEx(api, 2, c_outx, c_outy, 68.2932, current_pose[3], 1)
    dType.SetEndEffectorGripperEx(api, 1, 0)


def take_over():
    global yinputf, xinputf, listaposizioni_A
    x, y = listaposizioni_A[yinputf]

    for j in range(0, xinputf + 1, 1):
        if j == xinputf:
            y -= 2
            x += 0.5
            current_pose = dType.GetPose(api)
            dType.SetPTPCmdEx(api, 2, x, y, 68.2932, current_pose[3], 1)

            current_pose = dType.GetPose(api)
            dType.SetPTPCmdEx(api, 2, x, y, 27, current_pose[3], 1)

            move_out(x, y)
            break
        elif j > 4:
            x -= 1
            y += 22.95
        else:
            x -= 0.4
            y += 22.95


def forza_pinza(forza, boolvalue):
    for f in range(0, forza + 1, 1):
        dType.SetEndEffectorGripperEx(api, int(boolvalue), 1)
        dType.dSleep(125)
        dType.SetEndEffectorGripperEx(api, 0, 1)
        boolvalue = not boolvalue
        dType.SetEndEffectorGripperEx(api, int(boolvalue), 0)


# Esegui la connessione TCP e ottieni i valori
xinput, yinput, xinputf, yinputf = start_tcp_server(host='192.168.2.170', port=65432)

# Muovi il robot in base ai valori ricevuti
if take:
    take_over()

move()

# Rilascia il gripper alla fine
dType.SetEndEffectorGripperEx(api, 0, 0)