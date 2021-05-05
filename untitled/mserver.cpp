/**
 * @file mserver.cpp
 * @title MServer 
 * @brief Clase servidor
**/

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <cstdlib>
#include <netinet/in.h>
#include <string.h>
#include <sstream>
#include "rapidjson/document.h"


using namespace std;
using namespace rapidjson;


Document json_recieve(string str){
    const char* pchar = str.c_str();
    Document ptd;
    ptd.Parse(pchar);
    return ptd;
}

// A linked list node
class Node
{
    public:
        string name; //nombre de la variable
        int memory_value; //cantidad de memoria requerida para almacenar la variable
        int offset; //el offset en donde se encuentra la memoria en la variable
        int count; //la cantidad de runs que la variable tiene sin ser utilizado
        bool mark; //la marca de variable indicando si se encuentra presente en cada run del cliente
        int references; //la cantidad de veces que la variable ha estado presente
        Node* next; //puntero hacia el nodo siguiente en la lista
        Node* prev; //puntero hacia el nodo anterior en la lista
};

class List{
    public:
        Node* head; //El primer nodo de la lista
        /**
         * @brief
         * @param
        **/
        void set_head(Node* new_head){ //asigna un nodo recibido como el primer nodo de la lista
            head = new_head;
        }
        void append(string new_name, int new_memory, int new_offset, int new_count, bool new_mark, int new_references) //dado los datos, añade un nodo con los datos recibidos al final de la lista
        {
            Node* new_node = new Node();  //se crea un nodo nuevo     
            Node* last = head;

            // se asignan los valores al nuevo nodo
            new_node->name = new_name;
            new_node->memory_value = new_memory;
            new_node->offset = new_offset;
            new_node->count = new_count;
            new_node->mark = new_mark;
            new_node->references = new_references;
            new_node->next = NULL;

            if (head == NULL) //caso añadir nodo a una lista vacía
            {
                new_node->prev = NULL;
                head = new_node;
                return;
            }
            while (last->next != NULL) //while para llegar al final de la lista
                {last = last->next;}
            last->next = new_node; //se asigna el último nodo como el nodo nuevo
            new_node->prev = last;        
            return;
        }

        Node* deleteNode(Node* del) //función para borrar un nodo dado de la lista, este nodo es luego retornado para luego ser agregado a collector
        {
            if (head == NULL || del == NULL) //caso particular
                {//return;
                }
        
            if (head == del) //caso cuando el nodo que será borrado es el primer nodo
                {head = del->next;}
        
            if (del->next != NULL) //el nodo siguiente se cambia siempre y cuando no es el último nodo de la lista
                {del->next->prev = del->prev;}
        
            if (del->prev != NULL) //el nodo anterior se cambia siempre y cuando no es el primero nodo de la lista
                {del->prev->next = del->next;}
        
            //Se retorna el nodo que será borrado
            return del;
        }
    
        void printList() //Función que imprime todos los nombres de los elementos en la lista
        {
            Node* node = head;
            cout<<"\nEstado de la lista: \n";
            while (node != NULL)
            {
                cout<<" "<<node->name<<" ";
                node = node->next;
            }
        }

        bool check_list(string input_name){ //Función que revisa si el elemento dado existe en la lista
            Node* node = head;
            while (node != NULL)
            {
                if (node->name == input_name){
                    node->count = 0; //Se resetea el conteo de no estar presente en runs
                    node->references += 1; //se suma uno a la cantidad de veces llamado
                    node->mark = true; //Se marca el elemento como presente
                    return true;
                }
                else{
                    node = node->next;
                }
            }
            return false;
        }

        int find_offset(string input_name){ //Función que retorna el offset de un elemento indicado
            Node* node = head;
            while (node != NULL)
            {
                if (node->name == input_name){
                    return node->offset;
                }
                else{
                    node = node->next;
                }
            }
            return 0;
        }

        int find_count(string input_name){ //Función que retorna el offset de un elemento indicado
            Node* node = head;
            while (node != NULL)
            {
                if (node->name == input_name){
                    return node->references;
                }
                else{
                    node = node->next;
                }
            }
            return 0;
        }

        void add_counts(){ //función que suma un count a todos los elementos que no estaban presentes en el run
            Node* node = head;
            while (node != NULL)
            {
                if (node->mark == false){
                    node->count += 1;
                }
                node = node->next;
            }
        }

        void reset_marks(){ //función que resetea todas las marcas, preparándo la lista para el siguiente run
            Node* node = head;
            while (node != NULL)
            {
                node->mark = false;
                node = node->next;
            }
        }

};

