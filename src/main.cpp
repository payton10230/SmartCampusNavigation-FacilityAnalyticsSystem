/*
* main.cpp
* use: implementating all files, .txt files, and menu implementation
* Author: Payton Dencklau
*/
#include "CampusGraph.h"
#include "DepTree.h"
#include "Facility.h"
#include "FacilityManager.h"
// #include "GraphAlgorithims.h"
#include "HashTable.h"
#include "GraphLoader.h"

#include <iostream>
#include <string>
#include <limits>
#include <vector>


//Quick Trim function for Input
static string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if(first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

int main(int argc, char* argv[]) {
  Tree t;
  FacilityManager fm;
  CampusGraph cg;

  int choice=-1;
  string Dname, Dparent, Dreason;
  string ID;
  string Bname;

  //Loading From Files
  string dir = "data";
  //Acception commandline naming data directory
  if (argc>1) {
    dir = argv[1];
  }

  //FOR BUILDINGS.TXT
  loadBuildings(cg,dir);

  //FOR CONNECTIONS.TXT
  loadConnections(cg,dir);

  //FOR FACILITIES.TXT
  string Fpath = dir+"/facilities.txt";
  int accepted=0, rejected=0;
  vector<string> aB;
  for (int i = 0; i<cg.getBuildingCount(); i++) {
    aB.push_back(cg.getBuildingName(i));
  }
  fm.loadFromFile(Fpath,aB, accepted,rejected);
  cout<<"facilities.txt: "<<accepted<<" accepted, "<<rejected<<" rejected, Hash Capacity: "<<fm.getHashCapacity()<<", Hash Entries: "<<fm.getHashEntries()<<", Hash Collisions: "<<fm.getHashCollisions()<<endl;

  //FOR DEPARTMENTS.TXT
  string Dpath = dir+"/departments.txt";
  int ac=0, rj=0;
  t.loadFromFile(Dpath, ac, rj);
  cout<<"departments.txt: "<<ac<<" accepted, "<<rj<<" rejected\n";


  while (choice != 0) {
  cout<<"\n1. View all facilities\n2. Search facility by ID\n3. View campus graph\n4. Display graph counts and degrees\n5. Find reachable buildings (BFS)\n6. Add department\n7. Search department\n8. Display department hierarchy\n9. Run preorder traversal\n0. Exit\n\nEnter choice: ";
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
      fm.viewAllFacilities();
      break;

    //Search facility by ID
    case 2:
      // ID="";
      cout<<"Enter Facility ID: ";
      getline(cin, ID);
      ID=trim(ID);
      if (!ID.empty()) {
        fm.searchFacilityById(ID);
      } else {
        cout<<"Error: ID cannot be empty.\n";
      }
      break;

    //View Campus Graph
    case 3:
      cg.displayGraph();
      break;

    //Display graph counts and degrees
    case 4:
      cg.displayCountAndDegrees();
      break;

    //Find Reachable buildings (BFS)
    case 5:
      // Bname="";
      cout<<"Enter Building Name: ";
      getline(cin, Bname);
      Bname = trim(Bname);
      if(Bname.empty()) {
        cout<<"Error: Name is empty or only whitespace.\n";
        break;
      } else {
        cg.BfsReachable(Bname);
      }
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
