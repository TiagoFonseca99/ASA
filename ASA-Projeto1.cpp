/*=====================================*/
/*      ASA 2018/2019 - Projeto 1      */
/*	        Joao Dinis 89485	       */
/*	      Tiago Fonseca 89542	       */
/*=====================================*/

/* Biblioteca */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int numRouters, numConnections;
vector<router> routers;

struct {
	int id;
	vector<int*> connections;
} router;

struct {
	int size;
	router* firstElement;
} graph;

void readInput(string fileName) {
	ifstream file;

	file.open(fileName);
	if (!file.is_open()) {
		cout << "error while opening the file";
	}
	else {
		string line;
		int numb1, numb2;
		stringstream ss;

		getline(file, line);
		numRouters = stoi(line);
		getline(file, line);
		numConnections = stoi(line);

		for (int i = 0; i < connections; i++) {
			getline(file, line);
			router r = new router;

			ss << line;
			ss >> numb1 >> numb2;
			// Não me apeteceu fazer a parte de guardar os elementos xD
			ss.str("");
			ss.clear();
		}

		file.close();
	}
}

/* Codigo */
int main(int argc, char *argv[]) {
	readInput(argv[1]);

}


















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
