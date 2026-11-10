#include "GraphLoader.h"

#include <fstream>
#include <iostream>
#include <vector>
 
//lil helper func, cuts spaces, tabs, /r, /n from both ends
static string trim(const string& s) {
    const string ws = " \t\r\n";
    size_t a = s.find_first_not_of(ws);
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(ws);
    return s.substr(a, b - a + 1);
}
 
//Splits a line on '|'
static vector<string> splitFields(const string& line) {
    vector<string> out;
    string cur;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == '|') { out.push_back(trim(cur)); cur.clear(); }
        else cur += line[i];
    }
    out.push_back(trim(cur));
    return out;
}
 
//Opens a file, prints an error if it cant. Returns true on success
static bool openFile(ifstream& in, const string& dir, const string& name) {
    in.open((dir + "/" + name).c_str());
    if (!in) {
        cout << "Error: could not open " << name << endl;
        return false;
    }
    return true;
}
 
//prints a rejection warning
static void warn(const string& file, int line, const string& why) {
    cout << "Warning: " << file << " line " << line << ": " << why << endl;
}
 
//loads buildings.txt: one name per line.
void loadBuildings(CampusGraph& g, const string& dir) {
    const string file = "buildings.txt";
    int accepted = 0, rejected = 0, lineNo = 0;
    ifstream in;
    if (openFile(in, dir, file)) {
        string line;
        while (getline(in, line)) {
            lineNo++;
            string t = trim(line);
            if (t.empty() || t[0] == '#') continue;   // blank or comment
 
            string reason;
            if (g.addBuilding(t, reason)) accepted++;
            else { 
                rejected++; 
                warn(file, lineNo, reason);
             }
        }
    }
    cout << file << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}
 
//loads connections.txt: BuildingA|BuildingB per line.
void loadConnections(CampusGraph& g, const string& dir) {
    const string file = "connections.txt";
    int accepted = 0, rejected = 0, lineNo = 0;
    ifstream in;
    if (openFile(in, dir, file)) {
        string line;
        while (getline(in, line)) {
            lineNo++;
            string t = trim(line);
            if (t.empty() || t[0] == '#') continue;
 
            vector<string> f = splitFields(t);
            string reason;
            if (f.size() != 2) {
                rejected++;
                warn(file, lineNo, "expected 2 fields, found " + to_string(f.size()));
            } else if (f[0].empty() || f[1].empty()) {
                rejected++;
                warn(file, lineNo, "empty building name");
            } else if (g.addConnection(f[0], f[1], reason)) {
                accepted++;
            } else {
                rejected++;
                warn(file, lineNo, reason);
            }
        }
    }
    cout << file << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}