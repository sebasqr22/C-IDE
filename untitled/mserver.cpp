/**
 * @file mserver.cpp
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

/**
 * @brief json_recieve Hace los parse respectivos para JSON
 * @param str Recibe un string
 * @return Un Document con el parse
 **/
Document json_recieve(string str){
    const char* pchar = str.c_str();
    Document ptd;
    ptd.Parse(pchar);
    return ptd;
}

/**
 * @brief clase de un nodo utilizado en lista o collector
 **/
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
/**
 * @brief clase lista utilizado para almacenar nodos de variables usadas
 **/
class List{
    public:
        Node* head; //El primer nodo de la lista
        /**
         * @brief Prepara el primer nodo de la lista
         * @param new_head Recibe un puntero de un nodo
        **/
        void set_head(Node* new_head){ //asigna un nodo recibido como el primer nodo de la lista
            head = new_head;
        }

        /**
         * @brief Agrega un nodo al final de la lista
         * @param new_name nombre que será guardado
         * @param new_memory valor de memoria que será guardado
         * @param new_offset valor de offset que será guardado 
         * @param new_count valor de count que será guardado
         * @param new_mark valor de marcador que será guardado
         * @param new_references valor de reference que será guardado
        **/
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
        /**
         * @brief borra un nodo indicado de lista
         * @param del nodo que será borrado de lista
         * @return el nodo que fue borrado
        **/
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

        /**
         * @brief imprime todos los elementos de la lista
        **/
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

        /**
         * @brief función que revisa si algún nodo de la lista tiene el nombre especificado
         * @param input_name el nombre que se desea ver si está en la lista o no
         * @return un bool indicando si existe un nodo con el nombre especificado o no
        **/
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

        /**
         * @brief busca el offset almacenado en un nodo que tenga el nombre especificado
         * @param input_name el nombre especificado para buscar su offset respectivo
         * @return el offset del nodo con el nombre correcto
        **/
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

        /**
         * @brief busca la cantidad de veces que se ha llamado una variable
         * @param input_name el nombre especificado para buscar su conteo respectivo
         * @return la cantidad de veces que se ha llamado la variable
        **/
        int find_count(string input_name){ //Función que retorna el references de un elemento indicado
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

        /**
         * @brief le suma uno al count de todos los elementos en la lista que no estaban presentes en un run
        **/
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

        /**
         * @brief resetea las marcas de todos los nodos a false
        **/
        void reset_marks(){ //función que resetea todas las marcas, preparándo la lista para el siguiente run
            Node* node = head;
            while (node != NULL)
            {
                node->mark = false;
                node = node->next;
            }
        }

};

/**
 * @brief clase collector utilizado para almacenar nodos de variables no usadas
 */
class Collector{
    public:
        Node* head; //El primer nodo del collector
        int length = 0; //el largo de collector
        /**
         * @brief Prepara el primer nodo de collector
         * @param new_head Recibe un puntero de un nodo
        **/
        void set_head(Node* new_head){ //asigna un nodo recibido como el primer nodo de la lista
            head = new_head;
        }
        /**
         * @brief Agrega un nodo al final de collector
         * @param new_name nombre que será guardado
         * @param new_memory valor de memoria que será guardado
         * @param new_offset valor de offset que será guardado 
         * @param new_count valor de count que será guardado
         * @param new_mark valor de marcador que será guardado
         * @param new_references valor de reference que será guardado
        **/
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

        /**
         * @brief borra un nodo indicado de collector
         * @param del nodo que será borrado de collector
         * @return el nodo que fue borrado
        **/
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
    
        /**
         * @brief imprime todos los elementos de collector
        **/
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

        /**
         * @brief verifica si existe algún nodo que almacene 1 byte en collector
         * @return un bool indicando si existe un nodo que almacena 1 byte en él o no
        **/
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

        /**
         * @brief verifica si existe algún nodo que almacene 4 bytes en collector
         * @return un bool indicando si existe un nodo que almacena 4 bytes en él o no
        **/
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

