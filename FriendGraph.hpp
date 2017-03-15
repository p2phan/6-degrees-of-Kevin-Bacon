/**
 * FriendGraph.hpp 
 * Author: Peter Phan,
 *         Dephanie Ho
 *
 * Date: 03/14/2017
 *
 * Implements a graph that shows a network of friends on Facebook.
 */
#ifndef FRIENDGRAPH_HPP
#define FRIENDGRAPH_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class User
{
    
private:
    int ID; //id of the user

public:

    User(int id) : ID(id), dist(std::numeric_limits<int>::max()), 
                   searched(0), hits(0){} 

    int hits; //used to store number of hits
    int dist; //will be used in a BFS search
    bool searched; //check if user node has been searched
    unordered_set<int> friends; //list of the user's friends

    /** Getter to return ID of user
     */ 
    int getID() const;
 
    /**
     * Less-than comparison, so User will work in std::priority_queue
     * Parameter - other - User being compared
     */ 
    bool operator<(const User& other);
};

/** A 'function class' for use as the Compare class in a
 *  priority_queue<ActorNode*>.
 *  We will make it so that highest distance
 *  has the highest priority
 */
class UserPtrComp
{
public:
    bool operator()(User*& lhs, User*& rhs) const {
        return *lhs < *rhs;
    }
};

class FriendGraph
{
protected:
    unordered_map<int, User*> networks; //friend network of the user

private:
    /**
     * Helper method for destructor
     */
     void deleteAll();

    int min; //smallest id
    int max; //largest id

public:
    /**
     *  Constructor
     */ 
    FriendGraph(void) : min(std::numeric_limits<int>::max()),
                        max(std::numeric_limits<int>::min()) {}

    /**
     *  Destructor
     */ 
    ~FriendGraph();


    /**
     *  Load the graph from a tab-delimited file of user->friend relationships
     *
     *  Parameter: infilename - input file
     *
     *  Return true if file was loaded successfully, false otherwise
     */ 
    bool loadFromFile(const char* infilename);

    /** Does a BFS search to find suggestions
     *  Parameter: user - the user from which to start the BFS
     *  suggestions - the queue to store suggestions
     *            
     */
    void BFS(priority_queue<User*, vector<User*>, UserPtrComp>* q, User* user);
    
    /** Prints out the given suggestions
     *
     */
    void printSuggestions(int id, vector<User*>* v,
                          const char * outfilename);

    /**
     *  Returns a list of suggested friends
     *
     *  Parameter: user - user to search friend network from
     *             numSuggestions - number of friends suggested to return
     */
    void SuggestFriends(const char * outfilename);
};

int User::getID() const
{
    return ID;
}


bool User::operator<(const User& other)
{
    if(hits != other.hits)
    {
        cout << getID() << " vs " <<other.getID() << endl;
        return hits < other.hits;
    }

    return other.getID() < getID();
}


/**
 *  Load the graph from a tab-delimited file of user->friend relationships
 *
 *  Parameter: infilename - input file
 *
 *  Return true if file was loaded successfully, false otherwise
 */ 
bool FriendGraph::loadFromFile(const char* infilename)
{
    cout << networks.size();
    cout << "in" << endl;
    ifstream infile(infilename);
    
    while(infile)
    {
        string s;

        if(!getline( infile, s)) break;

        istringstream ss( s );
   
        vector<string> record;

        while(ss) 
        {
            string next;
            if(!getline(ss, next, ' ')) break;

            record.push_back(next);  
        }
        if(record.size() != 2)
        {
            continue;
        }

        //Retrieve users 
        int id1 = stoi(record[0]);
        int id2 = stoi(record[1]);

        if(id1 < min) min = id1;
        if(id2 < min) min = id2;
   
        if(max < id1) max = id1;
        if(max < id2) max = id2;


        if(networks.find(id1) == networks.end())
        {
            networks.insert(pair<int, User*>(id1, new User(id1) ));
        }

        if(networks.find(id2) == networks.end())
        {
            networks.insert(pair<int, User*>(id2, new User(id2) ));
        }

        User* user1 = networks[id1];
        User* user2 = networks[id2];

        if(user1->friends.find(id2) == user1->friends.end())
        {
            user1->friends.insert(id2);
        } 
         
        if(user2->friends.find(id1) == user2->friends.end())
        {
            user2->friends.insert(id1);
        } 
    } 

    if (!infile.eof()) {
        cerr << "Failed to read " << infilename << "!\n";
        return false;
    }
    infile.close();

    return true;

}

