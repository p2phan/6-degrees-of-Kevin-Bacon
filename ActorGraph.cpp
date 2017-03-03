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
#include <stack>

#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

ActorNode* ActorGraph::BFSTraverse(string actorFrom, string actorTo)
{
 //   cout << "before vector intilization"<< endl;
    if(actors.find(actorFrom) == actors.end() || 
       actors.find(actorTo) == actors.end()){
        return 0;
    }

    ActorNode* start = actors[actorFrom];
    ActorNode* end = actors[actorTo];

    for( auto it = actors.begin(); it != actors.end(); ++it)
    {
        it->second->v.searched = false;
        it->second->v.dist = std::numeric_limits<int>::max();
        it->second->v.prevA = 0;
        it->second->v.prevM = "";
    }
    
    queue<ActorNode*> toExplore;
    start->v.dist = 0;
    toExplore.push(start);

 //  cout << "before while" << endl;
    while(!toExplore.empty())
    {
        ActorNode* next = toExplore.front();
        toExplore.pop();
     
   //     cout << "popping " << next->getName() << endl;   
        auto it = next->movie_history.begin();
        for( ; it!= next->movie_history.end(); it++)
        {
     //       cout << *it << " connects..." <<endl;
            string movie_title = *it;
            Movie* movie = movies.at(movie_title);
            auto it2 = movie->cast.begin();
            for( ; it2 != movie->cast.end(); it2++)
            {
       //         cout << *it2 << " actor in movie" << endl;
                ActorNode* neighbor = actors.at(*it2);
 
//cout << neighbor->v.searched << " is bool" << endl;
//cout << neighbor->v.dist << " is dist" << endl;
                if(neighbor->v.searched){ continue;}           
                
//cout << neighbor->v.prevM<< " is movie" << endl;
                if(next->v.dist+1 < neighbor->v.dist){
                    neighbor->v.searched = true;
                    neighbor->v.dist = next->v.dist+1;
                    neighbor->v.prevA = next;
                    neighbor->v.prevM = movie_title;
                    toExplore.push(neighbor);
                }

                if(neighbor == end){ return end;}
                
            }


            //:86
            //ActorNode* neighbor = (*it)->getActorNode();
            
        }
    }
    
    // If it reaches the end, that means that there is no path between
    // the actors
    return nullptr;
}

void ActorGraph::printPath(ActorNode* path, ofstream& out)
{
    ActorNode* curr = path;
    stack<pair<string, ActorNode*>> store;

    while(curr->v.prevA)
    {
        store.push(pair<string, ActorNode*>(curr->v.prevM, curr));
        curr = curr->v.prevA;
    }

    out << "(" << curr->getName() << ")";
    while(!store.empty())
    {
        auto print = store.top();
        store.pop();
        out << "--[" << print.first << "]-->("
            << print.second->getName() << ")";

    }
    
    out << "\n";
}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
//    cout << "before while"<< endl;
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
    
  //      cout << "got strings"<< endl;
        if(actors.find(actor_name) == actors.end())
        {
            //insert actor into the graph    
 	    actors.insert(pair<string, ActorNode*>(actor_name, 
                                     new ActorNode(actor_name)));
        }
        
        ActorNode* actor = actors.at(actor_name);
    //    cout << "actor" << endl; 
    //    cout << actors.at(actor_name)->getName() << endl;
      
        // we have an actor/movie relationship, now what?
        string movie_title_year = movie_title + "#@" + record[2];
        if(movies.find(movie_title_year) == movies.end())
        {
         
            movies.insert(pair<string, Movie*>(movie_title_year,
                                     new Movie(movie_title, movie_year)));
        // ActorEdge* edge = new ActorEdge(movie, actor);
	}

        Movie* movie = movies.at(movie_title_year);
	
	//insert edge(that contain movie information) into actor->edge
	//actor->edges.insert(edge);
        actor->movie_history.insert(movie_title_year);
        movie->cast.insert(actor_name);
        
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}
