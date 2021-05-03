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
    string name;
    int memory_value;
    int offset;
    int count = 0;
    bool mark = false;
    Node* next;
    Node* prev;
};

class List{
    public:
    Node* head;
    void set_head(Node* new_head){
        head = new_head;
    }
    /* Given a reference (pointer to pointer) to the head
    of a DLL and an int, appends a new node at the end */
    void append(string new_name, int new_memory, int new_offset)
    {
        /* 1. allocate node */
        Node* new_node = new Node();
    
        Node* last = head; /* used in step 5*/
    
        /* 2. put in the data */
        new_node->name = new_name;
        new_node->memory_value = new_memory;
        new_node->offset = new_offset;
    
        /* 3. This new node is going to be the last node, so
            make next of it as NULL*/
        new_node->next = NULL;
    
        /* 4. If the Linked List is empty, then make the new
            node as head */
        if (head == NULL)
        {
            new_node->prev = NULL;
            head = new_node;
            return;
        }
    
        /* 5. Else traverse till the last node */
        while (last->next != NULL)
            last = last->next;
    
        /* 6. Change the next of last node */
        last->next = new_node;
    
        /* 7. Make last node as previous of new node */
        new_node->prev = last;
    
        return;
    }

    /* Function to delete a node in a Doubly Linked List.
    head_ref --> pointer to head node pointer.
    del --> pointer to node to be deleted. */
    Node* deleteNode(Node* del)
    {
        /* base case */
        if (head == NULL || del == NULL)
            //return;
            //make sure this base case never happens
    
        /* If node to be deleted is head node */
        if (head == del)
            head = del->next;
    
        /* Change next only if node to be
        deleted is NOT the last node */
        if (del->next != NULL)
            del->next->prev = del->prev;
    
        /* Change prev only if node to be
        deleted is NOT the first node */
        if (del->prev != NULL)
            del->prev->next = del->next;
    
        /* Finally, free the memory occupied by del*/
        return del;
    }
 
    // This function prints contents of
    // linked list starting from the given node
    void printList()
    {
        Node* last;
        Node* node = head;
        cout<<"\nTraversal in forward direction \n";
        while (node != NULL)
        {
            cout<<" "<<node->name<<" ";
            last = node;
            node = node->next;
        }
    
        cout<<"\nTraversal in reverse direction \n";
        while (last != NULL)
        {
            cout<<" "<<last->name<<" ";
            last = last->prev;
        }
    }

    bool check_list(string input_name){
        Node* node = head;
        while (node != NULL)
        {
            if (node->name == input_name){
                node->count = 0;
                node->mark = true;
                return true;
            }
            else{
                node = node->next;
            }
        }
        return false;
    }

    void add_counts(){
        Node* node = head;
        while (node != NULL)
        {
            if (node->mark == false){
                node->count += 1;
            }
            else{
                node = node->next;
            }
        }
    }

    void reset_marks(){
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
    Node* head;
    void set_head(Node* new_head){
        head = new_head;
    }
    /* Given a reference (pointer to pointer) to the head
    of a DLL and an int, appends a new node at the end */
    void append(string new_name)
    {
        /* 1. allocate node */
        Node* new_node = new Node();
    
        Node* last = head; /* used in step 5*/
    
        /* 2. put in the data */
        new_node->name = new_name;
    
        /* 3. This new node is going to be the last node, so
            make next of it as NULL*/
        new_node->next = NULL;
    
        /* 4. If the Linked List is empty, then make the new
            node as head */
        if (head == NULL)
        {
            new_node->prev = NULL;
            head = new_node;
            return;
        }
    
        /* 5. Else traverse till the last node */
        while (last->next != NULL)
            last = last->next;
    
        /* 6. Change the next of last node */
        last->next = new_node;
    
        /* 7. Make last node as previous of new node */
        new_node->prev = last;
    
        return;
    }

    /* Function to delete a node in a Doubly Linked List.
    head_ref --> pointer to head node pointer.
    del --> pointer to node to be deleted. */
    void deleteNode(Node* del)
    {
        /* base case */
        if (head == NULL || del == NULL)
            return;
    
        /* If node to be deleted is head node */
        if (head == del)
            head = del->next;
    
        /* Change next only if node to be
        deleted is NOT the last node */
        if (del->next != NULL)
            del->next->prev = del->prev;
    
        /* Change prev only if node to be
        deleted is NOT the first node */
        if (del->prev != NULL)
            del->prev->next = del->next;
    
        /* Finally, free the memory occupied by del*/
        //append(&head_collector, del->data);
        free(del);
        return;
    }
 
    // This function prints contents of
    // linked list starting from the given node
    void printList()
    {
        Node* last;
        Node* node = head;
        cout<<"\nTraversal in forward direction \n";
        while (node != NULL)
        {
            cout<<" "<<node->name<<" ";
            last = node;
            node = node->next;
        }
    
        cout<<"\nTraversal in reverse direction \n";
        while (last != NULL)
        {
            cout<<" "<<last->name<<" ";
            last = last->prev;
        }
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

    /* Start with the empty list */
    Node* head_list = NULL;
    Node* head_collector = NULL;
    Node* tmp_node;
    List list;
    Collector collector;
    list.set_head(head_list);
    collector.set_head(head_collector);
 
    list.append("apple",4,0);
    list.append("yes",8,4);
    list.append("on",4,12);
    list.printList();
    if (list.check_list("apple")== true){
        cout << "present!" << endl;
    }
    else{
        cout << "its false?" << endl;
    }

    tmp_node = list.deleteNode((list.head)->next->next);
    collector.append(tmp_node->name);
    list.printList();
    collector.printList();

    return 0;

    
    cout << "Server is now listening" << endl;
    ofstream myfile;
    myfile.open ("example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
    
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
    
    while(true){
    
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        string s = buffer;
        cout << s << endl;
        if (s != "end"){
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
            cout <<"TODO: "<< name << type_value << value_in_string << memory_value << endl;
            if (list.check_list(name) != true){
                list.append(name,stoi(memory_value),offset);
                list.printList();
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
                string string_send = jsonEnviar;
                cout << string_send << endl;
                ofstream myfile;
                myfile.open ("/home/kenichi/Documents/Github/C-IDE/untitled/example.txt",fstream::app);
                myfile << string_send + "\n";
                myfile.close();
                //char *message = &string_send[0];
                //send(port , message , strlen(message) , 0 );//Se envia la info
            }
        }
        else{            
            list.add_counts(); //add variables count that werent used
            Node* current_node = list.head;
            while (current_node != NULL){
                if (current_node->count >= 3){ //deletes variables with counts higher than 2
                    tmp_node = list.deleteNode(current_node);
                    collector.append(tmp_node->name); //adds the deleted node to collector
                }
                current_node = current_node->next;
            }

            list.reset_marks(); //set all marks to false
        }
    }
    return 0;
}