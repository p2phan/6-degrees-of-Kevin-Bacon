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


/** Uses Dijkstra's algortihm to find the shortest weighted path 
 * between 2 nodes
 *
 * Parameter: actorFrom: the node from which to start the search
 *            actorTo: the end node of the searched path
 */
ActorNode* ActorGraph::DijkstraTraverse(string actorFrom, string actorTo)
{
    //Checks to see if node of actors exist
    if(actors.find(actorFrom) == actors.end() ||
       actors.find(actorTo) == actors.end()){
        return 0;
    }

    ActorNode* start = actors[actorFrom];
    ActorNode* end = actors[actorTo];
 
    //Initializes every vertex in the actor node
    for( auto it = actors.begin(); it != actors.end(); ++it)
    {
        it->second->v.searched = false;
        it->second->v.dist = std::numeric_limits<int>::max();
        it->second->v.prevA = 0;
        it->second->v.prevM = "";
    }

    for( auto it = movies.begin(); it != movies.end(); ++it)
    {
        it->second->searched = false;
    }


    //Makes queue to store path    
    priority_queue<ActorNode*, vector<ActorNode*>, ActorNodePtrComp> toExplore;
    toExplore.push(start);
    start->v.dist = 0;
    int short_dist = -1;

    while(!toExplore.empty())
    {
        ActorNode* next = toExplore.top();
        toExplore.pop();
        if(next->v.dist == short_dist)
        {
            return end;
        }


        if(next->v.searched){ continue; }

        
        next->v.searched = true;
            
        auto it = next->movie_history.begin();
        for( ; it!= next->movie_history.end(); it++)
        {
            string movie_title = *it;
            Movie* movie = movies.at(movie_title);
            if(movie->searched){ continue; }
            movie->searched = true;
            auto it2 = movie->cast.begin();
            for( ; it2 != movie->cast.end(); it2++)
            {
                ActorNode* neighbor = actors.at(*it2);
                
                int c = next->v.dist + movie->getWeight();

                //update neighbor accordingly
                if(c < neighbor->v.dist){
                    neighbor->v.dist = c;
                    neighbor->v.prevA = next;
                    neighbor->v.prevM = movie_title;
                    toExplore.push(neighbor);

                    if(neighbor == end){
                        short_dist = c;
                    }
                }


            }
   

        }         

    }

    if(!end->v.searched)
    {
        return 0;
    }
   
    return end;
}

/** Does a BFS search to find the shortest path between two actors
 *
 *  Parameter: actorFrom - starting actor to search from
 *             actorTo - ending actor to search to
 *
 */
ActorNode* ActorGraph::BFSTraverse(string actorFrom, string actorTo)
{
 //   cout << "before vector intilization"<< endl;
    //Checks to see if node of actors exist
    if(actors.find(actorFrom) == actors.end() || 
       actors.find(actorTo) == actors.end()){
        return 0;
    }
    
    ActorNode* start = actors[actorFrom];
    ActorNode* end = actors[actorTo];
    
    //Initializes every vertex in the actor node
    for( auto it = actors.begin(); it != actors.end(); ++it)
    {
        it->second->v.searched = false;
        it->second->v.dist = std::numeric_limits<int>::max();
        it->second->v.prevA = 0;
        it->second->v.prevM = "";
    }
    
    //Makes queue to store path    
    queue<ActorNode*> toExplore;
    toExplore.push(start);
    start->v.dist = 0;

 //  cout << "before while" << endl;
    //BFS runs until queue is empty
    while(!toExplore.empty())
    {
        ActorNode* next = toExplore.front();
        toExplore.pop();
     
   //     cout << "popping " << next->getName() << endl;   
        //Checks through all the neighbors of current actor node
        auto it = next->movie_history.begin();
        for( ; it!= next->movie_history.end(); it++)
        {
     //       cout << *it << " connects..." <<endl;
            //Each actor in the current is the neighbor of the current node
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
                //If neighbor has not been searched, 
                //update neighbor accordingly
                if(next->v.dist+1 < neighbor->v.dist){
                    neighbor->v.searched = true;
                    neighbor->v.dist = next->v.dist+1;
                    neighbor->v.prevA = next;
                    neighbor->v.prevM = movie_title;
                    toExplore.push(neighbor);
                }
                //We want the node if reached the end 
                if(neighbor == end){ return end;}
                
            }
        }
    }
    
    // If it reaches the end, that means that there is no path between
    // the actors
    return nullptr;
}

/** Prints out the path between the two actors
 *
 *  Parameter: path - path to print out
 *             out - outfile to write to
 *
 */
