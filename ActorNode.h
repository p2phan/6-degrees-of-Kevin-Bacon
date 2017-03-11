#ifndef ACTORNODE_H
#define ACTORNODE_H

//#include "ActorEdge.h"
#include <unordered_set>
#include <limits>

/* CSE 100 PA 4 */
/* Authors: Peter Phan
 *          Dephanie Ho */
//class ActorEdge;

using namespace std;

/* ActorNode - Node class that stores actor name, previous name, and 
 * corresponding edges that connect the actors and the movies */
class ActorNode {
private:
    string name; //Name of actor


public:
    struct Vertex{
        bool searched; //Bool to check if vertex has been searched 
        int dist; //Dist from start node in BFS
        ActorNode* prevA; //Store previous actor 
        string prevM; //Store previous movie
    };
    
    Vertex v; //Vertex to run search algorithm
    unordered_set<string> movie_history; //Stores strings of movies
    
    /*
     * Getter to get actor name
     */
    string getName();

    //ActorNode(string actor): name(actor), searched(0), prev(0),
                             //dist(std::numeric_limits<int>::max()) {}
    /*
     * Constructor
     */
    ActorNode(string actor): name(actor) {}
    
    /*
     *  Less-than comparison, so ActorNodes will work in std::priority_queue
     */
    bool operator<(const ActorNode& other);     
};

#endif //ACTORNODE_H