void FriendGraph::BFS(priority_queue<User*, vector<User*>, UserPtrComp>* q, 
                      User* user)

{

    user->dist = 0;
    queue<User*> toExplore;
    toExplore.push(user);
    
    while(!toExplore.empty())
    {
        User* curr = toExplore.front();
        toExplore.pop();
        
        if(curr->dist == 2) break;
        
        auto it = curr->friends.begin();
        for( ; it != curr->friends.end(); it++)
        {
            User* neighbor = networks[(*it)];
            //cout << curr->getID() << " neighbors " << neighbor->getID() <<endl;
            neighbor->hits++;         
    
//            cout << neighbor->hits << " " <<neighbor->getID() << endl;
            if(neighbor->searched || neighbor == curr){ continue; }

            if(curr->dist+1 < neighbor->dist)
            {
                neighbor->searched = true;
                neighbor->dist = curr->dist+1;
                toExplore.push(neighbor);

                if(neighbor->dist == 2)
                {

                    q->push(neighbor);
                }
            }


        }
    }

}


void FriendGraph::printSuggestions(int id, vector<User*>* suggest,
                                   const char* outfilename)
{
    ofstream outfile(outfilename);
    outfile << "For user " << id  << ", we suggest the following friends " 
            << "in order of number of mutual friends:" << endl;

    for(int i = 0; i < suggest->size(); i++)
    {
        User* user = suggest->at(i);
        outfile << user->getID() << " have " << user->hits 
                << " mutuals" << endl;

    }
}

void FriendGraph::SuggestFriends(const char * outfilename)
{

    cout <<networks.size()-1<< endl;
    int numSuggestions; //number of suggestions to print
    int id; // the ID to search for 
    do {
        cout << "Please enter an ID from " << min << "-"  << max
             << " to find suggestions. -1 to quit" << endl;
        
        cin >> id;

        if(id == -1){ return; }

    } while(id <= min && max < id);

    do {
        cout << "Please enter the number of suggestions you would like "
             << "between 1 and 50 inclusive. -1 to quit" << endl
             << "We will try to get as many suggestions as possible :)"
             << endl; 

        cin >> numSuggestions;

        if(numSuggestions== -1){ return;}


    } while(id <= 0 && 50 < id);

    if(networks.find(id) == networks.end())
    {
        cout << "User " << id << " does not exist" << endl;
        return;
    }

    User* user = networks[id];

    priority_queue<User*, vector<User*>, UserPtrComp> q;
    vector<User*> suggest;  
    
    BFS(&q, user);
    
    int counter = 0;
    while(!q.empty() && counter < numSuggestions)
    {
        User* curr = q.top();
        q.pop();
 
        suggest.push_back(curr);
     
        //cout << curr->getID() << " " << curr->hits <<endl;     
        counter++;

/*        if(numSuggestions <= suggest.size())
        {
            auto it = suggest.begin();
            if((*it).first < curr->hits)
            {
                suggest.erase(it);
                suggest.insert(curr);
            }       
        }
        else
        {
            suggest.insert(user);
        }
*/

    }
    
    printSuggestions(id, &suggest, outfilename);

}


void FriendGraph::deleteAll()
{
    for(auto it = networks.begin(); it != networks.end(); it++)
    {
        delete (*it).second;
    }
}


/* Destructor */
FriendGraph::~FriendGraph(){deleteAll();}



#endif //FRIENDGRAPH_HPP
