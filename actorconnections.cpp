/*
 * Actorconnections.cpp
 * Author: Peter Phan
 *         Dephanie Ho
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
    //Argument check

    string algorithm;
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



    //graph.loadFromFile(argv[1], false);
    ifstream infile;
    infile.open(argv[2]);

    if(!infile.is_open())
    {
        cout << "Second file cannot be opened" << endl;
        return -1;
    }

    ofstream outfile(argv[3]);
    outfile << "Actor1\tActor2\tYear\n";

    if(algorithm == "bfs")
    {
        ActorGraph graph;
        graph.loadFromFile(argv[1], false);
        
        bool have_header = false;
        while(infile)
        {
            string s;
        
            if(!getline( infile, s )) break;

            if(!have_header) {
            //skip the header
                have_header = true;
                continue;
            }
            istringstream ss( s );
            vector <string> record;
            //get actors that are delimited by tab
            while(ss) {
                string next;
               
                if(!getline( ss, next, '\t' )) break;

                record.push_back( next );
            }
            if(record.size() != 2) {
                continue;
            }
            //Get pair of actors
            string actor1(record[0]);
            string actor2(record[1]);
            
            //cout << "Finding earliest year of connection between " 
            //     << actor1 << "and" << actor2 << endl;
            int year = graph.AC_BFS(actor1, actor2);
          
            outfile << actor1 << "\t" << actor2 << "\t" << year << endl;
        }
    }
    else if(algorithm == "ufind")
    {
        UnionFind set;
        set.loadFromFile(argv[1]);
        

        bool have_header = false;
        while(infile)
        {
            //cout<<"inwhile" << endl;
            string s;
        
            if(!getline( infile, s )) break;

            if(!have_header) {
            //skip the header
                have_header = true;
                continue;
            }

           // cout << "after if's" << endl;

            istringstream ss( s );
            vector <string> record;
            //get actors that are delimited by tab
            while(ss) {
                string next;
               
                if(!getline( ss, next, '\t' )) break;

                record.push_back( next );
            }
            if(record.size() != 2) {
                continue;
            }
            //Get pair of actors
            string actor1(record[0]);
            string actor2(record[1]);
           // cout << "after getting pairs"<<endl;
            int year = set.connectActors(actor1, actor2);
            if(year == -1)
            {
                outfile << "9999" << endl;
            }
            else
            {
                outfile << actor1 << "\t" << actor2 << "\t" << year << endl;
            }
        }       
    }
    else
    {
        cout << "Invalid 4th arg. Please input a 'bfs' or 'ufind'";
    }
    


    //check if there is 4 args
    
    //BFS
    //loadfile false
    //y= 1st year there is movieeeeeeee
    //while Actor A !connected to B
    //	add all edges pertaining to that yr
    //	do bfs, if node, return yr
    //	yr++
    //
    //Disjoint
    //union-merge 2 sets
    //find:given element e, return set
    //
    //ask: how to do disjoint
    //     how to make divjikra faster
    //
    //

}




