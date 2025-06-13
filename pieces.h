#include <iostream>
#include <stdio.h>
#include <string>
#include "functions_for_logs.h"
#include <limits>
#define lung_righe 8
#define lung_colon 8
#define game_dimension 2


using namespace std;
class pieces {      /*work in progress*/
protected:

    string piece_simble;
    int controls(pieces* scacchiera[lung_righe][lung_colon], int finaly, int finalx, bool turno) {       // teamkill (0 false)  casella non occupata (1) casella occopata dal nemico (2)

        //VA MESSA IN TUTTI (GESTIONE TEAM KILL)
        bool occupied_box = false;  //vero se la casella di arrivo è occupata
        if (scacchiera[finaly][finalx] != nullptr) {       //se la casella è piena
            string team_kill_controller = scacchiera[finaly][finalx]->simble();
            if (!turno) {
                for (char caracterfinder : {'p', 'c', 'a', 't', 'k', 'q'}) {
                    if (team_kill_controller.find(caracterfinder) != string::npos) {
                        //se la casella è piena e trova una lettera minuscola (quindi avversaria) e il turno è falso (quindi appartenente alla squadra maiuscola)
                        occupied_box = true;
                        break;
                    }
                }
            }
            else {
                for (char caracterfinder : {'P', 'C', 'A', 'T', 'K', 'Q'}) {
                    if (team_kill_controller.find(caracterfinder) != string::npos) {
                        //se la casella è piena e trova una lettera maiuscola (quindi avversaria) e il turno è vero (quindi appartenente alla squadra minuscola)
                        occupied_box = true;
                        break;
                    }
                }
            }

            if (!occupied_box) {
                return 0;       //teamkill
            }
        }
        else { occupied_box = false; }      //FINE GESTIONE TEAM KILL
        // teamkill (0 == false)  casella non occupata (1) casella occopata dal nemico (2)
        if (occupied_box) {
            return 2;
        }
        else {
            return 1;
        }
    }
public:
    pieces(string simbled) {
        piece_simble = simbled;
    }
    virtual bool valid_movement_controller(int S_x, int S_y, int F_x, int F_y, bool turno, pieces* box[lung_righe][lung_colon]) {
        //cout << "pieces virtual entry " << endl;      // TESTING 
        return false;
    }
    virtual string simble() {   //virtual impuro (non necessita per forza di override / fatto per sicurezza in alcune classi )
        return piece_simble;
    };
};

class tower : public pieces {

private:


public:
    tower(string simbled) : pieces(simbled) {
        piece_simble = simbled;
    }

    string simble() {
        return piece_simble;
    }
    bool valid_movement_controller(int startery, int starterx, int finaly, int finalx, bool turno, pieces* scacchiera[lung_righe][lung_colon]) {

        if (finaly != startery && finalx != starterx) {     //non si può spostare orizzontalmente e verticalmente in un turno
            cout << "non e' possible non muovere nessun pezzo nella scacchiera" << endl;
            error_log("Non si puo' fare una mossa e digitare la stessa casella di arrivo e di fine ");     //DEBUG
            return false;
        }

        bool occupied_box = false;
        int c = controls(scacchiera, finaly, finalx, turno);
        if (c == 0) {
            return false;
        }
        else if (c == 2) {
            occupied_box = true;
        }

        if (starterx == finalx) {       //per vedere se ci sono pezzi in mezzo al percorso
            int cycle_mod = (finaly > startery) ? 1 : -1;     //operatore ternario... è un if, il primo valore viene dato se vero il secondo se falso
            for (int i = startery + cycle_mod; i != finaly; i += cycle_mod) {
                if (scacchiera[i][starterx] != nullptr) {
                    return false;
                }
                //cout << "pluto " << endl;         //TEST
            }
        }
        else if (startery == finaly) {      //per vedere se ci sono pezzi in mezzo al percorso
            int cycle_mod = (finalx > starterx) ? 1 : -1;     //operatore ternario... è un if, il primo valore viene dato se vero il secondo se falso
            for (int i = starterx + cycle_mod; i != finalx; i += cycle_mod) {
                if (scacchiera[startery][i] != nullptr) {
                    return false;
                }
            }
        }
        return true;
    }
};

