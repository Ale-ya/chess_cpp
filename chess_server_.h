
#ifndef CHESS_SERVER_h
#define CHESS_SERVER_h
#include <iostream>
#include <string>
#include "macro.h"
#include <chrono>
#include <mutex>


using namespace std;

class Myclient
{
private:

    int port = 0;
    string ip = "";
    list<string> comandi;
    queue<string>  indexqueque;
    mutex queue_mutex;
    bool debug;
    /*PRIVATE METHODS*/
    bool checkIP(string iptocheck){
        if (iptocheck.length() < 7){
            printf ("[ERRORE IP] = ip troppo piccolo per essere reale [%s] ", iptocheck);
            return false;
        }
        for (int i = 0; i < iptocheck.length(); i++)
        {
            for (int n_before_point = 0; n_before_point < iptocheck.length(); n_before_point++)
            {
                if (iptocheck[i]!='.'){
                    i++;
                }else{
                    if(n_before_point > 3 || n_before_point <= 0){
                        printf("[ERRORE IP SERVER] IP SCRITTO MALE = %s", iptocheck);
                        return false;
                    }else{
                        break;
                    }
                }
            }
            
        }
        return true;
        
    } 
    void start(list<string> position){
        const char* message_to_send;
        string tmp="";
        for (auto &p : position){ tmp+=p + ";"; } 
        tmp.pop_back();
        message_to_send = tmp.c_str();
        if (debug) printf("[MESSAGE] -==> %c", message_to_send);
        int sock = 0;
        sockaddr_in server{};
        try{
            sock = socket(AF_INET, SOCK_STREAM, 0);
            server.sin_family = AF_INET;
            server.sin_port = htons(port);
            inet_pton(AF_INET, ip.c_str(), &server.sin_addr);
            if (debug)  printf("[CLIENT CONFIGURATION READY]");
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
        try{
            connect(sock,(sockaddr*)&server, sizeof(server));
            send(sock, message_to_send, strlen(message_to_send), 0);
        }catch(const exception& e){
            
        }        
        
        if (debug){
            close(sock);
            printf("[SOCKET CLOSE = SUCCESS]");
        }
        this_thread::sleep_for(chrono::milliseconds(200));
    }

public:

    Myclient(int SetPort, string setIp, bool debug){
        port = SetPort;
        ip = setIp;
        this->debug = debug;
    }
    ~Myclient(){

    }
    /*SET METHODS*/
    void setIp(string ip){
        this ->ip = ip;
    }
    void setIp(string ip, bool debug){

        this ->ip = ip;
        if (debug){
            printf("l'IP è stato settato su %s", this ->ip);
        }
    }

    void setPort(int port){
        this->port = port;
    }
    void setPort(int port, bool debug){
        this->port = port;
        if (debug)
        {
            printf("[PORT SETTED] Myclient PORT = %i", this->port);
        }
    }
    void push_value_to_send(string value){
        {
            lock_guard<mutex> lock(queue_mutex);
            indexqueque.push(value);
        }
        if (debug)
        {
            printf("[PUSH]: %s\n", value.c_str());
        }
        
        
        
    }
    /*THREAD METHODS*/
    void init(){
            if (debug)
            {
                printf("[INIT CLIENT]");
            }
            
            do{    
                comandi.clear();
                while (comandi.size() < 5 ){
                    {
                        lock_guard<mutex> lock(queue_mutex);    
                        //classe lock_guard per assicurare che non ci siano dedadlock, mutex invece serve per bloccare una variabile in modo che non si creino
                        //datarace tra due thread che usano la stessa variabile 
                        
                        if (!indexqueque.empty())
                        {
                            if (debug)
                            {
                                printf("[POP]\n");
                                printf("%s\n",indexqueque.front());
                            }
                            comandi.push_back(indexqueque.front());                            
                            indexqueque.pop();
                        }
                    }
                        std::this_thread::sleep_for(std::chrono::milliseconds(200));
                        //printf("comandi %i", comandi.size());
                }
                start(comandi);
            }while (true);
    }
            

};
#endif // !functions_h


