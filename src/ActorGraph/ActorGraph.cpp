/**
 * TODO: add file header
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>

using namespace std;

/* TODO */
    
class movieEdge {
	public:
		string movie;
		unsigned int year;
		bool visited;
		actorVertex * prev;

		vector<actorVertex *> actors;
		
		movieEdge(const string m, const unsigned int y) 
			: movie(m), year(y), visited(false) {
			actors = vector<actorVertex *>();
		}

		struct movieCompare {
			bool operator() (movieEdge *& a, movieEdge *& b) {
				if(a->movie != b->movie)
					return a->movie > b->movie;
				return a->year < b->year;
			}
		};

		~movieEdge();
};
class actorVertex {
	public: 
		string actor;
		unsigned int dist;
      		bool visited;
		movieEdge * prev;

		vector<movieEdge *> movies;
		
		actorVertex(const string a) : actor(a), dist(0), 
				        visited(false), prev(0) {
			movies = vector<movieEdge *>();
		}

		~actorVertex();
};

ActorGraph::ActorGraph() {
	actorList = vector<actorVertex *>();
	movieList = vector<movieEdge *>();
	//map<actorVertex *> actorList;
	//vector<movieEdge *, int, movieCompare> movieList;	
}

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraphFromFile(const char* filename) {
    ifstream infile(filename);
    bool readHeader = false;
    actorVertex * v;
    movieEdge * e;

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actor(record[0]);
        string title(record[1]);
        int year = stoi(record[2]);

        // TODO: we have an actor/movie relationship to build the graph
	bool actorFound = false;
	bool movieFound = false;
	int i = actorList.size()-1;
	int j = movieList.size()-1;
	//see if actor node already exists
	for(i; i >= 0; i--) {
		if(actorList[i]->actor == actor) {
			actorFound = true;
			break;
		}
	}
	//see if movie node already exists
	for(j; j >= 0; j--) {
		if(movieList[j]->movie == title && movieList[j]->year == year) {
			movieFound = true;
			break;
		}
	}
	//if the actor node exists...
	if(actorFound) {
		//if movie and actor node exist
		if(movieFound) {
			movieList[j]->actors.push_back(actorList[i]);
			actorList[i]->movies.push_back(movieList[j]);
		}
		//if movie node does not exist
		else {
			e = new movieEdge(title, year);
			e->actors.push_back(actorList[i]);
			actorList[i]->movies.push_back(e);
			movieList.push_back(e);
		}
	}
	//if the actor node doesn't exist...
	else {
		v = new actorVertex(actor);
		//if movie node exists but not actor
		if(movieFound) {
			v->movies.push_back(movieList[j]);
			movieList[j]->actors.push_back(v);
			actorList.push_back(v);
		}
		//if neither movie nor actor nodes exist
		else {
			e = new movieEdge(title, year);
			v->movies.push_back(e);
			e->actors.push_back(v);
			movieList.push_back(e);
			actorList.push_back(v);
		}
	}

    }

    // if failed to read the file, clear the graph and return
    if (!infile.eof()) {
        cerr << "Failed to read " << filename << endl;
        return false;
    }
    infile.close();

    return true;
}

void foundShortestPath(actorVertex *& v, string & s) {
	movieEdge * e = v->prev;

	while(e) {
		e = v->prev;
		s.insert(0, ")");
		s.insert(0, v->actor);
		cout<<"1 "<<v->actor<<" ";
		s.insert(0, "(");

		if(e) {
			s.insert(0, "]-->");
			s.insert(0, to_string(e->year));
			cout<<to_string(e->year);
			s.insert(0, "#@");
			s.insert(0, e->movie);
			cout<<" "<<e->movie<<endl;
			s.insert(0, "--[");
			v = e->prev;
		}
	}
	//cout<<"variable shortestPath now has value:"<<endl;
	//cout<<s<<endl;
}

/* TODO */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
	actorVertex * nextActor = NULL;
	for(int i = 0; i < actorList.size(); i++) {
		if(actorList[i]->actor == fromActor) {
			nextActor = actorList[i];
			break;
		}
	}
	if(!nextActor) return;

	queue<actorVertex *> actorExplore;
	queue<actorVertex *> done;


	nextActor->visited = true;
	nextActor->prev = NULL;
	actorExplore.push(nextActor);
	//movieEdge * nextMovie;
	int count = 0;
	while(!actorExplore.empty()) {
		cout<<"HEYYYYYYYYY"<<endl;
		nextActor = actorExplore.front();
		cout<<nextActor->actor<<endl;
		
		//search actor's movie list
		for(int i = 0; i < nextActor->movies.size(); i++) {
			//if the actor's movie hasn't already been visited...
			if(!nextActor->movies[i]->visited) {
				//set movie's prev value to nextActor
				nextActor->movies[i]->prev = nextActor; 
				//visited movie = true
				nextActor->movies[i]->visited = true;
				//search actors from actor's movie
				for(int j = 0; j < nextActor->movies[i]->actors.size();
						j++) {
				//if the actor's colleague hasn't been visited...
					if(!nextActor->movies[i]->actors[j]->visited) {
						//set nextActor's neighbor's prev to 
						//nextActor
						nextActor->movies[i]->actors[j]->prev
						    = nextActor->movies[i];
						//nextActor has been visited
						nextActor->movies[i]->actors[j]->visited
						    = true;
						if(nextActor->movies[i]->actors[j]->actor
							== toActor) {
							count++;
							nextActor = nextActor->movies[i]->actors[j];
							nextActor->dist = count;

							foundShortestPath(nextActor, 
									shortestPath);
							while(!actorExplore.empty()) {
								actorExplore.front()->visited = false;
								if(actorExplore.front()->prev) {
									actorExplore.front()->prev->visited = false;
								}
								actorExplore.pop();
							}
							while(!done.empty()) {
								done.front()->visited = false;
								if(done.front()->prev) {
									done.front()->prev->visited = false;
								}
								done.pop();
							}

							return;
						}
						//add nextActor's neighbor to queue
						actorExplore.push(
							nextActor->movies[i]->actors[j]);
					}
				}
			}
		}
		count++;
		done.push(actorExplore.front());
		actorExplore.pop();
	}
}

/* TODO */
void ActorGraph::predictLink(const string& queryActor,
                             vector<string>& predictionNames,
                             unsigned int numPrediction) {}

/* TODO */
ActorGraph::~ActorGraph() {}