class Collector{
    public:
        Node* head; //El primer nodo del collector
        int length = 0; //el largo de collector
        void set_head(Node* new_head){ //asigna un nodo recibido como el primer nodo de la lista
            head = new_head;
        }
        void append(string new_name, int new_memory, int new_offset, int new_count, bool new_mark, int new_references) //dado los datos, añade un nodo con los datos recibidos al final de collector
        {
            Node* new_node = new Node();  //se crea un nodo nuevo     
            Node* last = head;

            // se asignan los valores al nuevo nodo
            new_node->name = new_name;
            new_node->memory_value = new_memory;
            new_node->offset = new_offset;
            new_node->count = new_count;
            new_node->mark = new_mark;
            new_node->references = new_references;
            new_node->next = NULL;

            if (head == NULL) //caso añadir nodo a una lista vacía
            {
                new_node->prev = NULL;
                head = new_node;
                return;
            }
            while (last->next != NULL) //while para llegar al final de la lista
                {last = last->next;}
            last->next = new_node; //se asigna el último nodo como el nodo nuevo
            new_node->prev = last;        
            return;
        }

        Node* deleteNode(Node* del) //función para borrar un nodo dado del collector, este nodo es luego retornado para luego ser agregado a lista
        {
            if (head == NULL || del == NULL) //caso particular
                {//return;
                }
        
            if (head == del) //caso cuando el nodo que será borrado es el primer nodo
                {head = del->next;}
        
            if (del->next != NULL) //el nodo siguiente se cambia siempre y cuando no es el último nodo de la lista
                {del->next->prev = del->prev;}
        
            if (del->prev != NULL) //el nodo anterior se cambia siempre y cuando no es el primero nodo de la lista
                {del->prev->next = del->next;}
        
            //Se retorna el nodo que será borrado
            return del;
        }
    
        void printList() //Función que imprime todos los nombres de los elementos en el collector
        {
            Node* node = head;
            cout<<"\nEstado de collector: \n";
            while (node != NULL)
            {
                cout<<" "<<node->name<<" ";
                node = node->next;
            }
        }

        bool verify_recycled_one(){ //verifica si hay algún nodo presente donde se puede almacenar una variable de 1 byte
            Node* node = head;
            while (node != NULL)
            {
                if(node->memory_value == 1){
                    return true;
                }
                node = node->next;
            }
            return false;
        }

        bool verify_recycled_four(){ //verifica si hay algún nodo presente donde se puede almacenar una variable de 4 bytes
            Node* node = head;
            while (node != NULL)
            {
                if(node->memory_value == 4){
                    return true;
                }
                node = node->next;
            }
            return false;
        }