        /**
         * @brief verifica si existe algún nodo que almacene 8 bytes en collector
         * @return un bool indicando si existe un nodo que almacena 8 bytes en él o no
        **/
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

/**
  * @brief función main donde el servidor escucha peticiones del cliente y prepara outputs que serán utilizados por cliente
**/
int main(int argc, char const *argv[])
{   
    //Se pide especificar cuánta memoria apartar en bytes y el puerto en donde se desea escuchar
    int port;
    size_t size;
    cout << "Please indicate the amount of bytes you would like to use" << endl;
    cin >> size;
    cout << "Please indicate which port you would like to listen to" << endl;
    cin >> port;

    //se preparan las variables relevantes a los procesos del server
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

    //Se inicia con una lista vacía y un collector vacío
    Node* head_list = NULL;
    Node* head_collector = NULL;
    Node* tmp_node;
    Node* current_node;
    List list;
    Collector collector;
    list.set_head(head_list);
    collector.set_head(head_collector);

    //se limpia el file en donde se escribirá
    ofstream file;
    file.open ("./untitled/info.txt");
    file << "";
    file.close();

    cout << "Server is now listening" << endl;
    //Luego se procede con las preparaciones del socket
    
    // Creando un "file descriptor" para socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // "attaching" el socket al puerto indicado inicialmente
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );
    
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
    
