//parámetros el puerto en que escucha y el tamaño en bytes de la memoria total

//un malloc, cuando recibe una petición de C!, maneja offsets para determinar posicion de cada 
//variable C! en la memoria real

//escucha peticiones mandados en json, indica tipo de datos, nombre de variable, 
//tamaño de la variable por reserver

//server maneja la memoria

//lleva conteo de referencias cada cierto tiempo maneja garbage collector que elimina espacios 
//de memoria no referenciados
/*
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

using namespace std;

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

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "json.hpp"
<<<<<<< HEAD
#include <sstream>

using json = nlohmann::json;
using namespace std;

=======
#define PORT 8080
>>>>>>> 4145356cd4a31d1798b7c72f631ba157e1fe5a65
int main(int argc, char const *argv[])
{
    int size;
    int port;
    cout << "Por favor indique la cantidad de memoria que deseas apartar en bytes"<<endl;
    cin >> size;
    cout << "Por favor indique el puerto en que deseas escuchar"<<endl;
    cin >> port;
    int *ptr;
    ptr = (int*) malloc(size);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
<<<<<<< HEAD

    //json j;
    
    //ifstream i("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    //i >> j;
    //string s = j.dump();
    
    //char *message = &s[0];
=======
    char *hello = "hi";
>>>>>>> 4145356cd4a31d1798b7c72f631ba157e1fe5a65
       
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

    json j;
    string s;
    stringstream ss;
    ss << buffer;
    ss >> s;
    j = s;
    fstream o("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    o << j;


    //send(new_socket , message , strlen(message) , 0 );
    //printf("Message sent to client\n");
    return 0;
}