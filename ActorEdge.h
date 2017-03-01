#ifndef ACTOREDGE_H
#define ACTOREDGE_H

#include "ActorNode.h"
#include "ActorMovie.h"

/* CSE 100 PA4 */
/* Authors: Peter Phan
 *          Dephanie Ho */
/* ActorEdge.h - Class that points to one actor to another through a movie */


class ActorEdge {
public:
    bool searched;

    ActorMovie* getMovie();

    ActorNode* getActorNode();
   

private:
    ActorMovie* movie;
    ActorNode* actor;




};

#endif //ACTOREDGE_H
