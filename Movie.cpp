/**
 * Movie.cpp
 * Authors: Peter Phan A13042904 cs100wdh
 *          Dephanie Ho A12705618 cs100wam
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

/*
 * Less than comparison, so Movie will work in std::priority_queue
 * Parameter: other - other movie being compared 
 */
bool Movie::operator<(const Movie& other)
{
    if(getYear() != other.getYear())
    {
        return other.getYear() < getYear(); 

    }
    return true;
}

