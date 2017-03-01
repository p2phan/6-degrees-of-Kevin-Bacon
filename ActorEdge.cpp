/*
 *
 *
 */

#include "ActorEdge.h"

/** Getter method to acces the Movie class stored
 */
ActorMovie* ActorEdge::getMovie()
{
    return movie;
}

/** Getter method to access the ActorNode stored 
 */
ActorNode* ActorEdge::getActorNode()
{
    return actor;

}
