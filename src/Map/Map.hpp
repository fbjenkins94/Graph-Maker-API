#ifndef MAP_HPP
#define MAP_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Edge.hpp"

using namespace std;

class Map {
  private:
    // vector storing vertices in the map: id of each vertex = index in vector
    vector<Vertex*> vertices;

    // Map: name of vertex -> id of vertex = index storing vertex ptr
    unordered_map<string, unsigned int> vertexId;

    // Directed edge in vector represents an undirected edge used in MST
    vector<Edge*> undirectedEdges;

    /*
     * Add a vertex with name and x, y coordinates to the map graph. Returns
     * false if the name already existed in the map graph, and true otherwise
     */
    bool addVertex(const string& name, float x, float y);

    /*
     * Add an undirected edge between vertices with names "name1" and "name2".
     * Returns false if either name is not in the map graph.
     */
    bool addEdge(const string& name1, const string& name2);

  public:
    /**
     * Map() will initialize a Map data structure in Map.cpp
     */
    Map();

    /* Build the map graph from vertex and edge files */
    bool buildMapFromFile(const string& vertexFileName,
                          const string& edgeFileName);

    /**
     * Input:
     * 1) string from : node we will be searching from
     * 2) string to : node we will be searching for in a weighted shortest path
     * 3) vector<Vertex*> shortestPath : we will store the nodes traversed 
     *    in the shortest path in this vector.
     *
     * Description: 
     * We will run Dijkstra's shortest weighted path algorithm on the graph
     * we constructed from the files.
     */
    void Dijkstra(const string& from, const string& to,
                  vector<Vertex*>& shortestPath);

    /**
     * Input:
     * 1) vector<Edge*> MST : We will store the edges of the tree that 
     *    constitute a minimal spanning graph in this vector and return it.
     *
     * Description: 
     * as noted above, this function will build a minimal spanning tree from
     * the graph we built using Kruskal's Algorithm.
     */
    void findMST(vector<Edge*>& MST);

    /**
     * Input: 
     * 1) vector<Edge*> roads : we store the crucial edges from the graph we
     *    constructed in this vector.
     *
     * Description:
     * as noted above, this function will gather the crucial edges from the 
     * graph we constructed using a DFS in O(V+E) time.
     */
    void crucialRoads(vector<Edge*>& roads);

    /* Destructor of Map graph */
    ~Map();
};

#endif  // Map_HPP
