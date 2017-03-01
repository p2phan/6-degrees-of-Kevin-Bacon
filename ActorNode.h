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


using namespace std;

class ActorNode {
private:
    string name;


public:
    string getName();
    unordered_set<ActorEdge*> edges;

    bool searched;
    int dist;
    ActorNode* prev;

    ActorNode(string actor): name(actor), searched(0), prev(0),
                             dist(std::numeric_limits<int>::max()) {}
 


};

#endif //ACTORNODE_H
