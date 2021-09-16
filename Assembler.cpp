// Cesar Garci 428005133

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <bitset>
#include <cmath>
#include <vector>

using namespace std;

//global variables
unordered_map<string, int> symbolTable;
unordered_map<string, int> compTable;
unordered_map<string, int> destTable;
unordered_map<string, int> jumpTable;

string aInstruction(string&);
string cInstruction(string);
bool isValidCInstruction(string);

unordered_map<string, int> createSymbolTable(vector<string>);
unordered_map<string, int> createCompTable();
unordered_map<string, int> createDestTable();
unordered_map<string, int> createJumpTable();
string removeCommentsAndWhitespaces(string);
bool isValidAInstruction(string);


int main() {
    // symbol table
    string assemblyFile = "test.asm";
    //cout << "Please enter the name of the assembly file : ";
    string line;
    vector<string> lines;
    while (getline(cin, line)) {
        if (line == "stop")
            break;
        if (line.empty())
            continue;
        lines.push_back(line);
    }

    // 1st step: create symbol table
    symbolTable = createSymbolTable(lines);

    // Step 2: Make instruction lookup table
    compTable = createCompTable();
    destTable = createDestTable();
    jumpTable = createJumpTable();
    
    // Step 3: Parse each line, tokenize, and translate to binary
    //ifstream file(assemblyFile);
    //ofstream fileOUT(assemblyFile.substr(0, assemblyFile.length()-4) + ".hack");
    

    int index = 0;
    while (index != lines.size()) {
        line = lines.at(index);
        string binary;
        line = removeCommentsAndWhitespaces(line);
        index++;
        
        if (isspace(line.at(0)))
            continue;
        
        if (isValidAInstruction(line)) {
            binary = aInstruction(line);
            cout << binary << endl;
            continue;
        }
        
        if (isValidCInstruction(line)) {
            binary = cInstruction(line);
            cout << binary << endl;
            continue;
        }
        
    }

    //fileOUT.close();
    //file.close();

    return 0;
}

bool validSymbolChars(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str.at(i) >= 65 && str.at(i) <= 90) // uppercase letter
            continue;
        else if (str.at(i) >= 97 && str.at(i) <= 122)
            continue;
        else if (str.at(i) >= 48 || str.at(i) <= 57) // digit
            continue;
        else if (str.at(i) == 95) // underscore
            continue;
        else if (str.at(i) == 46) // dot
            continue;
        else if (str.at(i) == 36) // $
            continue;
        else if (str.at(i) == 58) // :
            continue;
        else
            return false;
    }
    return true;
}

string decimalTo15BitBinary(string str) {
    int decimal = stoi(str); 
    string binary = "";
    while (decimal != 0) {
        binary.insert(0,to_string(decimal % 2));
        decimal /= 2;
    }
    if (binary.length() != 15) {
        for (int i = binary.length(); i < 15; i++) {
            binary.insert(0, to_string(0));
        }
    }

    return binary;
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


string removeCommentsAndWhitespaces(string line) {
    string str = "";
    
    for (int i = 0; i < line.length()-1; i++) {
        if (line.at(i) == ' ') {
            continue;
        }
        if (line.at(i) == '/') {
            if (i != line.length() - 1)
                if (line.at(i+1 == '/')) 
                    break;
        }
        str += line.at(i);
        
    }
    return line;
}


bool isLabel (string str) {
    if (str.at(0) == '(' && str.at(str.length()-1) == ')')
        return true;
    return false;
}

bool isPostitiveNumber(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (!isdigit(str.at(i))) {
            return false;
        }
    }
    return true;
}

bool isValidAInstruction(string line) {
    // @....
    if (line.at(0) != '@') {
        return false;
    }

    string restOfLine = line.substr(1);
    
    if (isPostitiveNumber(restOfLine))
        return true;

    if (isdigit(restOfLine.at(0)))
        return false;

    //check to make sure symbols are valid
    if (validSymbolChars(restOfLine))
        return true;
        
    return false;
}

