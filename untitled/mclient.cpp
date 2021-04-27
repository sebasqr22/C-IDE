#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "json.hpp"
#define PORT 8080

using json = nlohmann::json;
using namespace std;
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    json j;
    j["variable1"] = {{"type", "int"}, {"value", 30}, {"memory", 4}};
    j["variable2"] = {{"type", "float"}, {"value", 40.3}, {"memory", 8}};
    j["variable3"] = {{"type", "double"}, {"value", 3240}, {"memory", 8}};
    //tomar la lista de variables de QT, tal vez con for o while
    //while(quedan elementos en la lista)
    //j["nombre de variable"] = {{"tipo de variable","valor de la variable","maybe valor de memoria de la variable"}}
    string s = j.dump();
    cout << s << endl;
    char *message = &s[0];
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , message , strlen(message) , 0 );
    printf("Message sent to server\n");
    //valread = read( sock , buffer, 1024);
    //printf("%s\n",buffer );

    //j = buffer;
    //fstream o("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    //o << setw(4) << j;

    return 0;
}
