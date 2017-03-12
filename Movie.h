#ifndef MOVIE_H
#define MOVIE_H

#include <unordered_set>

#include "ActorNode.h"

/* CSE 100 PA 4 */
/* Authors: Peter Phan
 *          Dephanie Ho */

class ActorNode;
//class ActorEdge;

using namespace std;

/* Movie.h - Class that contains movie name and list of actors in the movie */
class Movie {
private:
    string title; //Title of movie
    int year; //Year of movie

public:
    
    unordered_set<string> cast; //Stores actor names that was in movie
    bool searched; //checks to see if the movie has been searched yet

    
    /*
     * Getter to get movie
     */
    string getMovie();


    /*
     * Getter to get the weight calculated as 1+(2015-year)
     */
    int getYear();

    /*
     * Getter to get the weight calculated as 1+(2015-year)
     */ 
    int getWeight();

    /*
     * Constructor
     */
    Movie(string movie, int movieYear): title(movie), year(movieYear) {}

    
};

#endif //MOVIE_H

