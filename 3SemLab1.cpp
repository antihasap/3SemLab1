#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;
bool nt(char c) {
    return isupper((unsigned char)c) && isalpha((unsigned char)c);
}
bool t(char c) {
    return !nt(c);
}
int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    ifstream f("grammar.txt");
    if (!f.is_open()) {
        cout << "File not found" << "\n";
        return 1;
    }

    string line;
    int type = 3;
    while (getline(f, line)) {
        if (line.empty()) continue;

        size_t pos;
        while ((pos = line.find("⊥")) != string::npos) {
            line.replace(pos, 2, "#"); // тут меняем символ "⊥"
        }                              // на однобайтовый "#"
        size_t p = line.find("->");
        if (p == string::npos) continue;

        string l = line.substr(0, p);
        string r = line.substr(p + 2);

        l.erase(remove(l.begin(), l.end(), ' '), l.end());
        r.erase(remove(r.begin(), r.end(), ' '), r.end());

        if (type == 3) {
            bool ok = true;
            if (l.size() != 1 || !nt(l[0])) ok = false;
            if (r.size() > 2) ok = false;
            else if (r.size() == 2) {
                char a = r[0], b = r[1];
                if (!((t(a) && nt(b)) || (nt(a) && t(b)))) ok = false;
            }
            else if (r.size() == 1) {
                if (nt(r[0])) ok = false;
            }
            if (!ok) type = 2;
        }
        if (type == 2) {
            if (l.size() != 1 || !nt(l[0])) type = 1;
        }
        if (type == 1) {
            if (r.size() < l.size()) type = 0;
        }
    }
    f.close();

    if (type == 3) cout << "Type 3 (regular)" << "\n";
    else if (type == 2) cout << "Type 2 (context-free)" << "\n";
    else if (type == 1) cout << "Type 1 (context-dependent)" << "\n";
    else cout << "Type 0" << "\n";
    return 0;
}
