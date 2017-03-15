#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <unordered_set>
#include <limits>

/* CSE 100 PA 4 */
/* Authors: Peter Phan A13042904 cs100wdh
 *          Dephanie Ho A12705618 cs100wam*/

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

    /*
     * Constructor
     */
    ActorNode(string actor): name(actor) {}
    
    /*
     *  Less-than comparison, so ActorNodes will work in std::priority_queue
     *  Parameter - other - other Actornode being compared
     */
    bool operator<(const ActorNode& other);     
};

#endif //ACTORNODE_H
