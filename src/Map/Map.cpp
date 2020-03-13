
#include "Map.hpp"
#include <queue>
#include <vector>
#include <stack>

struct Compare {
	bool operator() (pair<int, Vertex *> & a, pair<int, Vertex *> & b) {
		return a.first > b.first;
	}
};

struct edgeCompare {
	bool operator() (Edge *& a, Edge *& b) {
		return a->weight > b->weight;
	}
};

/* TODO */
Map::Map() {}

/* Build the map graph from vertex and edge files */
bool Map::buildMapFromFile(const string& vertexFileName,
                           const string& edgeFileName) {
    // add vertices first
    ifstream vertexFile(vertexFileName);
    while (vertexFile) {
        string s;
        if (!getline(vertexFile, s)) break;

        // process data at each line
        istringstream ss(s);
        vector<string> data;
        while (ss) {
            string str;
            if (!getline(ss, str, ' ')) break;
            data.push_back(str);
        }
        if (data.size() != 3) continue;

        // add vertex defined in this line to the graph
        string name(data[0]);
        float x = stoi(data[1]);
        float y = stoi(data[2]);

        addVertex(name, x, y);
    }

    // then add edges
    ifstream edgeFile(edgeFileName);
    while (edgeFile) {
        string s;
        if (!getline(edgeFile, s)) break;

        // process data at each line
        istringstream ss(s);
        vector<string> data;
        while (ss) {
            string str;
            if (!getline(ss, str, ' ')) break;
            data.push_back(str);
        }
        if (data.size() != 2) continue;

        // add edge defined in this line to the graph
        string name1(data[0]);
        string name2(data[1]);

        addEdge(name1, name2);
    }

    return true;
}

/*
 * Add a vertex with name and x, y coordinates to the map graph. Returns
 * false if the name already existed in the map graph, and true otherwise
 */
bool Map::addVertex(const string& name, float x, float y) {
    if (vertexId.count(name) > 0) return false;
    vertexId[name] = vertices.size();
    vertices.push_back(new Vertex(name, x, y));
    return true;
}

/*
 * Add an undirected edge between vertices with names "name1" and "name2".
 * Returns false if either name is not in the map graph.
 */
bool Map::addEdge(const string& name1, const string& name2) {
    if (vertexId.count(name1) == 0 || vertexId.count(name2) == 0) {
        return false;
    }
    unsigned int id1 = vertexId[name1];
    unsigned int id2 = vertexId[name2];
    Vertex* v1 = vertices[id1];
    Vertex* v2 = vertices[id2];
    float weight = sqrt(pow(v1->x - v2->x, 2) + pow(v1->y - v2->y, 2));
    v1->outEdges.push_back(new Edge(v1, v2, weight));
    v2->outEdges.push_back(new Edge(v2, v1, weight));

    undirectedEdges.push_back(new Edge(v1, v2, weight));
    return true;
}

void foundShortestPath(vector<Vertex *>& sp, Vertex * v) {
	if(v->prev) foundShortestPath(sp, v->prev);
	v->prev = NULL;
	v->dist = -1;
	v->done = false;
	sp.push_back(v);
}	

void resetVertices(priority_queue<pair<int, Vertex *>, vector<pair<int, Vertex*>>,
		Compare> & pq, vector<Vertex *> & dump) {
	Vertex * v;
	while(!pq.empty()) {
		v = pq.top().second;
		v->done = false;
		v->dist = -1;
		v->prev = NULL;
		pq.pop();
	}
	for(int i = 0; i < dump.size(); i++) {
		v = dump[i];
		v->done = false;
		v->dist = -1;
		v->prev = NULL;
	}
}

