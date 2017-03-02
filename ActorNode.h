#ifndef ACTORNODE_H
#define ACTORNODE_H
#include "ActorEdge.h"
#include <unordered_set>
#include <limits>
/* CSE 100 PA 4 */
/* Authors: Peter Phan
 *          Dephanie Ho */
/* ActorNode.h - Node class that stores actor name, previous name, and 
 * corresponding edges that connect the actors and the movies */
class ActorEdge;

using namespace std;

class ActorNode {
private:
    string name;


public:
    struct Vertex{
        bool searched;
        int dist;
        ActorNode* prev;
    };

    Vertex v;

    string getName();
    unordered_set<string> movieString;

   //bool searched;
    //int dist;
    //ActorNode* prev;

    //ActorNode(string actor): name(actor), searched(0), prev(0),
                             //dist(std::numeric_limits<int>::max()) {}
 
    ActorNode(string actor): name(actor) {}

};

#endif //ACTORNODE_H
