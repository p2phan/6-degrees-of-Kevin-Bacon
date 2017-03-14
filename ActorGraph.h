/**
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
#include "Movie.h"

#include <iostream>
#include <unordered_map>
#include <queue>

// Maybe include some data structures here

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<ActorNode*>.
 *  We will make it so that lowest distance
 *  has the highest priority
 */
class ActorNodePtrComp {
public:
    bool operator()(ActorNode*& lhs, ActorNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/** A 'function class' for use as the Compare class in a
 *  priority_queue<ActorNode*>.
 *  We will make it so that lowest distance
 *  has the highest priority
 */
class MoviePtrComp {
public:
    bool operator()(Movie*& lhs, Movie*& rhs) const {
        return *lhs < *rhs;
    }
};

/**
 * ActorGraph - a class that is a representative of a graph where
 * actors are nodes, movies are edges, and movie's year is the weight
 */
class ActorGraph {
protected:
    unordered_map<string, Movie*> movies; //Maps movie name to Movie class 
    unordered_map<string, ActorNode*> actors; //Maps actor name to ActorNode
    // Maybe add class data structure(s) here

public:
    /** 
     * Constructor
     */
    ActorGraph(void);
     
    /**
     * Destructor
     */
    ~ActorGraph();

    /** Resets all the actor nodes for bfs search
     */
    void reset();
  

    /** Uses Dijkstra's Alogrithm to find the shortest path
     *  in a weighted graph
     */ 
    ActorNode* DijkstraTraverse(string actorFrom, string actorTo);

    /**
     *  Traverse the graph using BFS
     */
    ActorNode* BFSTraverse(string actorFrom, string actorTo);

    /** Prints out the shortest path of the two actors
     *
     */
    void printPath(ActorNode* path, ofstream& print);

    /**
     *  Uploads a pair of actor strings into the vector
     *
     *  Parameter: v - the vector to upload the pair of actor strings to
     *             in_filename -  name of file to read pairs 
     */  
    bool loadPairsFromFile(vector<pair<string, string>> &pairs,
                           const char* in_filename);

    /**
     * Prints out the information to file
     *
     * Parameter: pairs - vector of actor string pairs
     *            years - vectors of year corresponding to when pairs 
     *            first become connected
     *            out_filename - name to file to write to
     */
    void printConnections(vector<pair<string, string>> &pairs,
                          vector<int> &years, const char* out_filename);

   
    /** Uses BFs to find the year after which two actors became connected
     *  Parameters: actor1 - one of the actor node to find a path from
     *              actor2 - another actor node to find a path from
     *      
     */ 
    void AC_BFS(const char* in_filename, const char* out_filename);

 
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
    /**
     * Helper method for destructor
     */
    void deleteAll();  
};

#endif // ACTORGRAPH_H
