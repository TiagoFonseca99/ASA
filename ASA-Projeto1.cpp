/*=====================================*/
/*      ASA 2018/2019 - Projeto 1      */
/*	        Joao Dinis 89485	       */
/*	      Tiago Fonseca 89542	       */
/*=====================================*/

/* Libraries */
#include <iostream>
#include <list>

using namespace std;

// Classes
class SubNetwork {
	int maxId;
	int size = 0;
	SubNetwork* next = NULL;

public:
	SubNetwork(int id) {
		maxId = id;
	}

	void incrementSize() {
		size ++;
	}

	void setNext(SubNetwork* subNetwork) {
		next = subNetwork;
	}

	SubNetwork* getNext() {
		return next;
	}

	int getMaxId() {
		return maxId;
	}

	int getSize() {
		return size;
	}
};

// Linked list of subNetworks of the original graph
class SubNetworkList {
	SubNetwork *head = NULL;

public:
	SubNetworkList() {}

	void AddSubNetwork(int id) {
		SubNetwork *subNetwork = new SubNetwork(id);
		if (head == NULL) {
			head = subNetwork;
		}
		else {
			subNetwork->setNext(head);
			head = subNetwork;
		}
	}

	void clearSubNetwork() {
		SubNetwork *temp = head;

		while(temp != NULL) {
			head = temp->getNext();
			delete temp;
			temp = head;
		}
	}

	SubNetwork* getFirst() {
		return head;
	}

	int maxId() {
		int max = 0;
		int size = 0;
		SubNetwork* i;

		for(i = head; i != NULL; i = i->getNext()) {
			if (i->getSize() > size) {
				size = i->getSize();
				max = i->getMaxId();
			}
		}
		return max;
	}

	int size() {
		int res = 0;
		SubNetwork* i;

		for(i = head; i != NULL; i = i->getNext()) {
			res++;
		}
		return res;
	}

    void printIds() {
		SubNetwork* i;

		for(i = head; i != NULL; i = i->getNext()) {
			if (i == head) {
				printf("%d", i->getMaxId());
			}
			else {
				printf(" %d", i->getMaxId());
			}
		}
		printf("\n");
	}

	void printHighest() {
		SubNetwork* i;
		int maximum = 0;

		for (i = head; i != NULL; i = i->getNext()) {
			maximum = max(i->getSize(), maximum);
		}
		printf("%d\n", maximum);
	}
};

class Graph {
  int Routers;  // Number of routers
  list<int> *adjacency;

public:
	Graph() {}

	void graphInit (int r) {
		Routers = r;
		adjacency = new list<int>[r];
	}

	void freeAdjacency () {
		delete[] adjacency;
	}

	void addEdge(int x, int y) {
	  adjacency[x - 1].push_back(y - 1);
	  adjacency[y - 1].push_back(x - 1);
  	}

	void removeEdge(int x) {

		list<int>::iterator i;
	    for (i = getAdjacency()[x].begin(); i != getAdjacency()[x].end(); ++i) {
			int v = *i;
			adjacency[v].remove(x);
		}
		adjacency[x].erase(adjacency[x].begin(), adjacency[x].end());
	}

	list<int>* getAdjacency() {
		return adjacency;
	}
};

/* Prototypes */
void doAll(Graph graph);
void doAllAux(Graph graph, int id);
void printer1();
void printer2();

/* Variables */
int numRouters, numConnections;
SubNetworkList subNetworkList = SubNetworkList();
int numAP = 0;
int totalTime = 0;
Graph g;

// Mark all the vertices as not visited
bool *visited;
bool *articulationPoints;
int *disc;
int *low;
int *parent;

void listInit(int numRouters) {
    visited = new bool[numRouters];
	articulationPoints = new bool[numRouters];
    disc = new int[numRouters];
    low = new int[numRouters];
    parent = new int[numRouters];
}

void variablesInit() {
	for (int i = 0; i < numRouters; i++) {
        visited[i] = false;
		articulationPoints[i] = false;
    	parent[i] = -1;
  	}
}

void readInput() {
	int numb1, numb2;

    scanf("%d", &numRouters);
	if (numRouters < 2){
		printf("Numero de routers tem que ser maior ou igual a 2");
		exit(-1);
	}

	scanf("%d", &numConnections);
	if (numConnections < 1){
		printf("Numero de routers tem que ser maior ou igual a 2");
		exit(-1);
	}

	g.graphInit(numRouters);
    listInit(numRouters);
	for (int i = 0; i < numConnections; i++) {
		scanf("%d %d", &numb1, &numb2);
		g.addEdge(numb1, numb2);
	}
}

/* Codigo */
int main(int argc, char *argv[]) {

	readInput();

	variablesInit();
	doAll(g);
	printer1();

	// Removes adjacencies from all articulation points
	for (int i = 0; i  < numRouters; i++) {
		if (articulationPoints[i] == true) {
			g.removeEdge(i);
		}
	}
	numConnections -= numAP;

	// Clears all the subNetwork content
	subNetworkList.clearSubNetwork();

	variablesInit();
	doAll(g);
	printer2();

	delete[] visited;
	delete[] articulationPoints;
	delete[] disc;
	delete[] low;
	delete[] parent;

	subNetworkList.clearSubNetwork();
	g.freeAdjacency();

	return 0;
}

void doAll(Graph graph) {
    for (int i = numRouters - 1; i > 0; i--) {
        if (visited[i] == false) {
			subNetworkList.AddSubNetwork(i + 1);
            doAllAux(graph, i);
		}
	}
}

void doAllAux(Graph graph, int id) {
    int children = 0;
    visited[id] = true;
	disc[id] = low[id] = totalTime++;

	// Increment size of SubNetwork
	subNetworkList.getFirst()->incrementSize();

    list<int>::iterator i;
    for (i = graph.getAdjacency()[id].begin(); i != graph.getAdjacency()[id].end(); ++i) {
        int v = *i;

        if (!visited[v]) {
            children++;
            parent[v] = id;
            doAllAux(graph, v);

			low[id] = min(low[v], low[id]);

            if ((parent[id] == -1 && children > 1) || (parent[id] != -1 && low[v] >= disc[id])) {
				if (articulationPoints[id] == false) {
					numAP++;
                    articulationPoints[id] = true;
				}
			}
        }
        else if (v != parent[id]) {
			low[id] = min(disc[v], low[id]);
		}
    }
}

void printer1(){
	// 1st Output
	printf("%d\n", subNetworkList.size());

	// 2nd Output
	subNetworkList.printIds();

	// 3rd Output
	printf("%d\n", numAP);
}

void printer2() {
	// 4th Output
	subNetworkList.printHighest();
}