/* TODO */
void Map::Dijkstra(const string& from, const string& to,
                   vector<Vertex*>& shortestPath) {
    	if (vertexId.count(from) == 0 || vertexId.count(to)== 0) return;
	Vertex * v;
	int index = vertexId[from];
	v = vertices[index];
	v->dist = 0;
	v->prev = NULL;
	cout<<v->name<<endl;

	priority_queue<pair<int, Vertex *>, vector<pair<int, Vertex *>>, 
			Compare> pq;
	pair<int, Vertex *> pear;
       	pear = make_pair(0, v);
	pq.push(pear);

	vector<Vertex *> dump = vector<Vertex *>();
	while(!pq.empty()) {
		v = pq.top().second;
		pq.pop();

		if(!v->done) {
			v->done = true;

			if(v->name == to) {
				foundShortestPath(shortestPath, v);
				resetVertices(pq, dump);
				return;
			}	

			for(int i = 0; i < v->outEdges.size(); i++) {
				Vertex * next;
				int c = v->dist + v->outEdges[i]->weight;

				if(!v->outEdges[i]->source->done) {
					next = v->outEdges[i]->source;

					if(c < next->dist || next->dist == -1) {
						next->prev = v;
						next->dist = c;
					}
					pear = make_pair(c, next);
					pq.push(pear);
				}
				else if(!v->outEdges[i]->target->done) {
					next = v->outEdges[i]->target;
					
					if(c < next->dist || next->dist == -1) {
						next->prev = v;
						next->dist = c;
					}
					pear = make_pair(c, next);
					pq.push(pear);
				}
			}
			dump.push_back(v);
		}
	}
	foundShortestPath(shortestPath, v);
	resetVertices(pq, dump);
}

bool findMerge(Vertex *& source, Vertex *& target) {
	vector<Vertex *> up1;
	vector<Vertex *> up2;
	/**
	 * Find()
	 */
	while(source->prev || target->prev) {
		if(source->prev) {
			up1.push_back(source);
			source = source->prev;
		}
		if(target->prev) {
			up2.push_back(target);
			target = target->prev;
		}
	}
	for(int i = 0, j = 0; i < up1.size() || j < up2.size(); i++, j++) {
		if(i < up1.size()) up1[i]->prev = source;
		if(j < up2.size()) up2[j]->prev = target;
	}
	/**
	 * Merge()
	 */
	if(source->dist == -1) source->dist = 1;
	if(target->dist == -1) target->dist = 1;
	if(source != target) {
		if(source->dist <= target->dist) {
			if(target->dist <= 1) target->dist += 1;
			source->prev = target;
		}
		else {
			if(source->dist <= 1) source->dist += 1;
			//else source->dist += target->dist;
			target->prev = source;
		}
		return true;
	}
	else return false;
}

/* TODO */
void Map::findMST(vector<Edge*>& MST) {
	priority_queue<Edge *, vector<Edge *>, edgeCompare> pq;

	for(int i = 0; i < undirectedEdges.size(); i++) 
		pq.push(undirectedEdges[i]);
	
	Edge * e;
	Vertex * e1;
	Vertex * e2;
	while(MST.size() < vertices.size()-1) {//OOPS! Double check deleting -1
		e = pq.top();
		pq.pop();
	
		MST.push_back(e);
		e1 = e->source;
		e2 = e->target;	
		if(!findMerge(e1, e2)) MST.pop_back();
	}
}
/**void dfs2() {
	Vertex * u = vertices[0];
	Vertex * v;
	int timer = 0;

	Stack<Vertex *> s;
	s.push(u);
	while(!s.empty()) {
		u = s.top();
		s.pop();
		u->done = true;
		u->dist = u->low = timer++;

		for(int i = 0; i < u->outEdges.size(); i++) {
			if(!u->outEdges[i]->visited) {
				u->outEdges[i]->visited = true;
				v = u->outEdges[i]->target;

				if(v->done) {
				}
				else {
				}
			}
		}
	}
}**/
void dfs(Vertex *& u, Vertex *& v, int & timer, vector<Edge *> & r) {
	u->done = true;
	u->dist = u->low = timer++;
	Vertex * w;

	for(int i = 0; i < u->outEdges.size(); i++) {
		w = u->outEdges[i]->target;
		if(w!=v) {
			if(w->done) {
				if(u->low > w->dist) u->low = w->dist;
			}
			else {
				dfs(w, u, timer, r);
				if(u->low > w->low) u->low = w->low;
				if(w->low > u->dist) r.push_back(u->outEdges[i]);
			}
		}
	}
}

/* TODO */
void Map::crucialRoads(vector<Edge*>& roads) {
	int timer = 0;
	Vertex * u;

	for(int i = 0; i < vertices.size(); i++) {
		if(!vertices[i]->done) {
			u = vertices[i];
			dfs(u, u->prev, timer, roads);
		}
	}
}

/* Destructor of Map graph */
Map::~Map() {
    for (Vertex* v : vertices) {
        for (Edge* e : v->outEdges) {
            delete e;
        }
        delete v;
    }
    for (Edge* e : undirectedEdges) {
        delete e;
    }
}
