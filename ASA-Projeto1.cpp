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

	public :
		SubNetwork(int id){
			maxId = id;
		}

		void incrementSize(){
			size ++;
		}

		void setNext(SubNetwork* subNetwork){
			next = subNetwork;
		}

		SubNetwork* getNext(){
			return next;
		}
		int getMaxId(){
			return maxId;
		}

		int getSize(){
			return size;
		}

};

// Lista ligada de subNetworks do grafo original
class SubNetworkList{
	SubNetwork *head = NULL;

	public:

		SubNetworkList() {}

		void AddSubNetwork(int id){
			SubNetwork *subNetwork = new SubNetwork(id);
			if (head == NULL){
				head = subNetwork;
			}
			else{
				subNetwork->setNext(head);
				head = subNetwork;
			}

		}

		SubNetwork* getFirst(){
			return head;
		}

		int maxId(){
			int max = 0;
			int size = 0;
			SubNetwork* i;
			for(i=head; i->getNext() != NULL; i=i->getNext()){
				if (i->getSize() > size){
					size = i->getSize();
					max = i->getMaxId();
				}
			}
			return max;
		}

		int size(){
			int res = 0;
			SubNetwork* i;
			for(i=head; i->getNext() != NULL; i=i->getNext()){
				res++;
			}
		}

		void printIds(){
			SubNetwork* i;
			for(i=head; i->getNext() != NULL; i=i->getNext())
				printf("%d ", i->getMaxId());
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
		adjacency = new list<int>(r);
	}

  void addEdge(int x, int y) {
    adjacency[x - 1].push_back(y);
    adjacency[y - 1].push_back(x);
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
int time = 0;
Graph *g = new Graph();

// Mark all the vertices as not visited 
// Nao funciona se nao for ponteiro, dunno why
bool *visited = new bool[numRouters]; 
int *disc = new int[numRouters]; 
int *low = new int[numRouters];
int *subNetworkSize = new int[numRouters];
int *parent = new int[numRouters]; 

void readInput(string fileName) {
	ifstream file;

	file.open(fileName);
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

		g->graphInit(numRouters);
		for (int i = 0; i < numConnections; i++) {
			getline(file, line);
			ss << line;
			ss >> numb1 >> numb2;

			g->addEdge(numb1, numb2);
			ss.str("");
			ss.clear();
		}
		file.close();
	}
}

/* Codigo */
int main(int argc, char *argv[]) {
	readInput(argv[1]);

	// Initialize parent and numRoutersisited 
  for (int i = 0; i < numRouters; i++) {
		visited[i] = false;
		subNetworkSize[i] = 0;
    parent[i] = -1; 
  } 

	printer();

	return 0;
}
// The function to do DFS traversal. It uses recursive function APUtil() 
void doAll(Graph graph) {  

    // Call the recursive helper function to find articulation points 
    // in DFS tree rooted with vertex 'i' 
    for (int i = numRouters-1; i > 0; i--) { 
        if (visited[i] == false) {
						subNetworkList.AddSubNetwork(i);
            doAllAux(graph, i); 
				}
		}
  
} 

// A recursive function that find articulation points using DFS traversal 
// id --> The vertex to be visited next 
// visited[] --> keeps tract of visited vertices 
// disc[] --> Stores discovery times of visited vertices 
// parent[] --> Stores parent vertices in DFS tree 
// ap[] --> Store articulation points 
void doAllAux(Graph graph, int id) { 
  
    // Count of children in DFS Tree 
    int children = 0; 
  
    // Mark the current node as visited 
    visited[id] = true; 

		// Increment size of SubNetwork
		subNetworkList.getFirst()->incrementSize();
  
    // Initialize discovery time and low value 
    disc[id] = low[id] = time++;
		subNetworkSize[low[id]]++;
  
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
						if (low[v] < low[id]){
							subNetworkSize[low[id]]--;
							low[id] = low[v];
							subNetworkSize[low[id]]++;
						} 
  
            // id is an articulation point in following cases 
  
            // (1) id is root of DFS tree and has two or more chilren. 
						// (2) If id is not root and low value of one of its child is more than discovery value of id. 
            if (parent[id] == -1 && children > 1 || parent[id] != -1 && low[v] >= disc[id]) {
							numAP++;
							// Para contar o tamanho dos subNetworks apos os pontos de alocacao serem retiradas
							subNetworkSize[low[id]]--;
						}
        } 
  
        // Update low value of id for parent function calls. 
        else if (v != parent[id]) {
					if (disc[v] < low[id]){
							subNetworkSize[low[id]]--;
							low[id] = disc[v];
							subNetworkSize[low[id]]++;
						} 
				}
    } 
} 
  

void printer(){
	// Problema 1
	printf("%d\n", subNetworkList.size());

	// Problema 2
	subNetworkList.printIds();

	// Problema 3
	printf("%d\n", numAP);

	// Problema 4
	int max = 0;
	int i;
	for(i=0; i<numRouters; i++)
		if (subNetworkSize[i] > max)
			max = subNetworkSize[i];
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