        bool verify_recycled_eight(){ //verifica si hay algún nodo presente donde se puede almacenar una variable de 8 bytes
            Node* node = head;
            while (node != NULL)
            {
                if(node->memory_value == 8){
                    return true;
                }
                node = node->next;
            }
            return false;
        }
};

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

    int *tmp_int_ptr;
    long *tmp_long_ptr;
    char *tmp_char_ptr;
    float *tmp_float_ptr;
    double *tmp_double_ptr;
    int *tmp_ref_ptr;
    int *tmp_ref_ptr_value;

    bool loop = true;

    /* Start with the empty list */
    Node* head_list = NULL;
    Node* head_collector = NULL;
    Node* tmp_node;
    Node* current_node;
    List list;
    Collector collector;
    list.set_head(head_list);
    collector.set_head(head_collector);
    ofstream file;
    file.open ("./untitled/example.txt");
    file << "";
    file.close();

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
    
    while(loop == true){
    
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        string s = buffer;
        cout << s << endl;
        if (s[0] != 'e'){
            if (s[0] != 'i'){
                i = 0;
                new_s = "";
                while(s[i]!= '}'){
                    new_s += s[i];
                    i += 1;
                }
                new_s += '}';
                documentPet = json_recieve(new_s);
                string name = documentPet["name"].GetString();
                string type_value = documentPet["type"].GetString();
                string value_in_string = documentPet["value"].GetString();
                string memory_value = documentPet["memory"].GetString();
                string address_str;
                int ref_count_int;
                string ref_count_string;
                ostringstream get_address;
                stringstream ss;
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
                        else if (type_value == "reference"){ //confirmar con partner
                            type_value_aux = 6;
                        } 
                if (list.check_list(name) != true){              
                            switch(type_value_aux){
                                    case 1:                                                        
                                        ss << value_in_string;
                                        ss >> numerical_int_value;
                                        if(collector.verify_recycled_four() == false){                                        
                                            tmp_int_ptr = (int*)(ptr + offset);                            
                                            *tmp_int_ptr = numerical_int_value;  
                                            cout << (int*)(ptr + offset) << endl;
                                            cout << *(int*)(ptr + offset) << endl; 
                                            list.append(name,4,offset,0,true,0);                                          
                                            list.printList();  
                                            offset += stoi(memory_value);  
                                        }
                                        else{
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 4){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,4,tmp_node->offset,0,true,0); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_int_ptr = (int*)(ptr + tmp_node->offset);                            
                                            *tmp_int_ptr = numerical_int_value;  
                                            cout << (int*)(ptr + tmp_node->offset) << endl;
                                            cout << *(int*)(ptr + tmp_node->offset) << endl;  
                                        }
                                        get_address << tmp_int_ptr;
                                        address_str = get_address.str();                     
                                        break;
                                    case 2:                             
                                        ss << value_in_string;
                                        ss >> numerical_long_value;
                                        if(collector.verify_recycled_eight() == false){      
                                            tmp_long_ptr = (long*)(ptr + offset);                            
                                            *tmp_long_ptr = numerical_long_value;  
                                            cout << (long*)(ptr + offset) << endl;
                                            cout << *(long*)(ptr + offset) << endl;  
                                            list.append(name,8,offset,0,true,0);
                                            list.printList();                                            
                                            offset += stoi(memory_value);    
                                        }
                                        else{
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 8){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,8,tmp_node->offset,0,true,0); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_long_ptr = (long*)(ptr + tmp_node->offset);                            
                                            *tmp_long_ptr = numerical_long_value;   
                                        }
                                        get_address << tmp_long_ptr;
                                        address_str = get_address.str();
                                        break;                          
                                    case 3:
                                        if(collector.verify_recycled_one() == false){ 
                                            tmp_char_ptr = (ptr+offset);                          
                                            *tmp_char_ptr = value_in_string[0];  
                                            cout << (int*)(ptr + offset) << endl;
                                            cout << *(ptr + offset) << endl;     
                                            list.append(name,1,offset,0,true,0);
                                            list.printList();                         
                                            offset += stoi(memory_value);          
                                        }         
                                        else{
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 1){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,1,tmp_node->offset,0,true,0); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_char_ptr = (ptr + tmp_node->offset);                            
                                            *tmp_char_ptr = value_in_string[0];   
                                        }   
                                        get_address << (int*)(tmp_char_ptr);
                                        address_str = get_address.str();                  
                                        break;                            
                                    case 4:
                                        ss << value_in_string;
                                        ss >> numerical_float_value;
                                        if(collector.verify_recycled_four() == false){
                                            tmp_float_ptr = (float*)(ptr + offset);                            
                                            *tmp_float_ptr = numerical_float_value;
                                            cout << (float*)(ptr + offset) << endl;
                                            cout << *(float*)(ptr + offset) << endl; 
                                            list.append(name,4,offset,0,true,0);
                                            list.printList();                                 
                                            offset += stoi(memory_value);    
                                        }
                                        else{
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 4){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,4,tmp_node->offset,0,true,0); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_float_ptr = (float*)(ptr + tmp_node->offset);                            
                                            *tmp_float_ptr = numerical_float_value;  
                                            cout << (float*)(ptr + tmp_node->offset) << endl;
                                            cout << *(float*)(ptr + tmp_node->offset) << endl;  
                                        }                  
                                        get_address << tmp_float_ptr;
                                        address_str = get_address.str();
                                        break;                            
                                    case 5:
                                        ss << value_in_string;
                                        ss >> numerical_double_value;
                                        if(collector.verify_recycled_eight() == false){
                                            tmp_double_ptr = (double*)(ptr+offset);                            
                                            *tmp_double_ptr = numerical_double_value;    
                                            cout << (double*)(ptr + offset) << endl;
                                            cout << *(double*)(ptr + offset) << endl; 
                                            list.append(name,8,offset,0,true,0);
                                            list.printList();                            
                                            offset += stoi(memory_value);      
                                        } 
                                        else{
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 8){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,8,tmp_node->offset,0,true,0); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_double_ptr = (double*)(ptr + tmp_node->offset);                            
                                            *tmp_double_ptr = numerical_double_value;  
                                        }                
                                        get_address << tmp_double_ptr;
                                        address_str = get_address.str();                          
                                        break;
                                        
                                    case 6:
                                        if(collector.verify_recycled_four() == false){ 
                                            tmp_ref_ptr = (int*)(ptr+offset);   
                                            //tmp_ref_ptr_value = (int*)(ptr + (list.find_offset(value_in_string)));                       
                                            //*tmp_ref_ptr = to_string(tmp_ref_ptr_value);  
                                            cout << (int*)(ptr + offset) << endl;
                                            cout << *(ptr + offset) << endl;     
                                            list.append(name,4,offset,0,true,0);
                                            list.printList();                         
                                            offset += stoi(memory_value);          
                                        }         
                                        else{
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 4){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,4,tmp_node->offset,0,true,0); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            //tmp_ref_ptr = (ptr + tmp_node->offset);                            
                                            //*tmp_ref_ptr = value_in_string[0];   
                                        }   
                                        get_address << (int*)(tmp_ref_ptr);
                                        address_str = get_address.str();                   
                                        break;
                                        
                            } 
                            if (offset > size){
                                cout << "error, there is no memory left"<< endl;
                            }
                                    
                    jsonEnviar = R"({"name":")"+ name + R"(","type":")" + type_value + R"(","value":")" + value_in_string + R"(","memory":")" + memory_value + R"(","address":")" + address_str + R"(","count":")" + "0" + "\"}";
                    string string_send = jsonEnviar;
                    cout << string_send << endl;
                    ofstream myfile;
                    myfile.open ("./untitled/example.txt",fstream::app);
                    myfile << string_send + "\n";
                    myfile.close();
                }
                else{
                    switch(type_value_aux){
                            case 1:     
                                ss << value_in_string;
                                ss >> numerical_int_value;                                                                                             int *tmp_int_ptr;                                      
                                tmp_int_ptr = (int*)(ptr + list.find_offset(name));                            
                                *tmp_int_ptr = numerical_int_value;  
                                get_address << tmp_int_ptr;
                                address_str = get_address.str();     
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);
                                break;
                            case 2:    
                                ss << value_in_string;
                                ss >> numerical_long_value;                                              
                                tmp_long_ptr = (long*)(ptr + list.find_offset(name));                            
                                *tmp_long_ptr = numerical_long_value;                   
                                get_address << tmp_long_ptr;
                                address_str = get_address.str();
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);
                                break;                          
                            case 3:                                                                
                                tmp_char_ptr = (ptr + list.find_offset(name));                          
                                *tmp_char_ptr = value_in_string[0];                   
                                get_address << (int*)(tmp_char_ptr);
                                address_str = get_address.str(); 
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);                 
                                break;                            
                            case 4:
                                ss << value_in_string;
                                ss >> numerical_float_value;                                 
                                tmp_float_ptr = (float*)(ptr + list.find_offset(name));                            
                                *tmp_float_ptr = numerical_float_value;                                
                                offset += stoi(memory_value);                  
                                get_address << tmp_float_ptr;
                                address_str = get_address.str();
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);
                                break;                            
                            case 5:
                                ss << value_in_string;
                                ss >> numerical_double_value;                                
                                tmp_double_ptr = (double*)(ptr + list.find_offset(name));                            
                                *tmp_double_ptr = numerical_double_value;                               
                                offset += stoi(memory_value);                  
                                get_address << tmp_double_ptr;
                                address_str = get_address.str();  
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);                        
                                break;
                                
                            case 6:
                                //caso ref
                                break;
                                
                            } 
                    jsonEnviar = R"({"name":")"+ name + R"(","type":")" + type_value + R"(","value":")" + value_in_string + R"(","memory":")" + memory_value + R"(","address":")" + address_str + R"(","count":")" + ref_count_string + "\"}";
                    string string_send = jsonEnviar;
                    cout << string_send << endl;
                    ofstream myfile;
                    myfile.open ("./untitled/example.txt",fstream::app);
                    myfile << string_send + "\n";
                    myfile.close();
                }
                
            }
            else{
                file.open ("./untitled/example.txt");
                file << "";
                file.close();
            }
        }
        else{          
            cout << "end entered else" << endl;  
            list.add_counts(); //add variables count that werent use
            current_node = list.head;
            while (current_node != NULL){
                if (current_node->count >= 3){ //deletes variables with counts higher than 2
                    tmp_node = list.deleteNode(current_node);
                    collector.append(tmp_node->name,tmp_node->memory_value,tmp_node->offset,0,true,0); //adds the deleted node to collector
                }
                current_node = current_node->next;
            }

            list.reset_marks(); //set all marks to false
            cout << "final list" << endl;
            list.printList();
            cout << endl;
            collector.printList();
            cout << endl;
        }
        if (offset > size){
            cout << "error, there is no memory left"<< endl;
            loop = false;
        }
    }
    return 0;
}