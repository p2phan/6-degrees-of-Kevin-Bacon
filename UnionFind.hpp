#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

/** Filename: UnionFind.hpp
 *  Author: Peter Phan  
 *          Dephanie Ho
 *  Date:   03/10/17
 *
 *  Implements a disjoint set that allows us to keep track
 *  of all the connected sets of actors.
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

    /**
     *  Uploads a pair of actor strings into the vector
     *
     *  Parameter: v - the vector to upload the pair of actor strings to
     *             in_filename -  name of file to read pairs 
     */
    bool loadPairsFromFile(vector<pair<string, string>> &v,
                           const char* in_filename);

    /**
     * Prints out the information to file
     *
     * Parameter: pairs - vector of actor string pairs
     *            years - vectors of year corresponding to when pairs 
     *            first become connected
     *            out_filename - name to file to write to
     */
    void printConnections(vector<pair<string, string>> &pairs,
                          vector<int> &years, const char* out_filename);


    /** Makes the sentinel of the shorter set point to the sentinel
     *  of the longer set
     *
     *  Parameter: actor1 - actor node from the set to union
     *             actor2 - other actor node from the set to union     
     */
    void UF_union(string actor1, string actor2);

    /** Finds the sentinel thorugh path compresssion
     * 
     *  Parameter: actor - node from which to find sentinel
     */
    ActorNode* UF_find(string actor);

    /**
     *  Resets the unions
     */  
    void reset();
    
    /**
     * Returns the year after which two actors become connected
     * through a disjoint implementation
     *
     * Parameter: in_filename - input file to get the actor pairs
     *            out_filename - output file that stores the information 
     */
    void connectActors(const char* in_filename, const char* out_filename); 


    /** 
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     *
     * Return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename);

private:
    /**
     * Helper method for destructor
     */ 
    void deleteAll();
};

/**
 *  Uploads a pair of actor strings into the vector
 *
 *  Parameter: v - the vector to upload the pair of actor strings to
 *             in_filename -  name of file to read pairs 
 */
bool UnionFind::loadPairsFromFile(vector<pair<string, string>> &pairs,
                           const char* in_filename)
{
    ifstream infile(in_filename);

    bool have_header = false;

    while(infile)
    {   
        string s;
            
        if(!getline( infile, s )) break;
            
        if(!have_header) {
        //skip the header 
            have_header = true;
            continue;
        }
        istringstream ss( s );
        vector <string> record;
        //get actors that are delimited by tab
        
        while(ss) {
            string next;
                
            if(!getline( ss, next, '\t' )) break;
                
            record.push_back( next );
        }
        if(record.size() != 2) {
            continue;
        }
        //Get pair of actors
        string actor1(record[0]);
        string actor2(record[1]);

        pairs.push_back(pair<string, string>(actor1, actor2));            
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
}

/**
 * Prints out the information to file
 *
 * Parameter: pairs - vector of actor string pairs
 *            years - vectors of year corresponding to when pairs 
 *            first become connected
 *            out_filename - name to file to write to
 */
void UnionFind::printConnections(vector<pair<string, string>> &pairs,
                          vector<int> &years, const char* out_filename)
{
    ofstream outfile(out_filename);
    outfile << "Actor1\tActor2\tYear\n";

    for(int i = 0; i < pairs.size(); i++)
    {

        outfile << pairs[i].first << "\t" << pairs[i].second << "\t" 
                << years[i] << endl; 

    }
    outfile.close();
}

/**
 * Makes the sentinel of the shorter set point to the sentinel
 * of the longer set
 *
 * Parameter: actor1 - actor node from the set to union
 *            actor2 - other actor node from the set to union
 */
void UnionFind::UF_union(string actor1, string actor2)
{
    int height1;
    int height2;

    ActorNode* curr1 = actors[actor1];
    ActorNode* curr2 = actors[actor2];
    
    //Get heights from both sets   
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
 
    //Check which set points to which set based on height
    if(height1 < height2)
    {
        curr1->v.prevA = curr2;
    }
    else
    {
        curr2->v.prevA = curr1;
    }
}

/**
 * Finds the sentinel through path compression 
 *
 * Parameter: actor - node from which to find the sentinel
 */
ActorNode* UnionFind::UF_find(string actor)
{
    ActorNode* curr = actors[actor];

    queue<ActorNode*> compress;
 
    //Stores pathway to sentinel in a queue   
    while(curr->v.prevA != curr)
    {
        compress.push(curr);
        curr = curr->v.prevA;
    }
    
    //Get the sentinel, which is at the end of the queue
    while(!compress.empty())
    {
        ActorNode* reattach = compress.front();
        compress.pop();
        reattach->v.prevA = curr;
    }
    return curr;
}

/**
 * Resets the unions
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
 * Returns the year after which two actors become connected
 * through a disjoint implementation
 *
 * Parameter: in_filename - input file to get the actor pairs
 *            out_filename - output file that stores the information 
 */
void UnionFind::connectActors(const char* in_filename, const char* out_filename)
{
    int min_year = std::numeric_limits<int>::max();
    int max_year = std::numeric_limits<int>::min();
    priority_queue<Movie*, vector<Movie*>, MoviePtrComp> sortedMovieYear;
    for(auto it = movies.begin(); it!= movies.end(); it++)
    {
        Movie* movie = (*it).second;
        int year = movie->getYear();
        if(year < min_year )
        {
            min_year = year;
        }

        if(max_year < year )
        {
            max_year = year;
        }
        
        sortedMovieYear.push(movie);
    }

    vector<pair<string, string>> pairs;
    loadPairsFromFile(pairs, in_filename);
    vector<int> years(pairs.size(), 9999);

    
    for(int i = min_year; i <= max_year; i++)
    {

        while(!sortedMovieYear.empty())
        {
            Movie* movie = sortedMovieYear.top();
            if(movie->getYear() != i) {break;}
            string movie_title = movie->getMovie() + "#@" + to_string(i);
            sortedMovieYear.pop();


            auto it = movie->cast.begin();
            ActorNode* before = actors.at(*it);
            while(++it != movie->cast.end())
            {
                ActorNode* curr = actors.at(*it);
                UF_union(before->getName(), curr->getName());   
                before = curr;
            }   

        }
        //turn into for loop
        for(int j = 0; j < pairs.size(); j++)
        {
            if(years[j] == 9999 &&
               UF_find(pairs[j].first) == UF_find(pairs[j].second))
            {
                years[j] = i;
            }
        }
    }
    printConnections(pairs, years, out_filename);
}

/**
 * Load the graph from a tab-delimited file of actor->movie relationships
 *
 * Parameter: in_filename - input filename
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
