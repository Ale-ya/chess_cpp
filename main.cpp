#include <iostream>
#include <string>
#include <string.h>
#include <cctype>
#include <algorithm>
#include "functions_for_logs.h"
#include "pieces.h"
using namespace std;

#define lung_righe 8
#define lung_colon 8
#define game_dimension 2

#define YELLOW_CONSOLE  "\033[33m"      //cambiamento dei colori cross platform 
#define DEFAULT_CONSOLE "\033[0m"
#define GREEN_CONSOLE   "\033[32m"
#define RED_CONSOLE     "\033[31m"
#define BLUE_CONSOLE    "\033[34m"



class Scacchiera {

private:
    pieces  *scacchiera[lung_colon][lung_righe];        //matrice per garantire una logica solida e facile da gestire
    bool checkmate;                                     //valore boolean per lo scacco matto (per il fine partita) (non ancora implementato correttamente)(work in progress)
    bool check;
    bool turno;                                         //per poi gestire i turni 
    //METHODS
    bool king_finder() {        //separare le due funzioni (questa + quella che chiama) in una (con l'ausilio delle referenze chiamare l'altra dentro la funzione principale)
                                //per fare una funzione che trovi gli SCACCHI MATTI
        int y = 0, x = 0;
        for (int righe = 0; righe < lung_righe; righe++) {
            for (int colonne = 0; colonne < lung_colon; colonne++) {
                if (scacchiera[righe][colonne] != nullptr) {
                    king* k;
                    //controllo se � minuscolo se � uguale a k
                    //cout << scacchiera[righe][colonne] -> simble()<< endl;      //test 
                    if (scacchiera[righe][colonne]->simble().at(1) == 'k' && turno) {
                        k = dynamic_cast<king*>(scacchiera[righe][colonne]);        //dynamic_cast<> per permettere interfacciamento al metodo is_checked 
                        y = righe;
                        x = colonne;
                        //cout << "hanno perso i minuscoli" << endl;
                        //system("pause");
                        return k->is_checked(scacchiera, y, x);
                        
                    }//isupper(scacchiera[righe][colonne]->simble().at(1)) && toupper(scacchiera[righe][colonne]->simble().at(1)) == 'k' && !turno
                    else if (scacchiera[righe][colonne]->simble().at(1) == 'K' && !turno) {     //il contrario
                        k = dynamic_cast<king*>(scacchiera[righe][colonne]);

                        y = righe;
                        x = colonne;
                        //cout << "hanno perso i maiuscoli" << endl;
                        //system("pause");

                        return k->is_checked(scacchiera,y,x);
                        
                    }
                }
            }
        }
        return false;
    }
    bool get_possible_check() {
        return false;//king_finder();
    }
    void set_basic_chessboard() {
        for (int righe = 0; righe < lung_colon; righe++) {
            for (int colonne = 0; colonne < lung_righe; colonne++) {
                scacchiera[colonne][righe] = nullptr;
            }
        }
    }
    void set_basic_disposition() {      //disposizione dei pezzi (senza cicli per praticit� nella visualizzazione)
        //bianco [righe][colonne]
        scacchiera[0][0] = new tower (" T ");   scacchiera[1][0] = new pawn(" P ");        //attezione all'astrazione
        scacchiera[0][1] = new knight(" C ");   scacchiera[1][1] = new pawn(" P ");
        scacchiera[0][2] = new bishop(" A ");   scacchiera[1][2] = new pawn(" P ");
        scacchiera[0][3] = new queen (" Q ");   //scacchiera[1][3] = new pawn(" P ");

        scacchiera[0][4] = new king  (" K ");   scacchiera[1][4] = new pawn(" P ");
        scacchiera[0][5] = new bishop(" A ");   //scacchiera[1][5] = new pawn(" P ");
        scacchiera[0][6] = new knight(" C ");   scacchiera[1][6] = new pawn(" P ");
        scacchiera[0][7] = new tower (" T ");   scacchiera[1][7] = new pawn(" P ");
        scacchiera[4][0] = new king(" k ");     scacchiera[3][1] = new pawn(" P ");
        //nero [righe][colonne]
        scacchiera[7][0] = new tower (" t ");   scacchiera[6][0] = new pawn(" p ");
        scacchiera[7][1] = new knight(" c ");   scacchiera[6][1] = new pawn(" p ");
        scacchiera[7][2] = new bishop(" a ");   scacchiera[6][2] = new pawn(" p ");
        scacchiera[7][3] = new queen (" q ");   scacchiera[6][3] = new pawn(" p ");     //test

        scacchiera[7][4] = new king  (" k ");   scacchiera[6][4] = new pawn(" p ");
        scacchiera[7][5] = new bishop(" a ");   scacchiera[6][5] = new pawn(" p ");
        scacchiera[7][6] = new knight(" c ");   scacchiera[6][6] = new pawn(" p ");
        scacchiera[7][7] = new tower (" t ");   scacchiera[6][7] = new pawn(" p ");
    }
    bool validator(string starting_pos,string final_pos) {          // si potrebbe fare con i vettori ma non credo convenga per la logica del codice
        int S_row = 0, S_collumn = 0, F_row = 0, F_collumn = 0;                   //righe e colonne
        string tmp = "";
        for (int i = 0; i < game_dimension; i++){       //ripetizione per i caratteri tramite #define e for(){}         per capire le coordinate di inizio spostamento

            if (isalpha(starting_pos.at(i))) {
                
                if (tolower(starting_pos.at(i)) == 'a')      {S_collumn = 0;}
                else if (tolower(starting_pos.at(i)) == 'b') {S_collumn = 1;}
                else if (tolower(starting_pos.at(i)) == 'c') {S_collumn = 2;}
                else if (tolower(starting_pos.at(i)) == 'd') {S_collumn = 3;}
                else if (tolower(starting_pos.at(i)) == 'e') {S_collumn = 4;}
                else if (tolower(starting_pos.at(i)) == 'f') {S_collumn = 5;}
                else if (tolower(starting_pos.at(i)) == 'g') {S_collumn = 6;}
                else if (tolower(starting_pos.at(i)) == 'h') {S_collumn = 7;}
                else{   /*gestione degli errori*/
                    return false;
                }
            }
            else {
                if (starting_pos.at(i) == '1')      {S_row = 0;}
                else if (starting_pos.at(i) == '2') {S_row = 1;}
                else if (starting_pos.at(i) == '3') {S_row = 2;}
                else if (starting_pos.at(i) == '4') {S_row = 3;}
                else if (starting_pos.at(i) == '5') {S_row = 4;}
                else if (starting_pos.at(i) == '6') {S_row = 5;}
                else if (starting_pos.at(i) == '7') {S_row = 6;}
                else if (starting_pos.at(i) == '8') {S_row = 7;}
                else {/*gestione degli errori*/
                    return false;
                }
            }
        } //fine for starting_pos

        if (scacchiera[S_row][S_collumn] == nullptr) {
            cout << "nessun pezzo in questa posizione specifica " << S_row << " " << S_collumn << endl;
            return false;
        }

        //cout << scacchiera[row][collumn] << endl;                 //TESTING 
        if (turno && isupper(scacchiera[S_row][S_collumn]->simble().at(1))) {    //non si pu� giocare con i pezzi dell'avversiario
            cout << "non si puo' giocare con i pezzi maiuscoli" << endl;
            return false;
        }
        if (!turno && islower(scacchiera[S_row][S_collumn]->simble().at(1))) {    //non si pu� giocare con i pezzi dell'avversiario
            cout << "non si puo' giocare con i pezzi minuscoli" << endl;
            return false;
        }


        for (int i = 0; i < game_dimension; i++) {          //ripetizione per i caratteri tramite #define e for(){}     per capire le coordinate di fine spostamento
            if (isalpha(final_pos.at(i))) {
                if (tolower(final_pos.at(i)) == 'a')      {F_collumn = 0;}
                else if (tolower(final_pos.at(i)) == 'b') {F_collumn = 1;}
                else if (tolower(final_pos.at(i)) == 'c') {F_collumn = 2;}
                else if (tolower(final_pos.at(i)) == 'd') {F_collumn = 3;}
                else if (tolower(final_pos.at(i)) == 'e') {F_collumn = 4;}
                else if (tolower(final_pos.at(i)) == 'f') {F_collumn = 5;}
                else if (tolower(final_pos.at(i)) == 'g') {F_collumn = 6;}
                else if (tolower(final_pos.at(i)) == 'h') {F_collumn = 7;}
                else {/*gestione degli errori*/
                    return false;
                }
            }
            else {
                if (final_pos.at(i) == '1')      {F_row = 0;}
                else if (final_pos.at(i) == '2') {F_row = 1;}
                else if (final_pos.at(i) == '3') {F_row = 2;}
                else if (final_pos.at(i) == '4') {F_row = 3;}
                else if (final_pos.at(i) == '5') {F_row = 4;}
                else if (final_pos.at(i) == '6') {F_row = 5;}
                else if (final_pos.at(i) == '7') {F_row = 6;}
                else if (final_pos.at(i) == '8') {F_row = 7;}
                else {/*gestione degli errori*/
                    return false;
                }
            }
        } //fine for final_pos

        if (S_row == F_row && S_collumn == F_collumn) {
            cout << "Non � possibile non muovere pezzi durante il proprio turno " << endl;
            return false;
        }
        //row = asse y / collumn = asse x 
        if (scacchiera[S_row][S_collumn] -> valid_movement_controller(S_row, S_collumn, F_row, F_collumn,turno, scacchiera)) {  //cambio localizzazione dell'oggetto
            
            if (scacchiera[F_row][F_collumn] != nullptr && tolower(scacchiera[F_row][F_collumn]->simble().at(1)) == 'k') {
                checkmate = true;
                //return true;
            }
            scacchiera[F_row][F_collumn] = scacchiera[S_row][S_collumn];        //classico swap dei valori
            scacchiera[S_row][S_collumn] = nullptr;
            return true;
        }
        else {
            return false;
        }
    }
    
public:
    //costruttore
    Scacchiera() {
        checkmate = false;          /*WORK IN PROGRESS*/
        check = false;
        turno = false;              //giocatore con le maiuscole inizia per primo (bianco/MAIUSCOLE = false)(BLU)
        set_basic_chessboard();
        set_basic_disposition();
    }
    //distruttore 
    ~Scacchiera() {
        for (int i = 0; i < lung_righe; ++i) {
            for (int j = 0; j < lung_colon; ++j) {
                delete scacchiera[i][j];    //libera la memoria completamente assegnandoli un indirizzo non valido
                scacchiera[i][j] = nullptr; //opzionale ma consigliato per evitare errori (gestione della memoria in particolare)   (messo per sicurezza aggiuntiva)
            }
        }
    }

