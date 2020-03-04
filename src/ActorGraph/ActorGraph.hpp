/**
 * Name: Forest Jenkins
 * Email: fjenkins@ucsd.edu
 * ID: A14747597
 *
 * File Header: ActorGraph.hpp is the header file for ActorGraph.cpp.
 * Below are enumerated the functions that ActorGraph.cpp will inherit.
 * The class uses two other classes: movieEdge and actorVertex that function as
 * the elements of the set that constitute the graph in ActorGraph.cpp.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <vector>
#include <set>

using namespace std;

/**
 * Class Header: movieEdge will carry all the data associated with IMDB movies
 * and their relationships to actors.
 */

class movieEdge; 
/**
 * Class Header: actorVertex will carry all the data associated with IMDB actors
 * and their relationships to movies.
 */
class actorVertex;
/**
 * Class Header: ActorGraph will us movieEdge and actorVertex nodes to construct
 * a graph representing the relationships that actors and movies have with one
 * another.
 */
class ActorGraph {
  //protected:

  public:

    /*
     * constructor of the graph
     */
    ActorGraph();

    /* 
     * declaration of function that will build the graph with actorVertex and
     * movieEdge nodes.
     */
    bool buildGraphFromFile(const char* filename);

    /* 
     * declaration of the function taht will find the shortest path between
     * any two actors via a breadth-first search of the graph.
     */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* 
     * FINAL SUBMISSION
     */
    void predictLink(const string& queryActor, vector<string>& predictionNames,
                     unsigned int numPrediction);

    /* 
     * destructor of the ActorGraph structure
     */
    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