class queen : public pieces {
public:
    queen(string simbled) : pieces(simbled) {
        piece_simble = simbled;
    }
    bool valid_movement_controller(int startery, int starterx, int finaly, int finalx, bool turno, pieces* scacchiera[lung_righe][lung_colon]) {
        int c = controls(scacchiera, finaly, finalx, turno);        // team kill
        if (c == 0) {       //casella occupata dagli alleati (mossa non valida)
            return false;
        }

        if (abs(finalx - starterx) != abs(finaly - startery) && finaly != startery && finalx != starterx) {       //gestione di ciò che può e non può fare la regina
            cout << "movimento non valido per la regina" << endl;
            return false;
        }
        else if ((finalx - starterx) == (finaly - startery)) {
            //identifico se va giu o su
            int Index_oriz = ((finalx - startery) > 0 ? 1 : -1);
            int Index_vert = ((finaly - startery) > 0 ? 1 : -1);
            //uso di un ciclo while (for migliore?)
            int x = starterx + Index_oriz;
            int y = startery + Index_vert;
            while (x != finalx && y != finaly) {
                if (scacchiera[y][x] != nullptr) {
                    return false;
                }
                x += Index_oriz;
                y += Index_vert;
            }
        }
        else if (starterx == finalx) {       //per vedere se ci sono pezzi in mezzo al percorso
            int cycle_mod = (finaly > startery) ? 1 : -1;     //operatore ternario... è un if, il primo valore viene dato se vero il secondo se falso
            for (int i = startery + cycle_mod; i != finaly; i += cycle_mod) {
                if (scacchiera[i][starterx] != nullptr) {
                    return false;
                }
                //cout << "pluto " << endl;         //TEST
            }
        }
        else if (startery == finaly) {      //per vedere se ci sono pezzi in mezzo al percorso
            int cycle_mod = (finalx > starterx) ? 1 : -1;     //operatore ternario... è un if, il primo valore viene dato se vero il secondo se falso
            for (int i = starterx + cycle_mod; i != finalx; i += cycle_mod) {
                if (scacchiera[startery][i] != nullptr) {
                    return false;
                }
            }
        }

        return true;

    }
};

class bishop : public pieces {      //classe alfiere
public:
    bishop(string simbled) : pieces(simbled) {
        piece_simble = simbled;
    }
    bool valid_movement_controller(int startery, int starterx, int finaly, int finalx, bool turno, pieces* scacchiera[lung_righe][lung_colon]) {
        if (abs(finalx - starterx) != abs(finaly - startery)) {
            cout << "Non si può spostare l'alfiere se non in diagonale " << endl;
            return false;
        }

        bool occupied_box = false;
        int c = controls(scacchiera, finaly, finalx, turno);        // team kill
        if (c == 0) {       //casella occupata dagli alleati (mossa non valida)
            return false;
        }
        else if (c == 2) {  //casella occupata dai nemici
            occupied_box = true;
        }

        //identifico se va giu o su
        int Index_oriz = ((finalx - startery) > 0 ? 1 : -1);
        int Index_vert = ((finaly - startery) > 0 ? 1 : -1);
        //uso di un ciclo while (for migliore?)
        int x = starterx + Index_oriz;
        int y = startery + Index_vert;
        while (x != finalx && y != finaly) {
            if (scacchiera[y][x] != nullptr) {
                return false;
            }
            x += Index_oriz;
            y += Index_vert;
        }
        return true;


    }

};

class knight : public pieces {
public:
    knight(string simbled) : pieces(simbled) {
        piece_simble = simbled;
    }
    bool valid_movement_controller(int startery, int starterx, int finaly, int finalx, bool turno, pieces* scacchiera[lung_righe][lung_colon]) {
        int oriz = finalx - starterx;
        int vertical = finaly - startery;
        if (abs(oriz) == 2) {
            if (abs(vertical) != 1) {
                return false;

            }
        }
        else if (abs(oriz) == 1) {
            if (abs(vertical) != 2) {
                return false;
            }
        }
        else { return false; }
        int c = controls(scacchiera, finaly, finalx, turno);        // team kill
        if (c == 0) {       //casella occupata dagli alleati (mossa non valida)
            return false;
        }
        return true;
    }

};

