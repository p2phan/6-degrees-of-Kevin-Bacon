/*
 * ActorGraph.cpp
 * Author: Peter Phan
 *         Dephanie Ho
 * Date:   02/28/2017
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

ActorNode* ActorGraph::BFSTraverse(string actorFrom, string actorTo)
{
    ActorNode* start = actors.at(actorFrom);
    ActorNode* end = actors.at(actorTo);

    if(!start || !end){
        return 0;
    }

    for( auto it = actors.begin(); it != actors.end(); ++it)
    {
        it->second->v.searched = false;
        it->second->v.dist = std::numeric_limits<int>::max();
        it->second->v.prev = 0;
    }
    
    queue<ActorNode*> toExplore;
    start->v.dist = 0;
    toExplore.push(start);

    while(!toExplore.empty())
    {
        ActorNode* next = toExplore.front();
        toExplore.pop();
        
        auto it = next->movieString.begin();
        for( ; it!= next->movieString.end(); it++)
        {
            string movie_title = *it;
            Movie* movie = movies.at(movie_title);
            auto neighbors = movie->actorName.begin();




            ActorNode* neighbor = (*it)->getActorNode();
            if(next->v.dist+1 < neighbor->v.dist){
                neighbor->v.dist = next->v.dist+1;
                neighbor->v.prev = next;
                toExplore.push(neighbor);
            }
        }
    }
    
    return end;
}

void printPath(ActorNode* path, ofstream print)
{
   // for

}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
    
        ActorNode* actor = actors.at(actor_name);
        if(!actor)
        {
            //insert actor into the graph    
            actor = new ActorNode(actor_name);
 	    actors.insert(pair<string, ActorNode*>(actor_name, actor));
        }
        
       
      
        // we have an actor/movie relationship, now what?
        movie_title_year = movie_title + "#@" + record[2];
        Movie* movie = movies.at(movie_title_year);
        if(!movie)
        {
            movie = new Movie(movie_title, movie_year); 
            movie.insert(pair<string, Movie*>(movie_title_year, movie));
        // ActorEdge* edge = new ActorEdge(movie, actor);
	}
	
	//insert edge(that contain movie information) into actor->edge
	//actor->edges.insert(edge);
	
        
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}