    //el server comienza su loop para escuchar al cliente
    while(loop == true){
    
        valread = read( new_socket , buffer, 1024); //el server se queda acá esperando una petición del cliente
        printf("%s\n",buffer );
        string s = buffer; //se almacena la petición recibida en un string
        cout << s << endl;

        //verificación de casos
        if (s[0] != 'e'){ //caso cuando todavía no se ha recibido todo el contenido del run
            if (s[0] != 'i'){ //caso cuando no se está escuchando ni el inicio ni el final del run
                i = 0;
                new_s = "";
                while(s[i]!= '}'){
                    new_s += s[i];
                    i += 1;
                }
                new_s += '}';
                //se procesa el JSON recibido en partes individuales
                documentPet = json_recieve(new_s);
                string name = documentPet["name"].GetString();
                string type_value = documentPet["type"].GetString();
                string value_in_string = documentPet["value"].GetString();
                string memory_value = documentPet["memory"].GetString();

                //se preparan variables adicionales 
                string address_str;
                string pointer_address_str;
                int ref_count_int;
                string ref_count_string;
                ostringstream get_address;
                ostringstream get_pointer_address;
                stringstream ss;
                cout <<"TODO: "<< name << type_value << value_in_string << memory_value << endl;

                //se identifica qué tipo de variable fue recibida
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
                if (list.check_list(name) != true){     //caso la variable no está ya presente en la lista          
                            switch(type_value_aux){
                                    case 1:  //caso la variable es un int                                                      
                                        ss << value_in_string;
                                        ss >> numerical_int_value;
                                        if(collector.verify_recycled_four() == false){  //caso no hay un espacio de memoria de 4 bytes disponible en collector                                      
                                            tmp_int_ptr = (int*)(ptr + offset);                            
                                            *tmp_int_ptr = numerical_int_value;  
                                            cout << (int*)(ptr + offset) << endl;
                                            cout << *(int*)(ptr + offset) << endl; 
                                            list.append(name,4,offset,0,true,1);                                          
                                            list.printList();  
                                            offset += stoi(memory_value);  
                                        }
                                        else{ //caso sí hay un espacio de memoria de 4 bytes disponible en collector que será reutilizado
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 4){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,4,tmp_node->offset,0,true,1); 
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
                                    case 2:     //caso la variable es un long                        
                                        ss << value_in_string;
                                        ss >> numerical_long_value;
                                        if(collector.verify_recycled_eight() == false){  //caso no hay un espacio de memoria de 8 bytes disponible en collector     
                                            tmp_long_ptr = (long*)(ptr + offset);                            
                                            *tmp_long_ptr = numerical_long_value;  
                                            cout << (long*)(ptr + offset) << endl;
                                            cout << *(long*)(ptr + offset) << endl;  
                                            list.append(name,8,offset,0,true,1);
                                            list.printList();                                            
                                            offset += stoi(memory_value);    
                                        }
                                        else{ //caso sí hay un espacio de memoria de 4 bytes disponible en collector 
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 8){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,8,tmp_node->offset,0,true,1); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_long_ptr = (long*)(ptr + tmp_node->offset);                            
                                            *tmp_long_ptr = numerical_long_value;   
                                        }
                                        get_address << tmp_long_ptr;
                                        address_str = get_address.str();
                                        break;                          
                                    case 3: //caso la variable es un char
                                        if(collector.verify_recycled_one() == false){ //caso no hay un espacio de memoria de 1 byte disponible en collector 
                                            tmp_char_ptr = (ptr+offset);                          
                                            *tmp_char_ptr = value_in_string[0];  
                                            cout << (int*)(ptr + offset) << endl;
                                            cout << *(ptr + offset) << endl;     
                                            list.append(name,1,offset,0,true,1);
                                            list.printList();                         
                                            offset += stoi(memory_value);          
                                        }         
                                        else{  //caso sí hay un espacio de memoria de 1 byte disponible en collector 
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 1){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,1,tmp_node->offset,0,true,1); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_char_ptr = (ptr + tmp_node->offset);                            
                                            *tmp_char_ptr = value_in_string[0];   
                                        }   
                                        get_address << (int*)(tmp_char_ptr);
                                        address_str = get_address.str();                  
                                        break;                            
                                    case 4:  //caso la variable es un float
                                        ss << value_in_string;
                                        ss >> numerical_float_value;
                                        if(collector.verify_recycled_four() == false){  //caso no hay un espacio de memoria de 4 bytes disponible en collector 
                                            tmp_float_ptr = (float*)(ptr + offset);                            
                                            *tmp_float_ptr = numerical_float_value;
                                            cout << (float*)(ptr + offset) << endl;
                                            cout << *(float*)(ptr + offset) << endl; 
                                            list.append(name,4,offset,0,true,1);
                                            list.printList();                                 
                                            offset += stoi(memory_value);    
                                        }
                                        else{  //caso sí hay un espacio de memoria de 4 bytes disponible en collector 
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 4){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,4,tmp_node->offset,0,true,1); 
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
                                    case 5: //caso la variable es un double
                                        ss << value_in_string;
                                        ss >> numerical_double_value;
                                        if(collector.verify_recycled_eight() == false){  //caso no hay un espacio de memoria de 8 bytes disponible en collector 
                                            tmp_double_ptr = (double*)(ptr+offset);                            
                                            *tmp_double_ptr = numerical_double_value;    
                                            cout << (double*)(ptr + offset) << endl;
                                            cout << *(double*)(ptr + offset) << endl; 
                                            list.append(name,8,offset,0,true,1);
                                            list.printList();                            
                                            offset += stoi(memory_value);      
                                        } 
                                        else{  //caso sí hay un espacio de memoria de 8 bytes disponible en collector 
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 8){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,8,tmp_node->offset,0,true,1); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_double_ptr = (double*)(ptr + tmp_node->offset);                            
                                            *tmp_double_ptr = numerical_double_value;  
                                        }                
                                        get_address << tmp_double_ptr;
                                        address_str = get_address.str();                          
                                        break;
                                        
                                    case 6: //caso la variable es un reference
                                        if(collector.verify_recycled_four() == false){ //caso no hay un espacio de memoria de 4 bytes disponible en collector 
                                            tmp_ref_ptr = (int*)(ptr+offset);   
                                            tmp_ref_ptr_value = (int*)(ptr + (list.find_offset(value_in_string)));  
                                                               
                                            list.append(name,4,offset,0,true,1);
                                            list.printList();                         
                                            offset += stoi(memory_value);          
                                        }         
                                        else{  //caso sí hay un espacio de memoria de 4 bytes disponible en collector 
                                            current_node = collector.head;
                                            while (current_node != NULL){
                                                if (current_node->memory_value == 4){ 
                                                    tmp_node = collector.deleteNode(current_node);
                                                    list.append(name,4,tmp_node->offset,0,true,1); 
                                                }
                                                current_node = current_node->next;
                                            }
                                            tmp_ref_ptr = (int*)(ptr + tmp_node->offset);                            
                                            tmp_ref_ptr_value = (int*)(ptr + (list.find_offset(value_in_string))); 
                                        }   
                                        get_address << (int*)(tmp_ref_ptr);
                                        address_str = get_address.str();  
                                        get_pointer_address << (int*)(tmp_ref_ptr_value);    
                                        pointer_address_str = get_pointer_address.str();
                                        value_in_string = pointer_address_str;                   
                                        break;
                                        
                            } 
                            if (offset > size){ //caso se llenó la memoria
                                cout << "error, there is no memory left"<< endl;
                            }

                    //Se procede a tomar los datos procesados y se escriben en un documento .txt        
                    jsonEnviar = R"({"name":")"+ name + R"(","type":")" + type_value + R"(","value":")" + value_in_string + R"(","memory":")" + memory_value + R"(","address":")" + address_str + R"(","count":")" + "1" + "\"}";
                    string string_send = jsonEnviar;
                    cout << string_send << endl;
                    ofstream myfile;
                    myfile.open ("./untitled/info.txt",fstream::app);
                    myfile << string_send + "\n";
                    myfile.close();
                }
                else{  //caso la variable ya existe en la lista
                    switch(type_value_aux){
                            case 1:     //caso la variable es un int
                                ss << value_in_string;
                                ss >> numerical_int_value;                                                                                                                                 
                                tmp_int_ptr = (int*)(ptr + list.find_offset(name));                            
                                *tmp_int_ptr = numerical_int_value;  
                                get_address << tmp_int_ptr;
                                address_str = get_address.str();     
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);
                                break;
                            case 2:    //caso la variable es un long
                                ss << value_in_string;
                                ss >> numerical_long_value;                                              
                                tmp_long_ptr = (long*)(ptr + list.find_offset(name));                            
                                *tmp_long_ptr = numerical_long_value;                   
                                get_address << tmp_long_ptr;
                                address_str = get_address.str();
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);
                                break;                          
                            case 3:          //caso la variable es un char                                                      
                                tmp_char_ptr = (ptr + list.find_offset(name));                          
                                *tmp_char_ptr = value_in_string[0];                   
                                get_address << (int*)(tmp_char_ptr);
                                address_str = get_address.str(); 
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);                 
                                break;                            
                            case 4:   //caso la variable es un float
                                ss << value_in_string;
                                ss >> numerical_float_value;                                 
                                tmp_float_ptr = (float*)(ptr + list.find_offset(name));                            
                                *tmp_float_ptr = numerical_float_value;                                                
                                get_address << tmp_float_ptr;
                                address_str = get_address.str();
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);
                                break;                            
                            case 5:  //caso la variable es un double
                                ss << value_in_string;
                                ss >> numerical_double_value;                                
                                tmp_double_ptr = (double*)(ptr + list.find_offset(name));                            
                                *tmp_double_ptr = numerical_double_value;                                                
                                get_address << tmp_double_ptr;
                                address_str = get_address.str();  
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);                        
                                break;
                                
                            case 6:  //caso la variable es un reference
                                tmp_ref_ptr = (int*)(ptr+list.find_offset(name));   
                                tmp_ref_ptr_value = (int*)(ptr + (list.find_offset(value_in_string)));                                    
                                get_address << (int*)(tmp_ref_ptr);
                                address_str = get_address.str();  
                                get_pointer_address << (int*)(tmp_ref_ptr_value);    
                                pointer_address_str = get_pointer_address.str();
                                value_in_string = pointer_address_str;        
                                ref_count_int = list.find_count(name);
                                ref_count_string = to_string(ref_count_int);
                                break;
                                
                            } 
                    //Se procede a tomar los datos procesados y se escriben en un documento .txt
                    jsonEnviar = R"({"name":")"+ name + R"(","type":")" + type_value + R"(","value":")" + value_in_string + R"(","memory":")" + memory_value + R"(","address":")" + address_str + R"(","count":")" + ref_count_string + "\"}";
                    string string_send = jsonEnviar;
                    cout << string_send << endl;
                    ofstream myfile;
                    myfile.open ("./untitled/info.txt",fstream::app);
                    myfile << string_send + "\n";
                    myfile.close();
                }
                
            }
            else{ //caso inicio de un run 
                file.open ("./untitled/info.txt");
                file << "";
                file.close();
            }
        }
        else{       //caso final de un run   
            cout << "end entered else" << endl;  
            list.add_counts(); //se suma uno a los counts de las variables no referenciadas que están en lista
            current_node = list.head;
            while (current_node != NULL){
                if (current_node->count >= 3){ //se borran las variables que tienen 3 runs sin ser referenciados
                    tmp_node = list.deleteNode(current_node);
                    collector.append(tmp_node->name,tmp_node->memory_value,tmp_node->offset,0,true,0); //adds the deleted node to collector
                }
                current_node = current_node->next;
            }

            list.reset_marks(); //todos los marks se vuelven false
            cout << "final list" << endl;
            list.printList();
            cout << endl;
            collector.printList();
            cout << endl;
        }
        if (offset > size){ //caso se llenó la memoria
            cout << "error, there is no memory left"<< endl;
            loop = false;
        }
    }
    return 0;
}