    //  SET METHODS 
    void try_newMove() {
        string starting_position = "", final_position = "";
        cout << "Inserire la casella iniziale e finale del pezzo " << endl;
        cin >> starting_position >> final_position;
        while (starting_position.length() != 2) {
            cout << "si devono inserire le coordinate del pezzo che si desidera spostare " << endl;
            cin >> starting_position;
        }
        while (final_position.length() != 2) {
            cout << "si devono inserire le coordinate della casella finale del pezzo che si desidera spostare" << endl;
            cin >> final_position;
        }
        while (!validator(starting_position,final_position)) {
            cout << "Reinserire la casella iniziale e finale del pezzo, comando non valido" << endl;
            cin >> starting_position >> final_position;
        }
    }
    void set_turn() {
        if (turno) { turno = false; }  //gioca quello con le maiuscole
        else { turno = true; }   //gioca quello con le minuscole
    }
    //  GET METHODS 
    bool get_checkmate_value() {        /*WORK IN PROGRESS*/
        //int y = 0, x = 0;
        if (checkmate) {
            cout << endl << "Ha vinto il giocatore: " << (turno ? "BLU" : "ROSSO") << endl;
        }
        return checkmate;
    }
    //  UTILITY METHODS
    void print() {
        string index[lung_righe][lung_colon] = { 
            { " A "," B "," C "," D "," E "," F "," G "," H " },
            { " 1  "," 2  "," 3  "," 4  "," 5  "," 6  "," 7  "," 8  " }, 
        };
         
        cout << YELLOW_CONSOLE << "CHESS GAME         (by Alessio Paccino)\n\n"  << "    ";
        for (int idcolonne = 0; idcolonne < lung_colon; idcolonne++) { cout << index[0][idcolonne]; }
        cout << (turno ? RED_CONSOLE : BLUE_CONSOLE) << "          Turno del " << (turno ? "ROSSO" : "BLU") << "| Stato del re: " << 
                (get_possible_check() ? "Sotto scacco" : "Ok ") << YELLOW_CONSOLE<< endl;
        for (int righe = 0; righe < lung_righe; righe++) {      //TESTING
            cout << index[1][righe];
            for (int colonne = 0; colonne < lung_colon; colonne++) {
                if (scacchiera[righe][colonne] != nullptr) {
                    if (isupper(scacchiera[righe][colonne]->simble().at(1))){cout << BLUE_CONSOLE << scacchiera[righe][colonne]->simble() << YELLOW_CONSOLE;}
                    else { cout << RED_CONSOLE << scacchiera[righe][colonne]->simble() << YELLOW_CONSOLE; }      //[righe] [colonne]
                }
                else {cout << " ' ";}
            }
            cout << endl;
        }
    }
};



int main()
{
    Scacchiera game;
    console_cleaner();      //alcuni compilatori o ide ne hanno bisongo per far funzionare la stampa
    do {    // parte il bianco (blu) per primo (false)
        game.print();
        game.try_newMove();
        game.set_turn();
        console_cleaner();

    } while (!game.get_checkmate_value());
    cout << endl << "== FINE PROGRAMMA ==" << endl; //fine programma andr� aggiunto il risultato su file (json) (work in progress)
    return 0;
}
