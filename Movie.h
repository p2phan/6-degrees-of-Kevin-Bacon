#ifndef MOVIE_H
#define MOVIE_H

#include <unordered_set>
#include "ActorNode.h"
/* CSE 100 PA 4 */
/* Authors: Peter Phan
 *          Dephanie Ho */
/* Movie.h - Class that contains movie name and list of actors in the movie */

using namespace std;
class Movie {
private:
    string title;
    int year; 


public:
    string getMovie();
    unordered_set<ActorNode*> actors;

    Movie(string movie, int movieYear): title(movie), year(movieYear) {}


};

#endif //MOVIE_H

