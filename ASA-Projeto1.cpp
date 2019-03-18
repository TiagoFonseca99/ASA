/*=====================================*/
/*      ASA 2018/2019 - Projeto 1      */
/*	        Joao Dinis 89485	       */
/*	      Tiago Fonseca 89542	       */
/*=====================================*/

/* Biblioteca */
#include <iostream>
#include <fstream>
#include <sstream>
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

// Lista ligada de subNetworks do grafo original
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

	void addEdge(int x, int y) {
	  adjacency[x - 1].push_back(y - 1);
	  adjacency[y - 1].push_back(x - 1);
  	}

	list<int>* getAdjacency() {
		return adjacency;
	}
};


/* Prototipos */
void doAll(Graph graph);
void doAllAux(Graph graph, int id);
void printer();

/* Variaveis */
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
int *subNetworkSize;
int *parent;

void listInit(int numRouters) {
    visited = new bool[numRouters];
	articulationPoints = new bool[numRouters];
    disc = new int[numRouters];
    low = new int[numRouters];
	parent = new int[numRouters];
    subNetworkSize = new int[numRouters];
}

void readInput(string fileName) {
	ifstream file;

	file.open("testes.txt");
	if (!file.is_open()) {
		printf("error while opening the file");
	}
	else {
		string line;
		int numb1, numb2;
		stringstream ss;

		getline(file, line);
		numRouters = stoi(line);
		if (numRouters < 2){
			printf("Numero de routers tem que ser maior ou igual a 2");
			exit(-1);
		}
		getline(file, line);
		numConnections = stoi(line);
		if (numConnections < 1){
			printf("Numero de routers tem que ser maior ou igual a 2");
			exit(-1);
		}

		g.graphInit(numRouters);
		listInit(numRouters);
		for (int i = 0; i < numConnections; i++) {
			getline(file, line);
			ss << line;
			ss >> numb1 >> numb2;

			g.addEdge(numb1, numb2);
			ss.str("");
			ss.clear();
		}
		file.close();
	}
}

/* Codigo */
int main(int argc, char *argv[]) {
	readInput("a");

	// Initialize parent and numRoutersisited
  	for (int i = 0; i < numRouters; i++) {
		visited[i] = false;
		articulationPoints[i] = false;
		subNetworkSize[i] = 0;
    	parent[i] = -1;
  	}
	doAll(g);
	printer();

	return 0;
}

void doAll(Graph graph) {
	int i;
    for (i = numRouters - 1; i > 0; i--) {
        if (visited[i] == false) {
			subNetworkList.AddSubNetwork(i + 1);
            doAllAux(graph, i);
		}
	}
	for (i = 0; i < numRouters; i++) {
		if (articulationPoints[i] == true) {
			subNetworkSize[low[i]]--;
			printf("7 id: %d low[id]: %d subNetworkSize[low[id]]: %d\n", i, low[i], subNetworkSize[low[i]]);
		}
	}
}

void doAllAux(Graph graph, int id) {

    // Count of children in DFS Tree
    int children = 0;

    // Mark the current node as visited
    visited[id] = true;

	// Increment size of SubNetwork
	subNetworkList.getFirst()->incrementSize();

    // Initialize discovery time and low value
    disc[id] = low[id] = totalTime++;
	subNetworkSize[low[id]]++;

	printf("1 id: %d low[id]: %d subNetworkSize[low[id]]: %d\n", id, low[id], subNetworkSize[low[id]]);

    // Go through all vertices aadjacent to this
    list<int>::iterator i;
    for (i = graph.getAdjacency()[id].begin(); i != graph.getAdjacency()[id].end(); ++i) {
        int v = *i;  // v is current adjacent of id

        // If v is not visited yet, then make it a child of id
        // in DFS tree and recur for it
        if (!visited[v]) {
            children++;
            parent[v] = id;
            doAllAux(graph, v);

            // Check if the subtree rooted with v has a connection to
            // one of the ancestors of id
			if (low[v] < low[id]) {
				subNetworkSize[low[id]]--;
				printf("2 id: %d low[id]: %d subNetworkSize[low[id]]: %d\n", id, low[id], subNetworkSize[low[id]]);
				low[id] = low[v];
				subNetworkSize[low[id]]++;
				printf("3 id: %d low[id]: %d subNetworkSize[low[id]]: %d\n", id, low[id], subNetworkSize[low[id]]);
			}

            // id is an articulation point in following cases

            // (1) id is root of DFS tree and has two or more chilren.
			// (2) If id is not root and low value of one of its child is more than discovery value of id.
            if ((parent[id] == -1 && children > 1) || (parent[id] != -1 && low[v] >= disc[id])) {
				if (articulationPoints[id] == false) {
					numAP++;
					articulationPoints[id] = true;
					// Para contar o tamanho dos subNetworks apos os pontos de alocacao serem retiradas
				}
			}
        }
        // Update low value of id for parent function calls.
        else if (v != parent[id]) {
			if (disc[v] < low[id]){
				subNetworkSize[low[id]]--;
				printf("5 id: %d low[id]: %d subNetworkSize[low[id]]: %d\n", id, low[id], subNetworkSize[low[id]]);
				low[id] = disc[v];
				subNetworkSize[low[id]]++;
				printf("6 id: %d low[id]: %d subNetworkSize[low[id]]: %d\n", id, low[id], subNetworkSize[low[id]]);
			}
		}
    }
}

void printer(){

	for (int i = -1; i < numRouters; i++) {
		printf("subNetworkSize[%d]: %d\n", i, subNetworkSize[i]);
	}

	// Problema 1
	printf("%d\n", subNetworkList.size());

	// Problema 2
	subNetworkList.printIds();

	// Problema 3
	printf("%d\n", numAP);

	// Problema 4
	int i, max = 0;
	for(i = 0; i < numRouters; i++) {
		if (subNetworkSize[i] > max)
			max = subNetworkSize[i];
	}
	printf("%d\n", max);
}




/*
typedef struct {
	int id;
	list<int*> connections;
} router;

typedef struct {
	int size;
	router* firstElement;
} graph;
*/









/*
int** matrixInit () {
	int **matrix = new int*[routers];

	for (int i = 0; i < routers; ++i) {
		matrix[i] = new int[routers];
	}

	return matrix;
}

int** addMatrixElement(int x, int y) {
	matrix[x - 1][y - 1] = 1;
	matrix[y - 1][x - 1] = 1;
}

void printMatrix() {
	cout << "MATRIX" << endl;
	for (int i = 0; i <routers; i++) {
		for (int j = 0; j < routers; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}
*/
