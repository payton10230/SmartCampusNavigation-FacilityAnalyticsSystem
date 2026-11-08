#include <CampusGraph.h>

//lil helper func, cuts spaces, tabs, /r, /n from both ends
static string trimStr(const string& s){
    const string ws = " \t\r\n"//whitespaces needed gone
    size_t start = s.find_first_not_of(ws); //searches a string for the first character that does not match any of the characters in a specified set.
    if (start == string::npos) return ""; //npos is just not found
    size_t end = s.find_last_not_of(ws); // same thingg other side
    return s.substr(start, end - start + 1); //fuses with both ends cut & ready
}

//creates an empty graph
CampusGraph::CampusGraph() : edgeCount(0) {}

bool CampusGraph::equalsIgnore(string& a, string b){
    if(a.size() != b.size()) return false;
    for(size_t i = 0; i < a.size(); i++){ //size_t is just a really big num to handle whatevers entered
        char x = a[i], y = b[i];
        if(x >= 'A' && x <= 'Z') x = x - 'A' + 'a';
        if(y >= 'A' && y <= 'Z') y = y - 'A' + 'a';
        if(x != y) return false;
    }
    return true;
}

//adds building, returns false and sets reason if dupe or empty name
bool CampusGraph::addBuilding(string& name, string& reason){
    string clean = trimStr(name);
    if(clean.empty()){
        reason = "empty building name";
        return false
    }
    if(findBuilding(clean) != -1){
        reason = "dupe building '" + clean+"'";
        return false;
    }
    names.push_back(clean);
    adj.push_back(vector<int>());
    return true;
}
//adds connection, returns false and sets reason if building is unknown or duped
bool CampusGraph::addConnection(string& a, string& b, string& reason){
    int ia = findBuilding(a);
    int ib = findBuilding(b);

    if(ia == -1 || ib ==-1){
        reason = "unknown Building";
        return false;
    }
    if(ia == ib) {
        reason = "self connection on '" + names[ia] + "'";
        return false;
    }

    for(size_t i = 0; i < adj[ia].size(); i++){
        if(adj[ia][i] == ib) {
            reason = "dupe connection";
            return false;
        }
    }
    adj[ia].push_back(ib);
    adj[ib].push_back(ia);
    edgeCount++;
    return true;
}
//returns index of building, -1 if not found
int CampusGraph::findBuilding(string& name){
    string key = trimStr(name);
    for(size_t i = 0; i < names.size(); i++) {
        if(equalsIgnore(names[i], key)) return (int)i;
    }
    return -1;
}
//returns name at index
string& CampusGraph::getBuildingName(int index){
    return  names[index];
}
//returns neighbors of index, insertion order
vector<int>& CampusGraph::getNeighbors(int index){
    return adj[index];
}
//returns tot buildings
int CampusGraph::getBuildingCount(){return (int)names.size(); }
//returns tot connections
int CampusGraph::getConnectionCount() const {return edgeCount; }
//returns degree
int CampusGraph::getDegree(int index) const { return (int)adj[index].size(); }
//returns false if graph is empty
bool CampusGraph::isEmpty() const { return names.empty(); }

// Prints totals and the degree of every building.
void CampusGraph::displayCountsAndDegrees() const {
    if (names.empty()) {
        cout << "No buildings are loaded." << endl;
        cout << "Total buildings: 0" << endl;
        cout << "Total connections: 0" << endl;
        return;
    }
    cout << "Total buildings: " << names.size() << endl;
    cout << "Total connections: " << edgeCount << endl;
    for (size_t i = 0; i < names.size(); i++) {
        cout << "  " << names[i] << " (degree " << adj[i].size() << ")" << endl;
    }
}
 
// BFS from startName; prints visit order with distances and the reachable count.
void CampusGraph::bfsReachable(const string& startName) const {
    int start = findBuilding(startName);
    if (start == -1) {
        cout << "Error: unknown building '" << trimStr(startName) << "'." << endl;
        return;
    }
 
    vector<bool> visited(names.size(), false);
    vector<int> dist(names.size(), 0);
    queue<int> q;
    int reachable = 0;
 
    visited[start] = true;   // mark at enqueue time
    q.push(start);
 
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
 
        cout << names[cur] << " (distance " << dist[cur] << ")" << endl;
        reachable++;
 
        for (size_t i = 0; i < adj[cur].size(); i++) {
            int nb = adj[cur][i];
            if (!visited[nb]) {
                visited[nb] = true;
                dist[nb] = dist[cur] + 1;
                q.push(nb);
            }
        }
    }
 
    cout << "Reachable: " << reachable << " of " << names.size() << " buildings" << endl;
}

void CampusGraph::displayGraph() const {
    if (names.empty()) {
        cout << "No buildings are loaded." << endl;
        return;
    }
    for (size_t i = 0; i < names.size(); i++) {
        cout << names[i] << ": ";
        if (adj[i].empty()) {
            cout << "(no connections)";
        } else {
            for (size_t j = 0; j < adj[i].size(); j++) {
                if (j > 0) cout << ", ";
                cout << names[adj[i][j]];
            }
        }
        cout << endl;
    }
}