vector<string> split(string str, vector<string> delimiters) {
    
    vector<string> tokens;
    if (str.find(delimiters[0]) == -1 && str.find(delimiters[1]) == -1) { // for single comp
        tokens.push_back(str);
        return tokens;
    }
    for (int i = 0; i < delimiters.size(); i++) {
        int index = str.find(delimiters[i]);
        
        if (index >= 0) {
            tokens.push_back(str.substr(0,index));
            str = str.substr(index+1);
        }
    }
    tokens.push_back(str);
    return tokens;
}

int countChars(string str, char c) {
    int count = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str.at(i) == c)
            count++;
    }
    return count;
}

unordered_map<string, int> createCompTable() {
    unordered_map<string, int> compTable;
    
    // computations
    compTable["0"] = binaryToDecimal("0101010");
    compTable["1"] = binaryToDecimal("0111111");
    compTable["-1"] = binaryToDecimal("0111010");
    compTable["D"] = binaryToDecimal("0001100");
    compTable["A"] = binaryToDecimal("0110000");
    compTable["!D"] = binaryToDecimal("0001101");
    compTable["-A"] = binaryToDecimal("0110011");
    compTable["D+1"] = binaryToDecimal("0011111");
    compTable["A+1"] = binaryToDecimal("0110111");
    compTable["D-1"] = binaryToDecimal("0001110");
    compTable["A-1"] = binaryToDecimal("0110010");
    compTable["D+A"] = binaryToDecimal("0000010");
    compTable["D-A"] = binaryToDecimal("0010011");
    compTable["A-D"] = binaryToDecimal("0000111");
    compTable["D&A"] = binaryToDecimal("0000000");
    compTable["D|A"] = binaryToDecimal("0010101");
    compTable["M"] = binaryToDecimal("1110000");
    compTable["!M"] = binaryToDecimal("1110001");
    compTable["-M"] = binaryToDecimal("1110011");
    compTable["M+1"] = binaryToDecimal("1110111");
    compTable["M-1"] = binaryToDecimal("1110010");
    compTable["D+M"] = binaryToDecimal("1000010");
    compTable["D-M"] = binaryToDecimal("1010011");
    compTable["M-D"] = binaryToDecimal("1000111");
    compTable["D&M"] = binaryToDecimal("1000000");
    compTable["D|M"] = binaryToDecimal("1010101");

    return compTable;
}

unordered_map<string, int> createJumpTable() {
    unordered_map<string, int> jumpTable;

    // jumps
    jumpTable["JGT"] = binaryToDecimal("001");
    jumpTable["JEQ"] = binaryToDecimal("010");
    jumpTable["JGE"] = binaryToDecimal("011");
    jumpTable["JLT"] = binaryToDecimal("100");
    jumpTable["JNE"] = binaryToDecimal("101");
    jumpTable["JLE"] = binaryToDecimal("110");
    jumpTable["JMP"] = binaryToDecimal("111");

    return jumpTable;
}

unordered_map<string, int> createDestTable() {
    unordered_map<string, int> destTable;

    // destinations
    destTable["M"] = binaryToDecimal("001");
    destTable["D"] = binaryToDecimal("010");
    destTable["MD"] = binaryToDecimal("011");
    destTable["A"] = binaryToDecimal("100");
    destTable["AM"] = binaryToDecimal("101");
    destTable["AD"] = binaryToDecimal("110");

    return destTable;
}