void ActorGraph::printPath(ActorNode* path, ofstream& out)
{
    ActorNode* curr = path;
    stack<pair<string, ActorNode*>> store;

    //Stores the movie and actor in a stack because we want
    //the reverse order
    while(curr->v.prevA)
    {
        store.push(pair<string, ActorNode*>(curr->v.prevM, curr));
        curr = curr->v.prevA;
    }
    
    //Writes out the movie and actor in correct format
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

/** Returns the year after which two actors became connected
 *  using a BFS
 *  Paremter: actor1 - 1st actor to find year of connection
 *            actor2 - 2nd actor to find year of connection
 */
int ActorGraph::AC_BFS(string actor1, string actor2)
{
    if(actors.find(actor1) == actors.end() ||
       actors.find(actor2) == actors.end()){
        return 9999;
    }

    //ActorNode* start = actors[actorFrom];
    //ActorNode* end = actors[actorTo];   

    if(movies.empty()){ return 9999; }

    int min_year = std::numeric_limits<int>::max();
    unordered_map<int, vector<Movie*>* > sortedMovieYear;
    for(auto it = movies.begin(); it!= movies.end(); it++)
    {
        Movie* movie = (*it).second;
        int year = movie->getYear();

        if(year < min_year )
        {
            min_year = year;
        }
        
        //int index =  year-min_year;
        if(sortedMovieYear.find(year) == sortedMovieYear.end())
        {
            vector<Movie*> v;
            sortedMovieYear.insert(pair<int, vector<Movie*>* >
                                       (year, &v));
        }
        vector<Movie*>* v = sortedMovieYear[year];
        v->push_back(movie);
           
    }
    for(int i = min_year; i < 2015; i++)
    {

        vector<Movie*> v = *(sortedMovieYear[i]);
        for(int j = 0; j < v.size(); j++)
        {
            cout << "got movie" << endl;   
            Movie* movie = v[j];
            auto it = movie->cast.begin();
            for( ; it != movie ->cast.end(); it)
            {
                ActorNode* actor = actors.at(*it);
                actor->movie_history.insert();
            }
                actor->movie_history.insert(movie_string);
        }
        if(BFSTraverse(actor1, actor2)){ return min_year;}
        min_year++;
    }
/*

    while(min_year <= 2015)
    {

        for(auto it = movies.begin(); it!= movies.end(); it++)
        {
            Movie* movie = (*it).second;
            if(movie->getYear() == min_year)
            {
                auto it2 = movie->cast.begin();
                for( ; it2 != movie->cast.end(); it2++)
                { 
                    ActorNode* actor = actors.at(*it2);
                    actor->movie_history.insert((*it).first);
                }
            }
            
        }
        if(BFSTraverse(actor1, actor2)){ return min_year;}
        min_year++;
    }
*/
}

/** Does a BFS search to find the shortest path between two actors
 *
 *  Parameter: actorFrom - starting actor to search from
 *             actorTo - ending actor to search to
 *
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
//    cout << "before while"<< endl;
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
      
            //Get the next string before hitting a tab character and put it in 'next'
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
    
  //      cout << "got strings"<< endl;
        //Checks to see if there is already an actor
        if(actors.find(actor_name) == actors.end())
        {
            //Insert actor into the graph    
 	    actors.insert(pair<string, ActorNode*>(actor_name, 
                                     new ActorNode(actor_name)));
        }
        
    //    cout << "actor" << endl; 
    //    cout << actors.at(actor_name)->getName() << endl;
        //Checks to see if there is already a movie
        string movie_title_year = movie_title + "#@" + record[2];
        if(movies.find(movie_title_year) == movies.end())
        {
         
            movies.insert(pair<string, Movie*>(movie_title_year,
                                     new Movie(movie_title, movie_year)));
        // ActorEdge* edge = new ActorEdge(movie, actor);
	}

        Movie* movie = movies.at(movie_title_year);
        movie->cast.insert(actor_name);
	
	//Insert edge(that contain movie information) into actor->edge
	//actor->edges.insert(edge);
	if( use_weighted_edges)
        {
            ActorNode* actor = actors.at(actor_name);
            actor->movie_history.insert(movie_title_year);
        }
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
void ActorGraph::deleteAll()
{
    for(auto it = movies.begin(); it != movies.end(); it++){
        delete (*it).second;
    }
    for(auto it = actors.begin(); it != actors.end(); it++){
        delete (*it).second;
    }
}

/* Destructor */
ActorGraph::~ActorGraph(){deleteAll();}
