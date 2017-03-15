/**
 * ActorNode.cpp
 * Author: Peter Phan A13042904 cs100wdh
 *         Dephanie Ho A12705618 cs100wam
 *
 * Date:   2/28/2017
 * 
 * Purpose: Node class for ActorGraph to store actor name and movies
 * they were in
 */

#include "ActorNode.h"

/* Getter method to get name of the actor
 */
string ActorNode::getName() 
{
    return name;
}

/** Less-than comparison, so ActorNodes will work in std::priority_queue
 *  We want small counts to have high priority. 
 *  And we want to break ties deterministically.
 *
 *  Parameter:
 *         other: the ActorNode we are comparing current ActorNode to
 */
bool ActorNode::operator<(const ActorNode& other)
{
    //Return the opposite because we want the lowest to be at the top
    if(v.dist != other.v.dist)
    {
        return other.v.dist < v.dist;
    }

    return true; 

}
