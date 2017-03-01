#include "ActorGraph.h"

using namespace std
int main(int argc, char* argv){

    if(argv != 5)
    {
        cout << "Invalid number of arguments" << endl; 
        cout << "Format is ./pathfinder <file of actos/movie> "
             << "u/w-unweighted/weighted <file of pair of actors "
             << "<output file>" << endl;
        return -1;
    }

    ActorGraph actors;
    //call loadFromFile
    //do bfs
    //print out
    

}
