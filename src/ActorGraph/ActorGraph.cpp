/**
 * Name: Forest Jenkins
 * Email: fjenkins@ucsd.edu
 * ID: A14747597
 * Sources: Piazza (personal and peer questions)
 * 	    cplusplus.com for references to data structures
 * 	    stepik textbook
 * 	    discussion and lecture slides 
 *
 * File Header: ActorGraph.cpp inherits the methods from ActorGraph.hpp.
 * The file initializes and declares the instances of the movieEdge and 
 * actorVertex classes and provides personalized comparator functions 
 * for these classes that will be inserted into a set.
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

/*
 * Class Header: movieEdge contains: 
 * 1) string movie; title of the movie to be inserted into the graph
 * 2) unsigned int year; year that the title was released in theaters
 * 3) bool visited; true = the node was visited during a BFS
 * 4) actorVertex * prev; pointer to previous actor visited in a BFS
 * 5) vector<actorVertex *> actors; contains pointers to actorVertex nodes
 * 	containing actors that appeared in the movie
 */
    
class movieEdge {
	public:
		string movie;
		unsigned int year;
		bool visited;
		actorVertex * prev;
		vector<actorVertex *> actors;
		//constructor that initializes a movieEdge object
		movieEdge(const string m, const unsigned int y) 
			: movie(m), year(y), visited(false) {
			actors = vector<actorVertex *>();
		}
		//destructor for movieEdge object
		~movieEdge() {}
};
/*
 *Class Header: actorVertex contains: 
 * 1) string actor; name of the actor that the object represents
 * 2) bool visited; true = the node was visited during a BFS
 * 3) movieEdge * prev; pointer to previous movie visited in a BFS
 * 4) vector<movieEdge *> movies; contains pointers to movieEdge nodes
 * 	containing movies that the actor appeared in
 */
class actorVertex {
	public: 
		string actor;
      		bool visited;
		movieEdge * prev;
		vector<movieEdge *> movies;
		//constructor that initializes actorVertex object
		actorVertex(const string a) : actor(a), visited(false), prev(0){
			movies = vector<movieEdge *>();
		}
		//destructor for actorVertex object
		~actorVertex() {}
};
//personal comparator for the movieEdge class
struct m_Compare {
	bool operator() (const movieEdge * a, const movieEdge * b) {
		return a->movie > b->movie;
	}
};
//personal comparator for the actorVertex class
struct a_Compare {
	bool operator() (const actorVertex * a, const actorVertex * b) {
		return a->actor > b->actor;
	}
};
    
//array of sets containing actorVertex nodes
set<actorVertex *, a_Compare> actorList[255];//255 = ASCII symbols
//array of sets containing movieEdge nodes
set<movieEdge *, m_Compare> movieList[146];//146 = 2020-146 = 1874 (first movie
					   //on imdb.
/**set<actorVertex *, a_Compare> * actorList = 
	new set<actorVertex *, a_Compare>[255];
set<movieEdge *, m_Compare> * movieList = 
	new set<movieEdge *, m_Compare>[146];**/

//constructor for ActorGraph
ActorGraph::ActorGraph() {
	/**set<actorVertex *, a_Compare> * actorList = 
		new set<actorVertex *, a_Compare>[255];
      	set<movieEdge *, m_Compare> * movieList = 
		new set<movieEdge *, m_Compare>[146];**/
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
		delete tempA;
		tempA = *itActor;
		actorList[c].erase(itActor); 
	}
	if(itMovie != movieList[year-1874].end()) {
		delete tempM;
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
/*
 * Function Header: 
 *
 * Input:
 * 1) actorVertex *& v; reference to actorVertex pointer that represents the
 * 	final actorVertex node visited in a BFS algorithm. This is the toActor
 * 	that was found from fromActor.
 * 2) string & s; reference to shortestPath string. We will print the path to
 * 	this string from v.
 *
 * Description: 
 * This function starts at the actorVertex containing toActor and retraces the
 * path back to the fromActor node in a BFS traversal. We will print actor
 * contents first to the string, then movie contents, and repeat the process
 * until we have arrived at our fromActor node, at which point the function will
 * finish and return the shortestPath string completed.
 */
void foundShortestPath(actorVertex *& v, string & s) {
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

/* 
 * Function Header: 
 *
 * input: 
 * 1) const string& fromActor; constant reference to fromActor string 
 * 	representing the actor that we want find a shortest path from
 * 	to the toActor.
 * 2) const string& toActor; constant reference to toActor string 
 * 	representing the actor that we want to find a shortest path to
 * 	from the fromActor
 * 3) string& shortestPath; reference to empty shortestPath string that
 * 	we will print the shortest path to and return.
 *
 * Description: 
 * function starts by first finding the fromActor node. If it doesn't exist
 * then the function returns with an empty shortestPath string. It it exists, 
 * then we test if the fromActor string is equal to the toActor string. If it
 * is, then our shortest path is automatically 0 and we return a short 
 * shortestPath string consisting of the actor's name. If the two input string
 * differ, then we run through the algorithm that follows, which is a BFS
 * search consisting of two queues that keep track of all of the actor nodes
 * we visit and should visit next. Once the algorithm finishes, we make sure
 * to return each actor node's visited value to false and return the string.
 */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
	if(fromActor == toActor) return;	

	const char c = fromActor[0];
	actorVertex * nextActor = new actorVertex(fromActor);
	set<actorVertex *, a_Compare>:: iterator itActor = actorList[c].find(nextActor);
	
	if(itActor != actorList[c].end()) {
		delete nextActor;
		nextActor = *itActor;
	}
	else {
		delete nextActor;
		return;
	}

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
							foundShortestPath(nextActor, shortestPath);

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
}

/* 
 * FINAL SUBMISSION
 */
void ActorGraph::predictLink(const string& queryActor,
                             vector<string>& predictionNames,
                             unsigned int numPrediction) {}

/*
 * Function Header: 
 *
 * Description: 
 * Using iterators, we traverse our movieList and actorList arrays and traverse
 * through the set at each bucket, deleting every actorVertex or movieEdge node
 * in our sets.
 */
ActorGraph::~ActorGraph() {
	set<actorVertex *, a_Compare>:: iterator a;
	set<movieEdge *, m_Compare>:: iterator m;

	for(int i = 0; i < 255; i++) {       
		a = actorList[i].begin();

		while(a != actorList[i].end()) {
			//actorVertex * v1 = *a;
			delete *a;
			a++;
		}
	}
	for(int j = 0; j < 146; j++) {
		m = movieList[j].begin();

		while(m != movieList[j].end()) {
			//movieEdge * v2 = *m;
			delete *m;
			m++;
		}
	}
}
