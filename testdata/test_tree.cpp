//unused main for Test 10.4-1

#include "DepTree.h"

int main (
    Tree t;
    string reason;

    //Testing for empty tree
    t.PrintTree();
    t.PrintPreorder();

    //Testing adding root only
    cout<<t.addDep("University", "NONE", reason) <<endl;

    //Testing PrintTree and Print Preorder with entire tree
    t.addDep("College of Engineering", "University", reason) <<endl;
    t.addDep("College of Science", "University", reason) <<endl;
    t.addDep("Computer Science", "College of Engineering", reason) <<endl;
    t.addDep("Electrical Engineering", "College of Engineering", reason) <<endl;
    t.addDep("Mathematics", "College of Science", reason) <<endl;
    t.addDep("Physics", "College of Science", reason) <<endl;

    t.PrintTree();
    t.PrintPreorder();

    



)