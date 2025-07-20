#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include <set>
using namespace std;

int pc; //program counter
int mc; //memory counter
string a_out;
string comp_out;
string dest_out;
string jmp_out;
string myLine;
string compBits;
string destBits;
string jmpBits;
map<string,int> jmp_regs;
map<string, int> vars {{"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4}, {"SCREEN", 16384}, {"KBD", 24576}};

/*
Rules:
Parse line
If you see '//' or newline character, skip to next line
Increment pc, repeat
*/

/*
First pass - find all the loop variables
Scan through lines until you find a left parenthesis '('
Get the string from inside and store it as a pair with the value of pc
Continue to next line
*/

/*
Second pass - translate the assembly code into machine code whilst generating symbol table
If you see a '@', get the string and compare it to the existing symbol table
If variable does not exist, create a pair with string and value of mc
Increment mc and continue
*/

int main () {

    ofstream MyWriteFile("rect.txt"); //Output file

    ifstream MyReadFile("Rect.asm");
    pc = 0;
    mc = 16;

    while (getline(MyReadFile, myLine)) {
        myLine.erase(remove_if(myLine.begin(), myLine.end(), isspace), myLine.end());
        if (myLine[0] == '/' && myLine[1] == '/') {
            continue;
        }

        if (myLine.empty()) {
            continue;
        }

        //First pass, saving jump locations
        if (myLine[0] == '(' && myLine.back() == ')')
        {
            //Save text inside parentheses to dict
            string x = myLine.substr(1, myLine.length()-2); //Starting from index 1, grab substring
            jmp_regs.insert({x, pc}); 
            continue;
        }

        pc++;
    }
    //Clear flags
    MyReadFile.clear();
    MyReadFile.seekg(0);
    pc = 0;


    while (getline(MyReadFile, myLine)) {
        //Second pass
        myLine.erase(remove_if(myLine.begin(), myLine.end(), isspace), myLine.end());

        if (myLine[0] == '@') {
            //Convert address to memory location
            
            string a = myLine.substr(1);
            if (a[0] == 'R') {
                try {
                    if (stoi(a.substr(1)) < 16) {
                        MyWriteFile << "0" + bitset<15>(stoi(a.substr(1))).to_string() + "\n";

                        continue;
                    }
                 }
                
                catch (const std::invalid_argument& ia) {

                }
            }
            
            try {
                MyWriteFile << "0" + bitset<15>(stoi(a)).to_string() + "\n";
            }
            catch(const std::invalid_argument& ia) {
                if (jmp_regs.count(a)) {
                    MyWriteFile << "0" + bitset<15>(jmp_regs[a]).to_string() + "\n";
                }
                else {
                    if(vars.count(a)) {
                        MyWriteFile << "0" + bitset<15>(vars[a]).to_string() + "\n";
                    }
                    else {
                        vars.insert({a, mc});
                        MyWriteFile << "0" + bitset<15>(vars[a]).to_string() + "\n";
                        mc++;
                    }
                }
                pc++;
            }
            }
            
        
        else if (myLine[0] == '(') {
            string x = myLine.substr(1, myLine.length()-2); //Starting from index 1, grab substring
        }

        else if (myLine[0] == '/' && myLine[1] == '/') {
            //Do nothing
        }
        else if (!myLine.empty()) {
            //Get dest - substring before equality
            if (myLine.find('=') != std::string::npos) {
                destBits = myLine.substr(0, myLine.find('='));
            }
            else {
                destBits = "null";
            }
            
            //Get comp - substring after equality, before semicolon
            compBits = myLine.substr(myLine.find('=')+1, myLine.find(';')-myLine.find('=')-1);
            //Get jmp - optional substring after semicolon
            if (myLine.find(';') != std::string::npos) {
            jmpBits = myLine.substr(myLine.find(';')+1);
            }
            else {
                jmpBits = "null";
            }
        
            string a_out = "null";
            set<string> example{"M", "!M", "-M", "M+1", "M-1", "D+M", "D-M", "M-D", "D&M", "D|M"};
            if ( example.count(compBits)) 
            { 
                a_out = "1";
            }
            else {
                a_out = "0";
            }

            if (compBits == "0") {
                comp_out = "101010";
            }
            else if (compBits == "1") {
                comp_out = "111111";
            }
            else if (compBits == "-1") {
                comp_out = "111010";
            }
            else if (compBits == "D") {
                comp_out = "001100";
            }
            else if (compBits == "A" || compBits == "M") {
                comp_out = "110000";
            }
            else if (compBits == "!D") {
                comp_out = "001101";
            }
            else if (compBits == "!A" || compBits == "!M") {
                comp_out = "110001";
            }
            else if (compBits == "-D") {
                comp_out = "001111";
            }
            else if (compBits == "-A" || compBits == "-M") {
                comp_out = "110011";
            }
            else if (compBits == "!A" || compBits == "!M") {
                comp_out = "110001";
            }
            else if (compBits == "D+1") {
                comp_out = "011111";
            }
            else if (compBits == "A+1" || compBits == "M+1") {
                comp_out = "110111";
            }
            else if (compBits == "D-1") {
                comp_out = "001110";
            }
            else if (compBits == "A-1" || compBits == "M-1") {
                comp_out = "110010";
            }
            else if (compBits == "D+A" || compBits == "D+M") {
                comp_out = "000010";
            }
            else if (compBits == "D-A" || compBits == "D-M") {
                comp_out = "010011";
            }
            else if (compBits == "A-D" || compBits == "M-D") {
                comp_out = "000111";
            }
            else if (compBits == "D&A" || compBits == "D&M") {
                comp_out = "000000";
            }
            else if (compBits == "D|A" || compBits == "D|M") {
                comp_out = "010101";
            }

            if (destBits == "null") {
                dest_out = "000";
            }
            else if (destBits == "M") {
                dest_out = "001";
            }
            else if (destBits == "D") {
                dest_out = "010";
            }
            else if (destBits == "MD") {
                dest_out = "011";
            }
            else if (destBits == "A") {
                dest_out = "100";
            }
            else if (destBits == "AM") {
                dest_out = "101";
            }
            else if (destBits == "AD") {
                dest_out = "110";
            }
            else if (destBits == "AMD") {
                dest_out = "111";
            }

            if (jmpBits == "null") {
                jmp_out = "000";
            }
            else if (jmpBits == "JGT") {
                jmp_out = "001";
            }
            else if (jmpBits == "JEQ") {
                jmp_out = "010";
            }
            else if (jmpBits == "JGE") {
                jmp_out = "011";
            }
            else if (jmpBits == "JLT") {
                jmp_out = "100";
            }
            else if (jmpBits == "JNE") {
                jmp_out = "101";
            }
            else if (jmpBits == "JLE") {
                jmp_out = "110";
            }
            else if (jmpBits == "JMP") {
                jmp_out = "111";
            }
            MyWriteFile << "111" + a_out + comp_out + dest_out + jmp_out + "\n";
            pc++;
    }
    }

    MyWriteFile.close();

    //Write to file using MyFile << ""
    MyReadFile.close();

    return 0;
}




