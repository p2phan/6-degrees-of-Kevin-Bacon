/*
 * Actorconnections.cpp
 * Author: Peter Phan A13042904 cs100wdh
 *         Dephanie Ho A12705618 cs100wam
 * Date: 03/09/2017
 *
 * This class determines after which year did two actors became connected.
 */

#include "ActorGraph.h"
#include "UnionFind.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace  std;

int main(int argc, char** argv) {
    string algorithm;// stores which algorithm will be used

    //Argument check
    if(argc == 4)
    {
        algorithm = "ufind";

    }
    else if(argc == 5)
    {
        algorithm = string(argv[4]);
    }
    else
    {
        cout << "Invalid number of arguments" << endl;
        cout << "Format is ./actorconnections <file of actor/movies>"
             << "<file of pairs> <output file> bfs/ufind" << endl;
        return -1;
    }

    //Connect actors through either BFS search or Union-find
    if(algorithm == "bfs")
    {
        ActorGraph graph;
        graph.loadFromFile(argv[1], false);
        
        graph.AC_BFS(argv[2], argv[3]);

    }
    else if(algorithm == "ufind")
    {
        UnionFind set;
        set.loadFromFile(argv[1]);
        
        set.connectActors(argv[2], argv[3]);
   }
   else
   {
       cout << "Invalid 4th arg. Please input a 'bfs' or 'ufind'";
   }
}
