/*
 * DepTree.h
 * use: node struct and tree for department
 * Author: Payton Dencklau
 */
#ifndef DEPTREE_H
#define DEPTREE_H

#include <vector>
#include <iostream>
#include <string>
using namespace std;

//DNode Struct for Implementation in Tree Class
struct DNode {
    
    string name;
    vector<DNode*> children;
    //Parent initialized to nullptr
    DNode* parent = nullptr;
};

class Tree {
    private:
        DNode* root;
        int numDep;

        //For recursion during findDep
        DNode* findHelper(DNode* cur, const string& n) const;
        //For PrintTree Indention
        void printHelper(const DNode* cur, int depth) const;
        //For PrintPreorder Sequence
        void preorderHelper(const DNode* cur) const;

    public:
        //Constructor to initizalize root and numDep
        Tree();
        //Destructor
        ~Tree();
        //Add Department with name n and parent p
        bool addDep(const string& n, const string& p, string& reason);
        //Find Department with name n
        DNode* findDep(const string& n) const;
        //Check if Name already exists with name n
        bool nameExists(const string& n) const;
        //Check if tree is empty
        bool isEmpty() const;
        //Prints Tree (Regular)
        void PrintTree() const;
        //Prints Preorder version of tree
        void PrintPreorder() const;

};

#endif