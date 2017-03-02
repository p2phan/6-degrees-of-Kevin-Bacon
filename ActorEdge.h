#ifndef ACTOREDGE_H
#define ACTOREDGE_H

#include "ActorNode.h"
#include "Movie.h"

/* CSE 100 PA4 */
/* Authors: Peter Phan
 *          Dephanie Ho */
/* ActorEdge.h - Class that points to one actor to another through a movie */

class ActorNode;
class Movie;

class ActorEdge {
private:
    Movie* movie;
    ActorNode* actor;


public:
    bool searched;

    Movie* getMovie();

    ActorNode* getActorNode();
   
    ActorEdge(Movie* movie_edge, ActorNode* actor_node):
                                 movie(movie_edge), actor(actor_node) {}

};

#endif //ACTOREDGE_H
