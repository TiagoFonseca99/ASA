/*=====================================*/
/*      ASA 2018/2019 - Projeto 1      */
/*	        João Dinis 89485	       */
/*	      Tiago Fonseca 89542	       */
/*=====================================*/

/* Biblioteca */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int routers, connections;
int** matrix;
vector<int> elements;

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
		routers = stoi(line);
		getline(file, line);
		connections = stoi(line);

		int **matrix = new int*[routers];
		for (int i = 0; i < routers; ++i) {
			elements.push_back(i + 1);
    		matrix[i] = new int[routers];
		}

		for (int i = 0; i < connections; i++) {
			getline(file, line);
			ss << line;
			ss >> numb1 >> numb2;
			ss.str("");
			ss.clear();

			matrix[numb1 - 1][numb2 - 1] = 1;
			matrix[numb2 - 1][numb1 - 1] = 1;
		}

		cout << "MATRIX" << endl;
		for (int i = 0; i <routers; i++) {
			for (int j = 0; j < routers; j++) {
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}

		cout << "Vector" << endl;
		for (int i = 0; i < elements.size(); i++) {
			cout << elements[i] << " " << endl;
		}
		file.close();
	}
}

/* Codigo */
int main(int argc, char *argv[]) {
	readInput(argv[1]);

}
