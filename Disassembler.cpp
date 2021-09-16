#include <iostream>
#include <unordered_map>
#include <fstream>
#include <bitset>
#include <cmath>
#include <vector>

using namespace std;

unordered_map<string, string> compTable;
unordered_map<string, string> destTable;
unordered_map<string, string> jumpTable;

unordered_map<string, string> createCompTable();
unordered_map<string, string> createDestTable();
unordered_map<string, string> createJumpTable();
string getPrefix(string);
string getDest(string);
string getComp(string);
string getJump(string);
int binaryToDecimal(string);

int main() {
    // read in from cin
    vector<string> lines;
    string line;
    /*
    while (getline(cin, line)) {
        if (line == "stop")
            break;
        lines.push_back(line);
    }*/
    
    compTable = createCompTable();
    destTable = createDestTable();
    jumpTable = createJumpTable();

    while (getline(cin, line)) {
        if (line == "stop")
            break;
        if (line.at(0) == '0') { // a instruction
            int dec = binaryToDecimal(line.substr(1));
            cout << "@" + to_string(dec) << endl;
        }
        else { // c instruction
            string dest = getDest(line);
            string comp = getComp(line);
            string jump = getJump(line);
            
            if (dest == "000" && jump == "000") {// comp
                string str = compTable[comp];
                cout << str << endl;
            }
            else if (jump == "000") {// dest = comp
                string str = destTable[dest] + "=" + compTable[comp];
                cout << str << endl;

            }
            else if (dest == "000") {// comp ; jump
                string str = compTable[comp] + ";" + jumpTable[jump];
                cout << str << endl;
            }
            else { // dest = comp ; jump
                string str = destTable[dest] + "=" + compTable[comp] + ";" + jumpTable[jump];
                cout << str << endl;
            }
        }
    }
    
    return 0;
}

int binaryToDecimal(string binary) {
    int dec = 0;
    int mult = 0;
    for (int i = binary.length() - 1; i >= 0; i--){
        if (binary.at(i) == '1') {
            dec += pow(2,mult);
        }
        mult++;
    }

    return dec;
}

unordered_map<string, string> createCompTable() {
    unordered_map<string, string> compTable;
    
    // computations
    compTable["0101010"] = "0";
    compTable["0111111"] = "1";
    compTable["0111010"] = "-1";
    compTable["0001100"] = "D";
    compTable["0110000"] = "A";
    compTable["0001101"] = "!D";
    compTable["0110001"] = "!A";
    compTable["0001111"] = "-D";
    compTable["0110011"] = "-A";
    compTable["0011111"] = "D+1";
    compTable["0110111"] = "A+1";
    compTable["0001110"] = "D-1";
    compTable["0110010"] = "A-1";
    compTable["0000010"] = "D+A";
    compTable["0010011"] = "D-A";
    compTable["0000111"] = "A-D";
    compTable["0000000"] = "D&A";
    compTable["0010101"] = "D|A";
    compTable["1110000"] = "M";
    compTable["1110001"] = "!M";
    compTable["1110011"] = "-M";
    compTable["1110111"] = "M+1";
    compTable["1110010"] = "M-1";
    compTable["1000010"] = "D+M";
    compTable["1010011"] = "D-M";
    compTable["1000111"] = "M-D";
    compTable["1000000"] = "D&M";
    compTable["1010101"] = "D|M";

    return compTable;
}


unordered_map<string, string> createJumpTable() {
    unordered_map<string, string> jumpTable;

    // jumps
    jumpTable["001"] = "JGT";
    jumpTable["010"] = "JEQ";
    jumpTable["011"] = "JGE";
    jumpTable["100"] = "JLT";
    jumpTable["101"] = "JNE";
    jumpTable["110"] = "JLE";
    jumpTable["111"] = "JMP";

    return jumpTable;
}


unordered_map<string, string> createDestTable() {
    unordered_map<string, string> destTable;

    // destinations
    destTable["001"] = "M";
    destTable["010"] = "D";
    destTable["011"] = "MD";
    destTable["100"] = "A";
    destTable["101"] = "AM";
    destTable["110"] = "AD";
    destTable["111"] = "AMD";

    return destTable;
}

string getDest(string line) {
    string str = line.substr(10,3);
    return str;
}

string getComp(string line) {
    string str = line.substr(3,7);
    return str;
}

string getJump(string line) {
    string str = line.substr(13,3);
    return str;
}