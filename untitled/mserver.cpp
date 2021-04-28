//parámetros el puerto en que escucha y el tamaño en bytes de la memoria total

//un malloc, cuando recibe una petición de C!, maneja offsets para determinar posicion de cada 
//variable C! en la memoria real

//escucha peticiones mandados en json, indica tipo de datos, nombre de variable, 
//tamaño de la variable por reserver

//server maneja la memoria

//lleva conteo de referencias cada cierto tiempo maneja garbage collector que elimina espacios 
//de memoria no referenciados
/*

int main(int port, size_t size){ //size en bytes
    int *ptr;
    ptr = (int*) malloc(size);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int setsock(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    int listen(int sockfd, int backlog);
    int new_socket = accept(int sockfd, struct sockaddr *addr, socklen_t addrlen);
    //ptr + i; offset
    
    return 0;
}*/

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "json.hpp"
#include <sstream>

using json = nlohmann::json;
using namespace std;

int main(int argc, char const *argv[])
{   
    int port;
    size_t size;
    cout << "Please indicate the amount of bytes you would like to use" << endl;
    cin >> size;
    cout << "Please indicate which port you would like to listen to" << endl;
    cin >> port;
    char *ptr;
    int i = 0;
    ptr = (char*) malloc(size);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    int offset = 0;
    string name;
    stringstream ss;
    int numerical_int_value;
    long numerical_long_value;
    float numerical_float_value;
    double numerical_double_value;

    cout << "Server is now listening" << endl;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    cout << "buffer" << buffer << endl;

    json j = buffer;
    //{" hola ":{"memory":4,"type":"int","value":" 324"}}
    string s = j.dump();    
    
    while (s[i] != '}' || s[i+1] == ','){         
        if (s[i] == '"'){
                string name = "";
                i += 1;
                while (s[i] != '"'){
                    name += s[i];
                    i += 1;
                }
                cout << name << endl;
                
                string type_value = j[name]["type"];
                string value_in_string = j[name]["value"];
                int memory_value = j[name]["memory"];
                int type_value_aux;
                if (type_value == "int"){
                    type_value_aux = 1;
                } 
                else if (type_value == "long"){
                    type_value_aux = 2;
                }
                else if (type_value == "char"){
                    type_value_aux = 3;
                }
                else if (type_value == "float"){
                    type_value_aux = 4;
                }
                else if (type_value == "double"){
                    type_value_aux = 5;
                } 
                stringstream ss;
                ostringstream get_address;
                string str;
                switch(type_value_aux){
                        case 1:                                                        
                            ss << value_in_string;
                            ss >> numerical_int_value;
                            cout << numerical_int_value<< endl;
                            int *tmp_int_ptr;
                            tmp_int_ptr = (int*)(ptr + offset);                            
                            *tmp_int_ptr = numerical_int_value;  
                            cout << (int*)(ptr + offset) << endl;
                            cout << *(int*)(ptr + offset) << endl;                    
                            offset += memory_value;   
                            get_address << tmp_int_ptr;
                            str = get_address.str();
                            j[name]["address"] = str;
                            
                            break;
                        case 2:                             
                            ss << value_in_string;
                            ss >> numerical_long_value;
                            long *tmp_long_ptr;
                            tmp_long_ptr = (long*)(ptr + offset);                            
                            *tmp_long_ptr = numerical_long_value;  
                            cout << (long*)(ptr + offset) << endl;
                            cout << *(long*)(ptr + offset) << endl;       
                            offset += memory_value;                  
                            get_address << tmp_long_ptr;
                            str = get_address.str();
                            j[name]["address"] = str;
                            break;                          
                        case 3:
                            char *tmp_char_ptr;
                            tmp_char_ptr = (ptr+offset);                          
                            *tmp_char_ptr = value_in_string[0];  
                            cout << (int*)(ptr + offset) << endl;
                            cout << *(ptr + offset) << endl;                             
                            offset += memory_value;                  
                            get_address << (int*)(tmp_char_ptr);
                            str = get_address.str();
                            j[name]["address"] = str;                   
                            break;                            
                        case 4:
                            ss << value_in_string;
                            ss >> numerical_float_value;
                            float *tmp_float_ptr;
                            tmp_float_ptr = (float*)(ptr + offset);                            
                            *tmp_float_ptr = numerical_float_value;
                            cout << (float*)(ptr + offset) << endl;
                            cout << *(float*)(ptr + offset) << endl;                                 
                            offset += memory_value;                  
                            get_address << tmp_float_ptr;
                            str = get_address.str();
                            j[name]["address"] = str;
                            break;                            
                        case 5:
                            ss << value_in_string;
                            ss >> numerical_double_value;
                            double *tmp_double_ptr;
                            tmp_double_ptr = (double*)(ptr+offset);                            
                            *tmp_double_ptr = numerical_double_value;    
                            cout << (double*)(ptr + offset) << endl;
                            cout << *(double*)(ptr + offset) << endl;                            
                            offset += memory_value;                  
                            get_address << tmp_double_ptr;
                            str = get_address.str();
                            j[name]["address"] = str;                            
                            break;
                            
                        case 6:
                            //caso struct
                            break;
                        case 7:
                            //caso reference
                            break;
                } 
                if (offset > size){
                    cout << "error, there is no memory left"<< endl;
                }
            }else if (s[i] == ':'){
                while (s[i] != '}'){
                    i += 1;
                }
            }
            i += 1;
        
    } 
    s = j.dump();
    //send this json to C!
    
    return 0;
}