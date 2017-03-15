#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

/** Filename: UnionFind.hpp
 *  Author: Peter Phan A13042904 cs100wdh
 *          Dephanie Ho A12705618 cs100wam
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
 * Returns the year after which two actors become connected
 * through a disjoint implementation
 *
 * Parameter: in_filename - input file to get the actor pairs
 *            out_filename - output file that stores the information 
 */
void UnionFind::connectActors(const char* in_filename, const char* out_filename)
{
    //Initialize to opposite extremes to update correctly
    int min_year = std::numeric_limits<int>::max();
    int max_year = std::numeric_limits<int>::min();
  
    //priority queue to store the Movies so that the lowest movie is first
    priority_queue<Movie*, vector<Movie*>, MoviePtrComp> sortedMovieYear;
    for(auto it = movies.begin(); it!= movies.end(); it++)
    {
        Movie* movie = (*it).second;
        int year = movie->getYear();
  
        //updates to lowest and highest movie years
        if(year < min_year )
        {
            min_year = year;
        }

        if(max_year < year )
        {
            max_year = year;
        }
        //puts movie into the priority queue
        sortedMovieYear.push(movie);
    }

    //pairs vector correspond to years vector
    vector<pair<string, string>> pairs;
    loadPairsFromFile(pairs, in_filename);
    vector<int> years(pairs.size(), 9999);

    int counter = 0;    

    //Goes through each year and looks at the movie corresponding to each year
    for(int i = min_year; i <= max_year; i++)
    {
        //makes sure priority queue is not empty
        while(!sortedMovieYear.empty())
        {
            Movie* movie = sortedMovieYear.top();
            //Since the movies are sorted by year, if the top movie's year
            //is different, we move the next year
            if(movie->getYear() != i) {break;}
            string movie_title = movie->getMovie() + "#@" + to_string(i);
            sortedMovieYear.pop();

            //Looks through each movie's actors to add "edges"
            //which is really just the list of movies that actor
            //has been in
            auto it = movie->cast.begin();
            ActorNode* before = actors.at(*it);
            while(++it != movie->cast.end())
            {
                ActorNode* curr = actors.at(*it);
                UF_union(before->getName(), curr->getName());   
                before = curr;
            }   

        }
        //After each year, we will see if the actors are linked
        //if they are, we update the year and compress the path
        for(int j = 0; j < pairs.size(); j++)
        {
            if(years[j] == 9999 &&
               UF_find(pairs[j].first) == UF_find(pairs[j].second))
            {
                years[j] = i;
                counter++;
            }
        }
        if(counter == pairs.size()){ break; }
    }
    //in the end, we print out the connections
    printConnections(pairs, years, out_filename);
}

/**
 * Load the graph from a tab-delimited file of actor->movie relationships
 *
 * Parameter: in_filename - input filename
 */
bool UnionFind::loadFromFile(const char* in_filename)
{

    //Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    //Keep reading lines until the end of file is reached
    while (infile) {
        string s;

        //Get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            //Skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            //Get the next string before hitting a tab character 
            //and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            //We should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        //Checks to see if there is already an actor
        if(actors.find(actor_name) == actors.end())
        {
            //if not insert into the graph
            actors.insert(pair<string, ActorNode*>(actor_name,
                                     new ActorNode(actor_name)));
        }

        ActorNode* actor = actors.at(actor_name);
        actor->v.prevA = actor;

        //Checks to see if there is already a movie
        string movie_title_year = movie_title + "#@" + record[2];
        if(movies.find(movie_title_year) == movies.end())
        {

            movies.insert(pair<string, Movie*>(movie_title_year,
                                     new Movie(movie_title, movie_year)));

        }

        // updates Movie information
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
    // goes through each movie and actor to delete
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
