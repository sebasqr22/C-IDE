#include<iostream>
#include<fstream>
#include<string.h>
#include "json.hpp"

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
    j2["name"] = "Niels";

    // add another null object by passing nullptr
    j2["nothing"] = nullptr;

    // add an object inside the object
    j2["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j2["list"] = { 1, 0, 2 };

    // add another object (using an initializer list of pairs)
    //j2["object"] = {{"currency", "USD"},{"value", 42.99}};
    fstream o("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    o << j2;
    cout << j2["name"] << endl;
    cout << j2["answer"] << endl;
    return 0;
}