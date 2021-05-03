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
            {//return;
            }
            //make sure this base case never happens
    
        /* If node to be deleted is head node */
        if (head == del)
            {head = del->next;
            cout << "enters head case"<< endl;}
    
        /* Change next only if node to be
        deleted is NOT the last node */
        if (del->next != NULL)
            {del->next->prev = del->prev;}
    
        /* Change prev only if node to be
        deleted is NOT the first node */
        if (del->prev != NULL)
            {del->prev->next = del->next;}
    
        /* Finally, free the memory occupied by del*/
        return del;
    }
 
    // This function prints contents of
    // linked list starting from the given node
    void printList()
    {
        Node* node = head;
        cout<<"\nTraversal in forward direction \n";
        while (node != NULL)
        {
            cout<<" "<<node->name<<" ";
            node = node->next;
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
            node = node->next;
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
    int length = 0;
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
        length += 1;
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
        {
            //cout enters the head case
            head = del->next;
            }
    
        /* Change next only if node to be
        deleted is NOT the last node */
        if (del->next != NULL){
            del->next->prev = del->prev;}
    
        /* Change prev only if node to be
        deleted is NOT the first node */
        if (del->prev != NULL){
            del->prev->next = del->next;}
    
        length -= 1;
        /* Finally, free the memory occupied by del*/
        return del;
    }
 
    // This function prints contents of
    // linked list starting from the given node
    void printList()
    {
        Node* node = head;
        cout<<"\nTraversal in forward direction \n";
        while (node != NULL)
        {
            cout<<" "<<node->name<<" ";
            node = node->next;
        }
    }
    bool verify_recycled_one(){
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

    bool verify_recycled_four(){
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

    bool verify_recycled_eight(){
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
    Node* del_node = list.head;

    tmp_node = list.deleteNode((del_node));
    collector.append(tmp_node->name,tmp_node->memory_value,tmp_node->offset);
    list.printList();
    collector.printList();

    return 0;
}