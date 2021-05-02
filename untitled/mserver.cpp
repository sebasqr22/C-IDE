#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <cstdlib>
#include <netinet/in.h>
#include <string.h>
#include "json.hpp"
#include <sstream>
#include "rapidjson/document.h"
#include <SFML/Network.hpp>


using json = nlohmann::json;
using namespace std;
using namespace rapidjson;
using namespace sf;


Document json_recieve(Packet packet){
    string tmp;
    packet >> tmp;
    const char* pchar = tmp.c_str();
    Document ptd;
    ptd.Parse(pchar);
    return ptd;
}

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
    int numerical_int_value;
    long numerical_long_value;
    float numerical_float_value;
    double numerical_double_value;
    Document documentPet;
    string jsonEnviar;
    string new_s;

    //varibales nuevas sockets
    IpAddress ip = IpAddress::getLocalAddress();
    TcpSocket newSocket;
    Packet sendP, recieveP;
    string newJson;
    TcpListener listener;
    listener.listen(port);
    listener.accept(newSocket);

    
    cout << "Server is now listening" << endl;
    /*
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
    */
    while(true){
        if(recieveP.getData() == NULL){
            //fuck
        }
        else{
            /*
            valread = read( new_socket , buffer, 1024);
            printf("%s\n",buffer );
            string s = buffer;
            cout << s << endl;
            i = 0;
            new_s = "";
            while(s[i]!= '}'){
                new_s += s[i];
                i += 1;
            }
            new_s += '}';
            */

            documentPet = json_recieve(recieveP);
            string name = documentPet["name"].GetString();
            string type_value = documentPet["type"].GetString();
            string value_in_string = documentPet["value"].GetString();
            string memory_value = documentPet["memory"].GetString();
            cout <<"TODO: "<< name << type_value << value_in_string << memory_value << endl;
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
                    string address_str;
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
                                offset += stoi(memory_value);  
                                get_address << tmp_int_ptr;
                                address_str = get_address.str();                     
                                break;
                            case 2:                             
                                ss << value_in_string;
                                ss >> numerical_long_value;
                                long *tmp_long_ptr;
                                tmp_long_ptr = (long*)(ptr + offset);                            
                                *tmp_long_ptr = numerical_long_value;  
                                cout << (long*)(ptr + offset) << endl;
                                cout << *(long*)(ptr + offset) << endl;       
                                offset += stoi(memory_value);                  
                                get_address << tmp_long_ptr;
                                address_str = get_address.str();
                                break;                          
                            case 3:
                                char *tmp_char_ptr;
                                tmp_char_ptr = (ptr+offset);                          
                                *tmp_char_ptr = value_in_string[0];  
                                cout << (int*)(ptr + offset) << endl;
                                cout << *(ptr + offset) << endl;                             
                                offset += stoi(memory_value);                  
                                get_address << (int*)(tmp_char_ptr);
                                address_str = get_address.str();                  
                                break;                            
                            case 4:
                                ss << value_in_string;
                                ss >> numerical_float_value;
                                float *tmp_float_ptr;
                                tmp_float_ptr = (float*)(ptr + offset);                            
                                *tmp_float_ptr = numerical_float_value;
                                cout << (float*)(ptr + offset) << endl;
                                cout << *(float*)(ptr + offset) << endl;                                 
                                offset += stoi(memory_value);                  
                                get_address << tmp_float_ptr;
                                address_str = get_address.str();
                                break;                            
                            case 5:
                                ss << value_in_string;
                                ss >> numerical_double_value;
                                double *tmp_double_ptr;
                                tmp_double_ptr = (double*)(ptr+offset);                            
                                *tmp_double_ptr = numerical_double_value;    
                                cout << (double*)(ptr + offset) << endl;
                                cout << *(double*)(ptr + offset) << endl;                            
                                offset += stoi(memory_value);                  
                                get_address << tmp_double_ptr;
                                address_str = get_address.str();                          
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
            jsonEnviar = R"({"name":")"+ name + R"(","type":")" + type_value + R"(","value":")" + value_in_string + R"(","memory":")" + memory_value + R"(","address":")" + address_str + "\"}";
            sendP << jsonEnviar;
            newSocket.send(sendP);
            recieveP.clear();
            sendP.clear();
            /*
            string string_send = jsonEnviar;
            cout << string_send << endl;
            string nepe = "nepererepe de pepe";
            char *message = &nepe[0];
            cout << "MENSAJE: " << message << endl;
            cout << "PINGA DE TORO" << endl;
            int n = write(8080, message, strlen(message));
            //send(8080 , message , strlen(message) , 0 );//Se envia la info
            cout << "PUTI KK SI SIRVE" << endl;
            */
        }
    }
    return 0;
}//main