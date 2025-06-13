#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <stdio.h>
#include "functions_for_logs.h"

using namespace std;
#define lung_righe 8
#define lung_colon 8
#define game_dimension 2
/*working progress */
void error_log(string errorname ) {         //FUNZIONE DI DEBUG ()

    string file_err_name = "scacchiera_log.txt";
    FILE* log_file;
    if (fopen_s(&log_file, file_err_name.c_str(), "a+") != 0) {
        cout << "errore nell'apertura/scrittura del file " << endl;
    }
    else {
        fputs(errorname.c_str(), log_file);
        fclose(log_file);
    }

}
void console_cleaner() {            /*working progress*/        //tentativo di renderlo crossplatform

#ifdef _WIN32
    system("pause && cls");         // Comando per prompt di Windows
#else           //TEST DA FARE 
    system("clear");                // Comando per sistemi operativi Unix-like (da provare);
    cout << "Premi qualsiasi tasto per continuare... " << endl;     //"pause" non esiste su linux che io sappia quindi questo 
    cin.get();                                                      //dovrebbe tentare di imitarlo 

#endif
}


/*

bool caracter_controll(string vector_action[]) {        //diamo un senso importante a sta funzione plss (ora non � utilizzata)
int dimensione = sizeof(vector_action) / sizeof(vector_action[0]);      //forse ti da errore codeblock in questo punto... commentalo
for (int action = 0; action < dimensione; action++) {
    for (int i = 0; i < game_dimension; i++) {
        if (isalpha(vector_action[action].at(i))) {
            //controllo porcata se po' fa meglioh 
            if (tolower(vector_action[action].at(i)) != 'a' && tolower(vector_action[action].at(i)) != 'b' && tolower(vector_action[action].at(i)) != 'c'
                && tolower(vector_action[action].at(i)) != 'd' && tolower(vector_action[action].at(i)) != 'e' && tolower(vector_action[action].at(i)) != 'f'
                && tolower(vector_action[action].at(i)) != 'g' && tolower(vector_action[action].at(i)) != 'h') {
                return false;
            }

        }
        else
        {
            if (vector_action[action].at(i) != '0' && vector_action[action].at(i) != '1' && vector_action[action].at(i) != '2'
                && vector_action[action].at(i) != '3' && vector_action[action].at(i) != '4' && vector_action[action].at(i) != '5'
                && vector_action[action].at(i) != '6' && vector_action[action].at(i) != '7') {
                return false;
            }
        }
    }//fine for interno

}//fine for esterno
return true;
}



*/