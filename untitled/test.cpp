#include<iostream>
#include<fstream>
#include<string.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//"/home/kenichi/Documents/Github/C-IDE/untitled/variables.json"

int main()
{
    ifstream i("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    json j;
    i >> j;
    string s = j.dump();
    cout << s <<endl;
    
    json j2 = {
        {"pi", 3.141},
        {"happy", "fucked"},
        {"name", "Jesus"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
            {"pls no 1", "USD"},
            {"value", 69.69}
        }}
        };
    fstream o("/home/kenichi/Documents/Github/C-IDE/untitled/variables.json");
    o << j2;
    return 0;
}