unordered_map<string, int> createSymbolTable(vector<string> lines) {
    // populate symbol table with predefined values
    unordered_map<string, int> symbolTable;

    symbolTable["SP"] = 0;
    symbolTable["LCL"] = 1;
    symbolTable["ARG"] = 2;
    symbolTable["THIS"] = 3;
    symbolTable["THAT"] = 4;

    for (int i = 0; i < 16; i++) {
        symbolTable["R" + to_string(i)] =i;
    }

    symbolTable["SCREEN"] = 16384;
    symbolTable["KBD"] = 24576;
    
    // populate symbol table with labels
    //ifstream file(assemblyFile);
    int pc = 0; // refers to location in the file

    string line;
    int index = 0;
    while (index != lines.size()) {

        line = lines.at(index);
        string cleanLine = removeCommentsAndWhitespaces(line);
        //cout << cleanLine << endl;
        if (isLabel(cleanLine)) {

            string label = cleanLine.substr(1,cleanLine.length()-2);
            
            if (symbolTable.count(label) == 0) {
                symbolTable[label] = pc;
            }
        }
        if (!isLabel(cleanLine))
            pc++;
        index++;
    }

    //file.close();
    
    // populate symbol table with variables
    //file.open(assemblyFile);
    int nextAddress = 16;
    index = 0;
    while (index != lines.size()) {
        line = lines.at(index);
        string cleanLine = removeCommentsAndWhitespaces(line);
        
        if (isValidAInstruction(cleanLine)) {
            string AInstructionVal = cleanLine.substr(1);

            if (!isPostitiveNumber(AInstructionVal) && symbolTable.count(AInstructionVal) == 0) {
                symbolTable[AInstructionVal] = nextAddress;
                nextAddress++;
            }
        }
        index++;
    }

    //file.close();
    
    return symbolTable;
}

string aInstruction(string& line) {
    string newLine = line.substr(1);
    string binary;

    if (newLine.at(0) != '-' && isPostitiveNumber(newLine)) {
        binary = decimalTo15BitBinary(newLine);
    }

    if (!isPostitiveNumber(newLine)) {
        binary = bitset<15>(symbolTable[newLine]).to_string();
        
    }
    return ('0' + binary);
}

string cInstruction(string line) {
    vector<string> delimiters = {"=", ";"};
    vector<string> tokens = split(line, delimiters);
    string prefix = "111";
    string dest;
    string comp;
    string jump;
    if (tokens.size() == 1) { // only comp, NOTE: only implemented because i was asked to
        comp = bitset<7>(compTable.at(tokens[0])).to_string();
        dest = "000";
        jump = "000";
    }
    else if (tokens.size() == 2) {
        if (countChars(line, '=') == 1) { // dest and comp
            dest = bitset<3>(destTable.at(tokens[0])).to_string(); // throws exception if not found
            comp = bitset<7>(compTable.at(tokens[1])).to_string();
            jump = "000";
        }
        else { // comp and jump
            dest = "000";
            comp = compTable.at(tokens[0]);
            jump = compTable.at(tokens[1]);
        }
    }
    else { // all three are present
        dest = destTable.at(tokens[0]);
        comp = compTable.at(tokens[1]);
        jump = jumpTable.at(tokens[2]);
    }
    
    return prefix + comp + dest + jump;
}

bool isValidCInstruction(string line) {
    
    if (!(countChars(line, '=') == 1 || countChars(line, ';') == 1 || compTable.count(line) >= 1))
        return false;
    
    vector<string> delimiters = {"=", ";"};
    vector<string> tokens = split(line, delimiters);

    if (tokens.size() != 1 && tokens.size() != 2 && tokens.size() != 3) {
        return false;
    }

    if (tokens.size() == 1) { // comp NOTE: only implemented because i was asked to
        if (!compTable.count(tokens[0]) > 0)
            return false;
    }
    else if (tokens.size() == 2) { // two tokens
        if (countChars(line, '=') == 1) { // dest and comp
            if (!destTable.count(tokens[0]) > 0)
                return false;
            if (!compTable.count(tokens[1]) > 0)
                return false;
        }
        else { // comp and jump
            if (!compTable.count(tokens[0]) > 0)
                return false;
            if (!jumpTable.count(tokens[1]) > 0)
                return false;
        }
    }
    else { // three tokens AKA // dest comp and jump
        if (!destTable.count(tokens[0]) > 0)
            return false;
        if (!compTable.count(tokens[1]) > 0)
            return false;
        if (!jumpTable.count(tokens[2]) > 0)
            return false;
    }

    return true;
}
