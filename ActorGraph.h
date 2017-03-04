/*
 * ActorGraph.h
 * Author: Peter Phan, 
 *         Dephanie Ho
 * Date:   02/28/2017
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include "ActorNode.h"
#include "ActorEdge.h"
#include "Movie.h"

#include <iostream>
#include <unordered_map>
#include <queue>

// Maybe include some data structures here

using namespace std;
/*
 * ActorGraph - a class that is a representative of a graph where
 * actors are nodes, movies are edges, and movie's year is the weight
 */
class ActorGraph {
protected:
    unordered_map<string, Movie*> movies; //Maps movie name to Movie class 
    unordered_map<string, ActorNode*> actors; //Maps actor name to ActorNode
    // Maybe add class data structure(s) here

public:
    /* 
     * Constructor
     */
    ActorGraph(void);
     
    /*
     * Destructor
     */
    ~ActorGraph();

    // Maybe add some more methods here
    /*
     *  Traverse the graph using BFS
     */
    ActorNode* BFSTraverse(string actorFrom, string actorTo);

    /** Prints out the shortest path of the two actors
     *
     */
    void printPath(ActorNode* path, ofstream& print);
  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    
private:
    /*
     * Helper method for destructor
     */
    void deleteAll();  
};


#endif // ACTORGRAPH_H
