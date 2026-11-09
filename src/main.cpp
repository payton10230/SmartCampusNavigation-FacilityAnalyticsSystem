// #include "CampusGraph.h" //Has some errors rn
#include "DepTree.h"
#include "Facility.h"
#include "FacilityManager.h"
#include "GraphAlgorithims.h"
#include "HashTable.h"

#include <iostream>
#include <string>
#include <limits>


//Quick Trim function for Input
static string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if(first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

int main() {
  Tree t;
  FacilityManager fm;
  // CampusGraph cg;

  int choice=-1;
  string Dname, Dparent, Dreason;
  int ID;

  //Loading From Files
  string dir = "data";

  //FOR FACILITIES.TXT
  string Fpath = dir+"/facilities.txt";
  int accepted=0, rejected=0;
  f.loadFromFile(Fpath,accepted,rejected);
  cout<<"facilites.txt: "<<accepted<<" accepted, "<<rejected<<" rejected, Hash Capacity: "<<fm.getHashCapacity()<<", Hash Entries: "<<fm.getHashEntries<<", Hash Collisions: "fm.getHashCollisions<<endl;

  //FOR DEPARTMENTS.TXT
  string Dpath = dir+"/departments.txt";
  int ac=0, rj=0;
  t.loadFromFile(Dpath, ac, rj);
  cout<<"departments.txt: "<<ac<<" accepted, "<<rj<<" rejected\n";


  while (choice != 0) {
  cout<<"1. View all facilities\n2. Search facility by ID\n3. View campus graph\n4. Display graph counts and degrees\n5. Find reachable buildings (BFS)\n6. Add department\n7. Search department\n8. Display department hierarchy\n9. Run preorder traversal\n0. Exit\n\nEnter choice: ";
  cin>>choice;
  if(!cin) {
    if (cin.eof()) {
      break;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout<<"Invalid Choice. Try again\n";
    choice =-1;
    continue;
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  if (choice>9||choice<0) {
    cout<<"Invalid Choice. Try again.\n";
    continue;
  }

  switch (choice) {
    //View all facilites
    case 1:
      fm.viewAllFacilites();
      break;

    //Search facility by ID
    case 2:
      ID=0;
      cout<<"Enter Facility ID: ";
      getline(cin, ID);
      ID=trim(ID);
      if (!ID.empty()) {
        fm.searchFacilityById(ID);
      } else {
        cout<<"Error: Name cannot be empty.\n";
      }
      break;

    //View Campus Graph
    case 3:

      break;

    //Display graph counts and degrees
    case 4:

      break;

    //Find Reachable buildings (BFS)
    case 5:

      break;

    //Add department
    case 6:
      Dname="";
      cout<<"Enter Name: ";
      getline(cin,Dname);
      Dname = trim(Dname);
      if(Dname.empty()) {
        cout<<"Error: Name is empty or only whitespace.\n";
        break;
      } else {
        if (t.isEmpty()) {
           if (t.addDep(Dname, "NONE", Dreason)) {
            cout<<"Success!\n";
            } else {
              cout<<"Error: "<<Dreason<<endl;
              break;
            }
        } else {
          Dparent="";
          cout<<"Enter Parent Name: ";
          getline(cin,Dparent);
          Dparent=trim(Dparent);
          if(Dparent.empty()) {
            cout<<"Error: Parent Name is empty or only whitespace.\n";
            break;
          } else {
            if (t.addDep(Dname, Dparent, Dreason)) {
            cout<<"Success!\n";
            } else {
              cout<<"Error: "<<Dreason<<endl;
              break;
            }
          }
        }
      }

      break;

    //Search Department
    case 7:
      Dname="";
      cout<<"Enter Name: ";
      getline(cin, Dname);
      Dname=trim(Dname);
      if (!Dname.empty()) {
        t.searchDep(Dname);
      } else {
        cout<<"Error: Name cannot be empty.\n";
      }
      break;

    //Display Department Hierarchy
    case 8:
      t.PrintTree();
      break;
    
    //Run preorder traversal
    case 9:
      t.PrintPreorder();
      break;

    case 0:
      cout<<"Exiting menu function.\n";
      break;
    
    default:
      cout<<"Invalid choice.\n";
      break;
  }


  }

  
  return 0;
}