class pawn : public pieces {

private:
    bool promoted;              //per la promozione (work in progress)
    bool first_move;            //per fare il balzo di due caselle
    void promotion(pieces* scacchiera[lung_righe][lung_colon], /*int final_y, int final_x,*/ int starty, int startx) {          //work in progress
        int prom = 0; 
        while (true) {
            cout << "\nIl tuo pedone ha raggiunto l'ultima traversa, in cosa lo trasformi: \n[1] Torre\n[2] Regina\n[3] Cavallo\n[4] Alfiere\nComando inserito: ";
            cin >> prom;

            if (cin.fail() || prom < 1 || prom > 4) {
                cout << "Scelta non valida! Per promuovere il tuo pedone scrivi un numero tra 1 e 4.\n";
                cin.clear(); // Resetta il flag di errore
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora i caratteri non validi
            }
            else {
                cout << "Promozione riuscita!" << endl;
                break;      //esce dal while potenzialmente infinito
            }
        }

        try             //DEBUG     (working progress) (fare un ritorno nella funzione?)      
        {
            string c = scacchiera[starty][startx]->simble();//prendo il valore del simbolo prima di distruggere l'oggetto (si può fare anche in modi più semplici)
            delete scacchiera[starty][startx];
            
            if (prom == 1) {
                scacchiera[starty][startx] = new tower(islower(c.at(1)) ? " t " : " T ");
            }
            else if (prom == 2) {
                scacchiera[starty][startx] = new queen(islower(c.at(1)) ? " q " : " Q ");
            }
            else if (prom == 3) {
                scacchiera[starty][startx] = new knight(islower(c.at(1)) ? " c " : " C ");
            }
            else if (prom == 4) {
                scacchiera[starty][startx] = new bishop(islower(c.at(1)) ? " a " : " A ");
            }
        }
        catch (const std::exception& e)
        {
            //what() funzione per leggere gli errori
            cout << "errore: " << e.what() << endl;
            
        }
    }

public:

    pawn(string simbled) : pieces(simbled) {
        piece_simble = simbled;
        promoted = false;
        first_move = true;
    }
    bool valid_movement_controller(int startery, int starterx, int finaly, int finalx, bool turno, pieces* scacchiera[lung_righe][lung_colon]) {
        int err = 0, jump = 1;
        bool eating = false;
        bool occupied_box = false;
        int c = controls(scacchiera, finaly, finalx, turno);        // team kill
        if (c == 0) {
            return false;
        }
        else if (c == 2) {
            occupied_box = true;
        }

        if (first_move) {    //per fargli fare il balzo iniziale di due caselle
            jump++;
        }

        //nel caso del movimento base del pedone sono le x quelle che aumentano o diminuiscono
        if (finaly > (startery + jump) || finaly <= startery && !turno) {
            err++;
        }
        else if (finaly < (startery - jump) || finaly >= startery && turno) {
            err++;
        }
        if (finalx == (starterx + 1) || finalx == (starterx - 1)) {
            if (!occupied_box) {            //capisce se sta mangiando qualsiasi pezzo
                err++;
            }
            else {
                eating = true;          // se è vero sta mangiando
            }
        }
        else if (finalx != starterx) {
            err++;
        }
        if (err != 0) {
            cout << "Errore nello spostamento " << endl;        //TESTING
            return false;
        }
        else {
            if (!occupied_box || eating) {
                cout << "Spostamento corretto" << endl;         //TESTING
                first_move = false;
                if (finaly == 7 || finaly == 0) {
                    promotion(scacchiera, startery, starterx);      //work in progress
                }
                return true;
            }
            else {
                cout << "Spostamento errato, c'e' un pezzo che ostruisce lo spostamento " << endl;          //TESTING
                return false;
            }

        }
    }
    string simble() override {      //specifico l'override (non necessario)
        return piece_simble;
    }

};

class king : public pieces {

private:
    bool first_move;
public:
    king(string simbled) : pieces(simbled) {
        piece_simble = simbled;
        first_move = true;
    }

