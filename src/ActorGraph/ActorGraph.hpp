/**
 * TODO: add file header
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <vector>
#include <set>

using namespace std;

/**
 * TODO: add class header
 */

class movieEdge; 
class actorVertex;

class ActorGraph {
  protected:
    // TODO: add data structures used in actor graph

  public:
//set<actorVertex *, a_Compare> * actorList[255] = new set<actorVertex *, a_Compare>();
//set<movieEdge *, m_Compare> * movieList[146] = new set<movieEdge *, m_Compare>();
	  //vector<actorVertex *> actorList;
	  //vector<movieEdge *> movieList;
	  //set<string, actorVertex *> actorList[255];
	  //set<string, movieEdge *> movieList[146];
    /* TODO */
    ActorGraph();

    /* TODO */
    bool buildGraphFromFile(const char* filename);

    /* TODO */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* TODO */
    void predictLink(const string& queryActor, vector<string>& predictionNames,
                     unsigned int numPrediction);

    /* TODO */
    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
