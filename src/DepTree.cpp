/*
 * DepTree.cpp
 * use: Tree and Tree functions implementation
 * Author: Payton Dencklau
 */
#include "DepTree.h"
#include <cctype>
#include <algorithm>
#include <string>

//For Case-Sensitive Strings to put them to lowercase
static string toLC(const string& s) {
    string str = s;
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return str;
}
//Trim function for loading from File
static string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if(first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

//For recursion during findDep
DNode* Tree::findHelper(DNode* cur, const string& n) const {
    if (cur==nullptr) {
        return nullptr;
    }
    if (toLC(cur->name)==toLC(n)) {
        return cur;
    }
    for (size_t i = 0; i<cur->children.size(); i++) {
        DNode* child = findHelper(cur->children[i], n);
        if (child!=nullptr) {
            return child;
        }
    }
    return nullptr;
}

//For PrintTree Indention
void Tree::printHelper(const DNode* cur, int depth) const {
    if (cur==nullptr) return;
    string ind(depth * 4, ' ');
    cout<<ind<<cur->name<<endl;
    for (size_t i = 0; i<cur->children.size(); i++) {
        printHelper(cur->children[i], depth+1);
    }
}

//For PrintPreorder Sequence
void Tree::preorderHelper(const DNode* cur, bool& first) const {
    if (cur==nullptr) return;
    if (!first) cout<< ", ";
    cout<<cur->name;
    first = false;
    for (size_t i = 0; i<cur->children.size(); i++) {    
        preorderHelper(cur->children[i], first);
    }
}
//Destructor for DNode*
void Tree::desHelper(DNode* cur) {
    if (cur==nullptr) return;
    for (size_t i = 0; i<cur->children.size(); i++) {    
        desHelper(cur->children[i]);
    }
    delete cur;
}


//Constructor to initizalize root and numDep
Tree::Tree() {
    root = nullptr;
    numDep = 0;
}

//Destructor
Tree::~Tree() {
    desHelper(root);
}

//Loading from departments.txt
bool Tree::loadFromFile(const string& filepath, int& accepted, int& rejected) {
    accepted=0,rejected=0;
    string reason;
    ifstream file(filepath);
    if (!file.is_open()) {
        cout<<"Error: Couldn't find file "<<filepath<<". File will be treated as empty.\n";
        return false; 
    }
    int lineNum=0;
    string line;
    while (getline(file,line)) {
        lineNum++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        
        int x = count(line.begin(), line.end(), '|');
        if (x==1) {

            size_t pos = line.find('|');
            string dnode = line.substr(0,pos);
            string pnode = line.substr(pos+1);

            dnode = trim(dnode);
            pnode = trim(pnode);

            if (dnode.empty() || pnode.empty()) {
                cout<<"Warning [departments.txt: "<<lineNum<<"]: One of the inputs is empty.\n";
                rejected++;
                continue;
            }

            if (!addDep(dnode, pnode, reason)) {
                cout<<"Warning [departments.txt: "<<lineNum<<"]: "<<reason<<".\n";
                rejected++;
            } else {accepted++;}
        } else { 
            cout<<"Warning [departments.txt: "<<lineNum<<"]: There is an invalid number of seperators.\n";
            rejected++;
        }



    }
    file.close();
    return true;
}

//Add Department with name n and parent p
bool Tree::addDep(const string& n, const string& p, string& reason) {
    if (n.empty()) {
        reason="blank name";
        return false;
    }
    if (nameExists(n)) {
        reason="name exists";
        return false;
    }
    if (isEmpty()) {
        string pn = toLC(p);
        if (pn!="none") {
            reason="parent isn't NONE";
            return false;
        } else {
            DNode* d = new DNode;
            d->name = n;
            root = d;

            numDep++;
            return true;
        }
    } else {
        DNode* par = findDep(p);
        if (par!=nullptr) {
            DNode* d = new DNode;
            d->name = n;
            d->parent = par;
            par->children.push_back(d);
            numDep++;
            return true;
        } else {
            reason="can't find parent";
            return false;
        }
    }
}

//Find Department with name n
DNode* Tree::findDep(const string& n) const {
    return findHelper(root, n);
}

//Check if Name already exists with name n
bool Tree::nameExists(const string& n) const {
    return findDep(n) != nullptr;
}

//Check if tree is empty
bool Tree::isEmpty() const {
    return root == nullptr;
}

//Prints Tree (Regular)
void Tree::PrintTree() const {
    if(!isEmpty()) {
        printHelper(root, 0);
    } else {
        cout<<"The Tree is empty.\n";
    }
}

//Prints Preorder version of tree
void Tree::PrintPreorder() const {
    if(!isEmpty()) {
        bool first = true;
        preorderHelper(root, first);
        cout<<endl;
    } else {
        cout<<"The Tree is empty.\n";
    }
}

//Search and Display Method for tree
bool Tree::searchDep(const string& n) const {
    int depth = 0;
    DNode* res = findDep(n);
    if (res!=nullptr) {
        DNode* res2 = res;
        while (res2->parent!=nullptr) {
            res2=res2->parent;
            depth++;
        }
        cout<<"Name: "<<res->name<<endl;
        if (res->parent==nullptr) {
            cout<<"Parent: None\n";
        } else {
            cout<<"Parent: "<<res->parent->name<<endl;
        }
        cout<<"Child Count: "<<res->children.size()<<endl;
        cout<<"Depth: " <<depth<<endl;
        return true;
    } else {
        cout<<"Department not found.\n";
        return false;
    }
}


