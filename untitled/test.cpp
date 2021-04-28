#include<iostream>
#include<fstream>
#include<string.h>
#include "json.hpp"
#include<sstream>

using namespace std;
using json = nlohmann::json;

//"/home/kenichi/Documents/Github/C-IDE/untitled/variables.json"

int main()
{
    /*
    ifstream i("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    json j;
    i >> j;
    string s = j.dump();
    cout << s <<endl;
    */
    
    /*json j2 = {
        {"pi", 3.141},
        {"happy", "true"},
        {"name", "Kenichi"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
            {"pls no 1", "USD"},
            {"value", 318}
        }}
        };*/

    json j2;
    j2["pi"] = 3.141;

    // add a Boolean that is stored as bool
    j2["happy"] = true;

    // add a string that is stored as std::string
    j2["type"] = "Niels";

    // add another null object by passing nullptr
    j2["nothing"] = nullptr;

    // add an object inside the object
    j2["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j2["list"] = { 1, 0, 2 };

    // add another object (using an initializer list of pairs)
    //j2["object"] = {{"currency", "USD"},{"value", 42.99}};
    //fstream o("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    //o << j2;
    json j;
    j["thunder"] = {{"type", "int"}, {"value","30"}, {"memory", 4}};
    j["fire"] = {{"type", "long"}, {"value","300000"}, {"memory", 8}};
    j["water"] = {{"type", "char"}, {"value","w"}, {"memory", 1}};
    j["leaf"] = {{"type", "float"}, {"value","30.9"}, {"memory", 4}};
    j["vine"] = {{"type", "float"}, {"value","60.18"}, {"memory", 4}};
    j["ice"] = {{"type", "double"}, {"value","4000"}, {"memory", 8}};
    string s = j.dump();
    
    int i = 0;
    int offset = 0;
    string name;
    char *ptr;
    ptr = (char*) malloc(8000);
    stringstream ss;
    int numerical_int_value;
    long numerical_long_value;
    float numerical_float_value;
    double numerical_double_value;
    
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
                if (offset > 8000){
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

    return 0;
}