    bool is_checked(pieces * scacchiera[lung_righe][lung_colon], int y , int x) {
        int verticale = 0;
        int horizontal = 0;
        for (int i = 0; i < 2; i++ ) {      //controllo scacchi verticali al RE (classe king)
            (i == 0 ? verticale = (y - 1) : verticale = (y + 1) );
            for (int v = verticale; (i == 0 ? v >= 0 : v < lung_colon); (i == 0 ? v-- : v++) ) {        //v < 0
                //cout << "numero(y): " <<v << " lettera(numero)(asse x): " << x << endl; //test
                if (scacchiera[v][x] != nullptr) {
                    if (islower(scacchiera[v][x]->simble().at(1)) && islower(scacchiera[y][x]->simble().at(1))) {   //gestione degli alleati nella traiettoria
                        break;
                    }
                    else if (isupper(scacchiera[v][x]->simble().at(1)) && isupper(scacchiera[y][x]->simble().at(1))) {  //gestione degli alleati nella traiettoria
                        break;
                    }
                    else if (tolower(scacchiera[v][x]->simble().at(1)) == 'q' || tolower(scacchiera[v][x]->simble().at(1)) == 't') {
                        return true;
                    }
                }
            }
        }
        for (int i = 0; i < 2; i++) {      //controllo scacchi verticali al RE (classe king)
            (i == 0 ? horizontal = (x - 1) : horizontal = (x + 1));
            for (int oriz = horizontal; (i == 0 ? oriz >= 0 : oriz < lung_righe); (i == 0 ? oriz-- : oriz++)) {        //v < 0
                //cout << "numero(y): " << y << " lettera(numero)(asse x): " << oriz << endl; //test
                if (scacchiera[y][oriz] != nullptr) {
                    if (islower(scacchiera[y][oriz]->simble().at(1)) && islower(scacchiera[y][x]->simble().at(1))) {   //gestione degli alleati nella traiettoria
                        break;
                    }
                    else if (isupper(scacchiera[y][oriz]->simble().at(1)) && isupper(scacchiera[y][x]->simble().at(1))) {  //gestione degli alleati nella traiettoria
                        break;
                    }
                    else if (tolower(scacchiera[y][oriz]->simble().at(1)) == 'q' || tolower(scacchiera[y][oriz]->simble().at(1)) == 't') {
                        return true;
                    }
                }
            }
        }

        for (int i = 0; i < 4; i++) {       // quattro direzioni da controllare (neg,neg//pos,neg//pos,pos//neg,pos)
            
            int xdir = 0, ydir = 0;
            if (i == 0) {
                xdir = -1; ydir = -1;       //da gestire se è nell'ultima traversa
            }
            else if (i == 1) {
                xdir = 1; ydir = - 1;
            }
            else if (i == 2) {
                xdir = 1; ydir = 1;
            }
            else {
                xdir = -1; ydir = 1;
            }
            int ychecked = y + ydir, xchecked = x + xdir;
            int cicle = 0;//contatore per far si che prenda i pedoni che sono vicini al re di una casella in diagonale
            while (ychecked <= 8 && ychecked >= 0 && xchecked <= 8 && xchecked >= 0) {  //senza gli " = " non prende le ultime traverse e le ultime colonne 
                if (scacchiera[ychecked][xchecked] != nullptr) {
                    if (islower(scacchiera[ychecked][xchecked]->simble().at(1)) && islower(scacchiera[y][x]->simble().at(1))) {   //gestione degli alleati nella traiettoria
                        break;
                    }
                    else if (isupper(scacchiera[ychecked][xchecked]->simble().at(1)) && isupper(scacchiera[y][x]->simble().at(1))) {  //gestione degli alleati nella traiettoria
                        break;
                    }
                    else if (tolower(scacchiera[ychecked][xchecked]->simble().at(1)) == 'q' || tolower(scacchiera[ychecked][xchecked]->simble().at(1)) == 'a') {
                        return true;
                    }
                    else if (tolower(scacchiera[ychecked][xchecked]->simble().at(1)) == 'p') {
                        if (cicle == 0) {
                            return true;
                        }
                        else {
                            break;
                        }
                    }
                }
                ychecked += ydir;
                xchecked += xdir;
                cicle++;
            }

        }


        return false;

    }
    bool valid_movement_controller(int startery, int starterx, int finaly, int finalx, bool turno, pieces* scacchiera[lung_righe][lung_colon]) {
    
        //controllo per l'arrocco 

        int c = controls(scacchiera, finaly, finalx, turno);        // team kill
        if (c == 0) {       //casella occupata dagli alleati (mossa non valida)
            return false;
        }//possibile controllo di protezione casella chiamando is_checked e dando le coordinate di dove si vuole andare
        int absx = abs(finalx - starterx);
        int absy = abs(finaly - startery);
        if (absx != 1 && absx != 0 || absy != 1 && absy != 0) { cout << "non si puo spostare il re di piu' di una casella" << endl; return false; }
        else { /*cout << "corretto " << endl; */return true; }
    
    }
    string simble() override {
        return piece_simble;
    }

};


