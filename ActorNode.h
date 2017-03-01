#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <unordered_set>

/* CSE 100 PA 4 */
/* Authors: Peter Phan
 *          Dephanie Ho */
/* ActorNode.h - Node class that stores actor name, previous name, and 
 * corresponding edges that connect the actors and the movies */


using namespace std:

class ActorNode {

public:
    ActorNode(string actor): name(actor) {}
    bool searched;
    int dist;
    ActorNode* prev;
    string getName();
    unordered_set edges;


private:
    string name;









};

#endif //ACTORNODE_H
