#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;
bool nt(char c) {
    return isupper(c) && isalpha(c);
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
            line.replace(pos, 2, "#"); // тут меняем символ '⊥'
        }                              // на однобайтовый '#'
        size_t p = line.find("->");
        if (p == string::npos) continue;

        string l = line.substr(0, p);
        string r = line.substr(p + 2);

        l.erase(remove(l.begin(), l.end(), ' '), l.end());
        r.erase(remove(r.begin(), r.end(), ' '), r.end());

        if (l.size() != 1 || !nt(l[0])) {
            if (type > 2) type = 2;
        }

        if (r.size() == 2) {
            char a = r[0];
            char b = r[1];
            if (!((t(a) && nt(b)) || (nt(a) && t(b)))) {
                if (type > 2) type = 2;
            }
        }
        else if (r.size() < l.size()) {
            if (type > 1) type = 1;
        }
    }
    f.close();

    if (type == 3) cout << "Type 3 (regular)" << "\n";
    else if (type == 2) cout << "Type 2 (context-free)" << "\n";
    else if (type == 1) cout << "Type 1 (context-dependent)" << "\n";
    else cout << "Type 0" << "\n";
    return 0;
}
