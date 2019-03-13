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
};

/* Variaveis */
int numRouters, numConnections;
Graph *g = new Graph();

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
		getline(file, line);
		numConnections = stoi(line);

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
	return 0;
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
