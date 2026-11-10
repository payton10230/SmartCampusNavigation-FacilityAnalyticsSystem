/*
 * GraphLoader.h
 * Purpose: Loads buildings.txt and connections.txt into CampusGraph
 * Austin Feste
 */

 #ifndef GRAPHLOADER_H
 #define GRAPHLOADER_H

 #include <string>
 #include "CampusGraph.h"

 void loadBuildings(CampusGraph& g, const string& dir);

 void loadConnections(CampusGraph& g, const string& dir);

 #endif // GRAPHLOADER_H