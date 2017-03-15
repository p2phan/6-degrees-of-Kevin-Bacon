/**
 * Filename: extension.cpp
 * Authors: Peter Phan A13042904 cs100wdh
 *          Dephanie Ho A12705618 cs100wam
 *
 * Purpose: Given a Facebook friend data set, recommend someone to add
 *          based on their social connections 
 *
 */
#include <iostream>

#include "FriendGraph.hpp"

using namespace std;

int main(int argc, char** argv)
{

    if(argc != 3)
    {

        cout << "invalid number of arguments. The correct format is : "
             << endl
             << "./extension <friend list dataset> <outputfile>"
             << endl;
        return -1;
    }

    //Find suggested friends through BFS search 
    FriendGraph graph;
    
    graph.loadFromFile(argv[1]);
    graph.SuggestFriends(argv[2]);

    return 0;
}









