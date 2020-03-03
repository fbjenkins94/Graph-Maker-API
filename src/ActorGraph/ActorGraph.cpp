/**
 * TODO: add file header
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <set>
#include <iterator>

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

		/**struct compare {
			bool operator() (movieEdge *& a, movieEdge *& b) {
				return a->movie > b->movie;
			}
		};
		struct m_find {
			m_find( const string & movie) : movie(movie) {}
			bool operator() (const movieEdge *& e) {
				return e->movie == movie;
			}
		private: 
			string movie;
		};**/

		//~movieEdge();
};
class actorVertex {
	public: 
		string actor;
      		bool visited;
		movieEdge * prev;
		vector<movieEdge *> movies;
		
		actorVertex(const string a) : actor(a), visited(false), prev(0){
			movies = vector<movieEdge *>();
		}

		/**struct compare {
			bool operator() (actorVertex *& a, actorVertex *& b) {
				return a->actor > b->actor;
			}
		};
		struct a_find {
			a_find( const string & actor) : actor(actor) {}
			bool operator() (const actorVertex *& v) {
				return v->actor == actor;
			}
		private: 
			string actor;
		};**/

		//~actorVertex();
};
struct m_Compare {
	bool operator() (const movieEdge * a, const movieEdge * b) {
		return a->movie > b->movie;
	}
};
struct a_Compare {
	bool operator() (const actorVertex * a, const actorVertex * b) {
		return a->actor > b->actor;
	}
};
    

	set<actorVertex *, a_Compare> actorList[255];
      	set<movieEdge *, m_Compare> movieList[146];

ActorGraph::ActorGraph() {
	//set<actorVertex *, a_Compare> actorList[255];
      	//set<movieEdge *, m_Compare> movieList[146];
	//actorList = {0}; //names
	//movieList = {0}; //years
	//actorList = <int, actorVertex *>();
	//movieList = map<string, movieEdge *>();	
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

	/**
	 * To search for movie: see if array[year-1874].find(movie) != nullptr. 
	 * If not, then return that node with iterator, copy it and update it,
	 * delete it and then insert the newly updated copy.
	 *
	 * To search for actor, see if array[name[0]].find(actor) returns !NULL.
	 * If so, obtain through iterator and do the same as before.
	 *
	 */

	actorVertex * tempA = new actorVertex(actor);
	const char c = actor[0];
	set<actorVertex *, a_Compare>:: iterator itActor = actorList[c].find(tempA);

	movieEdge * tempM = new movieEdge(title, year);
	set<movieEdge *, m_Compare>:: iterator itMovie = movieList[year-1874].find(tempM);

	if(itActor != actorList[c].end()) {
		//delete tempA;
		tempA = *itActor;
		actorList[c].erase(itActor); 
	}
	if(itMovie != movieList[year-1874].end()) {
		//delete tempM;
		tempM = *itMovie;
		movieList[year-1874].erase(itMovie);
		tempM->actors.push_back(tempA);
		tempA->movies.push_back(tempM);
		
		movieList[year-1874].insert(tempM);
		actorList[c].insert(tempA);
	}
	else {
		tempM->actors.push_back(tempA);
		tempA->movies.push_back(tempM);

		movieList[year-1874].insert(tempM);
		actorList[c].insert(tempA);
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

void p(actorVertex *& v, string & s) {
	movieEdge * e = v->prev;

	while(e) {
		e = v->prev;
		s.insert(0, ")");
		s.insert(0, v->actor);
		s.insert(0, "(");

		if(e) {
			s.insert(0, "]-->");
			s.insert(0, to_string(e->year));
			s.insert(0, "#@");
			s.insert(0, e->movie);
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
	const char c = fromActor[0];
	actorVertex * nextActor = new actorVertex(fromActor);
	set<actorVertex *, a_Compare>:: iterator itActor = actorList[c].find(nextActor);
	
	if(itActor != actorList[c].end()) nextActor = *itActor;
	else return;

	queue<actorVertex *> actorExplore;
	queue<actorVertex *> done;

	nextActor->visited = true;
	nextActor->prev = NULL;
	actorExplore.push(nextActor);
	
	while(!actorExplore.empty()) {
		nextActor = actorExplore.front();

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
							nextActor = nextActor->movies[i]->actors[j];
							p(nextActor, shortestPath);

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
						actorExplore.push(nextActor->movies[i]->actors[j]);
					}
				}
			}
		}
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
