#include <bits/stdc++.h>
using namespace std;

struct Symbol {
    string name, section, value, type;
    int location, size;
};

const set<string> REGISTERS = {
    "EAX", "EBX", "ECX", "EDX",
    "ESI", "EDI", "ESP", "EBP"
};

string upper(string s) {
    for (char &c : s)
        c = toupper(static_cast<unsigned char>(c));
    return s;
}

string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

bool isRegister(string s) {
    return REGISTERS.count(upper(trim(s))) != 0;
}

bool isNumber(const string &s) {
    string x = trim(s);
    if (x.empty()) return false;
    try {
        size_t pos = 0;
        stoll(x, &pos, 0);
        return pos == x.size();
    } catch (...) {
        return false;
    }
}

bool isIdentifier(const string &s) {
    return regex_match(s, regex("[A-Za-z_][A-Za-z0-9_]*"));
}

bool isMemory(const string &s) {
    string x = trim(s);
    if (x.size() < 3 || x.front() != '[' || x.back() != ']')
        return false;

    x = upper(x.substr(1, x.size() - 2));
    // Recognize register-based x86 addressing such as [EAX] and
    // [EBX+ESI*4]. Reject unknown tokens and unsupported syntax.
    for (char &c : x) {
        if (c == '+' || c == '*' || c == '-') c = ' ';
    }

    stringstream ss(x);
    string token;
    while (ss >> token) {
        if (isRegister(token)) continue;
        if (token == "1" || token == "2" ||
            token == "4" || token == "8") continue;
        return false;
    }
    return true;
}

bool validInstruction(const string &line) {
    string op, operands;
    stringstream ss(trim(line));
    ss >> op;
    op = upper(op);
    getline(ss, operands);
    operands = trim(operands);

    size_t comma = operands.find(',');
    string a = comma == string::npos
        ? operands : trim(operands.substr(0, comma));
    string b = comma == string::npos
        ? "" : trim(operands.substr(comma + 1));

    if (op == "INC" || op == "DEC" ||
        op == "PUSH" || op == "POP") {
        if (a.empty() || comma != string::npos) return false;
        // Registers are unambiguous. Memory operands need an
        // explicit size in this simplified checker.
        return isRegister(a);
    }

    if (op == "MOV" || op == "ADD" || op == "SUB") {
        if (comma == string::npos || a.empty() || b.empty() ||
            b.find(',') != string::npos) return false;

        if (op == "MOV") {
