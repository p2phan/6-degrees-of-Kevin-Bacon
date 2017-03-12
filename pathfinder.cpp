#include "ActorGraph.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
/* CSE 100 PA 4 */
/* Authors: Peter Phan
 *          Dephanie Ho
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
    
    //cout << "making graph"<< endl;
    
    ActorGraph graph;
    //Makes the graph
    
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


   // cout << "finding shortest distances between pairs" << endl;

    //Reading pairactor file and printing paths
    ofstream outfile(argv[4]);
    outfile << "(actor)--[movie#@year]-->(actor)--...\n";

    //cout << "header" << endl;

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

        //cout << "before header" <<endl;
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
       // cout << "before bfs" << endl;
       // cout << actor1 << " is with " << actor2 << endl;
        //Find shortest path between actor 1 and actor 2
        ActorNode* endNode;
        if(!weighted)
        {
            endNode = graph.BFSTraverse(actor1, actor2);
        }
        else 
        {
            endNode = graph.DijkstraTraverse(actor1, actor2);
        }


     //   cout << "before print" << endl; 
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

   // cout << "done " <<endl;


}
