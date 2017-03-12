/**
 * Movie.cpp
 * Authors: Peter Phan
 *          Dephanie Ho
 *
 * Date: 28 February 2017
 */

#include "ActorNode.h"
#include "Movie.h"

/* Getter to return name of movie
 */
string Movie::getMovie() 
{
    return title;
}

/*
 * Returns the year of the movie
 */
int Movie::getYear() const
{
    return year;
}

/*
 * Returns the weight of the movie
 */
int Movie::getWeight()
{
    return 1 + (2015-year);
}

bool Movie::operator<(const Movie& other)
{
    if(getYear() != other.getYear())
    {
        return other.getYear() < getYear(); 

    }

    return true;

}

