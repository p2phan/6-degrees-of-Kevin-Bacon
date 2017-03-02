#ifndef MOVIE_H
#define MOVIE_H

#include <unordered_set>
#include "ActorNode.h"
/* CSE 100 PA 4 */
/* Authors: Peter Phan
 *          Dephanie Ho */
/* Movie.h - Class that contains movie name and list of actors in the movie */

class ActorNode;
class ActorEdge;

using namespace std;
class Movie {
private:
    string title;
    int year;
    string movie; 


public:
    
    string getMovie();
    unordered_set<string> actorName;

    Movie(string movie, int movieYear): title(movie), year(movieYear) {}


};

#endif //MOVIE_H

