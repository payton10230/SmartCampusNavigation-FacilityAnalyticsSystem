/*
 * CampusGraph.h
 * use: unweighted graph of campus buildings
 * Author: Austin Feste 
 */

#pragma once
#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

#using namespace std

class CampusGraph {
    private:
        vector<> names;
        vector<> adj;
        int edgeCount;

        //none case sensitive string compare
        static bool equalsIgnore(string& a, string b);
    public:
        //creates an empty graph
        CampusGraph();
        //adds building, returns false and sets reason if dupe or empty name
        bool addBuilding(string& name, string& reason);
        //adds connection, returns false and sets reason if building is unknown or duped
        bool addConnection(string& a, string& b, string& reason);
        //returns index of building, -1 if not found
        int findbuilding(string& name);
        //returns name at index
        string& getBuildingName(int index);
        //returns neighbors of index, insertion order
        vector<int>& getNeighbors(int index);
        //returns tot buildings
        int getBuildingCount();
        //returns tot connections
        int getConnectionCount();
        //returns degree
        int getDegree();
        //returns false if graph is empty
        bool isEmpty();
}
#endif //CAMPUSGRAPH_H_
