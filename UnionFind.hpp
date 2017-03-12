#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

/** Filename: UnionFind.hpp
 *  Author: Peter Phan  
 *          Dephanie Ho
 *  Date:   03/10/17
 *
 *  Implements a disjoint set that allows us to keep track
 *  of all cthe connected sets of actors.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

#include "ActorNode.h"
#include "Movie.h"

using namespace std;

class UnionFind
{
protected:
    unordered_map<string, Movie*> movies;
    unordered_map<string, ActorNode*> actors;        

public:
    /*
     * Constructor
     */
    UnionFind(void) {}

    /*
     * Destructor
     */
    ~UnionFind();

    /** Makes the sentinel of the shorter set point to the sentinel
     *  of the longer set
     *
     *  Parameter: actor1 - actor node from the set to union
     *             actor2 - other actor node from the set to union     
     */
    void UF_union(string actor1, string actor2);

    /** Finds the sentinel thorugh path compresssion
     * 
     *  Parameter: actor - node from which to find sential
     */
    ActorNode* UF_find(string actor);

    /**
     *  Resets the unions
     */  
    void reset();

    /** Return the year after which two actors become connected
     *  through a disjoint implementation
     *
     *  Parameter: actor1 - first actor string to find connection 
     *             actor2 - second actor string to find connections
     */
    int connectActors(string actor1, string actor2); 


    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     *
     * return true if file was loaded sucessfully, false otherwise
     */

    bool loadFromFile(const char* in_filename);

private:
    void deleteAll();
};

/**
 *
 */
void UnionFind::UF_union(string actor1, string actor2)
{
    int height1;
    int height2;

    ActorNode* curr1 = actors[actor1];
    ActorNode* curr2 = actors[actor2];
    
    while(curr1->v.prevA !=curr1)
    {
        curr1 = curr1->v.prevA;
        height1++;
    }

    while(curr2->v.prevA !=curr2)
    {
        curr2 = curr2->v.prevA;
        height2++;
    }
 
    if(height1 < height2)
    {
        curr1->v.prevA = curr2;
    }
    else
    {
        curr2->v.prevA = curr1;
    }


}


/*
 *
 *
 *
 */
ActorNode* UnionFind::UF_find(string actor)
{
    ActorNode* curr = actors[actor];

    queue<ActorNode*> compress;
    
    while(curr->v.prevA != curr)
    {
        compress.push(curr);
        curr = curr->v.prevA;
    }

    while(!compress.empty())
    {
        ActorNode* reattach = compress.front();
        compress.pop();
        reattach->v.prevA = curr;
    }
    return curr;
}



/*
 *
 *
 *
 */
void UnionFind::reset()
{
    for(auto it = actors.begin(); it != actors.end(); it++)
    {
        ActorNode* actor = (*it).second;
        actor->v.prevA = actor;
    }
}

/**
 *
 */
int UnionFind::connectActors(string actor1, string actor2)
{
    if(actors.find(actor1) == actors.end() ||
       actors.find(actor2) == actors.end()){
        return -1;
    }

    if(movies.empty()){ return -1; }

    reset();

    int year = std::numeric_limits<int>::max();
    for(auto it = movies.begin(); it!= movies.end(); it++)
    {
        ///hash_map of hashset
        //key is year
        //every time we will look at the year
        //and store the corresponding year to the hashset of the 
        //year in the hasmap
        if((*it).second->getYear() < year )
        {
            year = (*it).second->getYear();
        }
    }

    while(year <= 2015)
    {
        for(auto it = movies.begin(); it!= movies.end(); it++)
        {
            Movie* movie = (*it).second;
            if(movie->getYear() == year)
            {
                auto it2 = movie->cast.begin();
                ActorNode* before = actors.at(*it2);
                while(++it2 != movie->cast.end())
                {
                    ActorNode* curr = actors.at(*it2);
                    UF_union(before->getName(), curr->getName());   
                    before = curr;            
                }
            }

        }

        if(UF_find(actor1) == UF_find(actor2)) { return year;}

        year++;
    }


    return -1;
}

/**
 *
 *
 */
bool UnionFind::loadFromFile(const char* in_filename)
{
   ifstream infile(in_filename);

    bool have_header = false;


    while (infile) {
        string s;

        if (!getline( infile, s )) break;

        if (!have_header) {

            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {

            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        if(actors.find(actor_name) == actors.end())
        {
            actors.insert(pair<string, ActorNode*>(actor_name,
                                     new ActorNode(actor_name)));
        }

        ActorNode* actor = actors.at(actor_name);
        actor->v.prevA = actor;

        string movie_title_year = movie_title + "#@" + record[2];
        if(movies.find(movie_title_year) == movies.end())
        {

            movies.insert(pair<string, Movie*>(movie_title_year,
                                     new Movie(movie_title, movie_year)));

        }

        Movie* movie = movies.at(movie_title_year);
        movie->cast.insert(actor_name);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

/*
 * Helper method for destructor
 */
void UnionFind::deleteAll()
{
    for(auto it = movies.begin(); it != movies.end(); it++){
        delete (*it).second;
    }
    for(auto it = actors.begin(); it != actors.end(); it++){
        delete (*it).second;
    }
}

/* Destructor */
UnionFind::~UnionFind(){deleteAll();}



#endif //UNIONFIND_HPP
