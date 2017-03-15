#include "ActorGraph.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
/* CSE 100 PA 4 */
/* Authors: Peter Phan A13042904 cs100wdh
 *          Dephanie Ho A12705618 cs100wam
 * Pathfinder.cpp - Finds the shortest path using ActorGraph 
 * of 2 related nodes
 */
int main(int argc, char** argv){
    //Argument check
    if(argc != 5)
    {
        cout << "Invalid number of arguments" << endl; 
        cout << "Format is ./pathfinder <file of actos/movie> "
             << "u/w-unweighted/weighted <file of pair of actors "
             << "<output file>" << endl;
        return -1;
    }
    
    ActorGraph graph;
    graph.loadFromFile(argv[1], true);

    ifstream infile;
    infile.open(argv[3]);

    if(!infile.is_open())
    {
        cout << "Third file cannot be opened" << endl;
        return -1;
    }

    bool weighted;
    if(string(argv[2]) == "u")
    {
        weighted = false;
    }
    else if(string(argv[2]) == "w")
    {
        weighted = true;
    }
    else
    {
        cout << "Invalid 2nd arg. Please input a 'u' or 'w'";
        infile.close();
        return -1;
    }

    //Reading pairactor file and printing paths
    ofstream outfile(argv[4]);
    outfile << "(actor)--[movie#@year]-->(actor)--...\n";

    bool have_header = false;

    while (infile)
    {
        string s;

        if(!getline( infile, s )) break;
        
        if (!have_header) {
           //skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;
        
        //get actors that are delimited by tab
        while (ss) {
            string next;

            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
         
        if (record.size() != 2) {
             continue;
        }

        //Get pair of actors 
        string actor1(record[0]);
        string actor2(record[1]);
        
        cout << "Calculating path for " << actor1 
             << " and " << actor2 << endl;
        
        //Find shortest path between actor 1 and actor 2 based on weighted
        //or unweighted edges
        ActorNode* endNode;
        if(!weighted)
        {
            endNode = graph.BFSTraverse(actor1, actor2);
        }
        else 
        {
            endNode = graph.DijkstraTraverse(actor1, actor2);
        }

        if(endNode)
        {
            graph.printPath(endNode, outfile);
        }
        else 
        {
            outfile << "No Path" << endl;
        }
    }

    infile.close();
    outfile.close();   
}
