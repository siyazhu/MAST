#include "voroneighbors.h"
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

Voroneighbors::Voroneighbors(){
	num_elements = 0;
	col = 0;
}

Voroneighbors::Voroneighbors(int num_elements){
	this->num_elements = num_elements;
	voronhist.resize(num_elements);
	col = int(pow(15,num_elements));
	for (int i = 0;i<num_elements;i++){
		anglehist[i].resize(col);
	}
}

void Voroneighbors::zeros(){
	for (int i = 0; i < num_elements; i++){
		for (int j = 0; j < col; j++){
			anglehist[i][j] = 0;
		}
	}
}

int Voroneighbors::readfile(const char* filename){
	std::ifstream inputfile(filename);
	if (!inputfile) {
		std::cerr << "Read angles wrong, file not exist!";
		return(-1);
	}
	inputfile >> num_elements;
	col = int(pow(15,num_elements));
	anglehist.resize(num_elements);
        for (int i = 0; i < num_elements; i++){
                anglehist[i].resize(col);
        	for (int j = 0; j < col; j++){
			inputfile >> anglehist[i][j];
		}
	}
	inputfile.close();
	return(0);
}

Real Voroneighbors::calculate_error(Voroneighbors target){
	Real sum = 0;
	if (num_elements!=target.num_elements) {
		//parameters not match
		cerr<<"Voroneighbors calculate_error wrong! Parameters not match!";
		return (-1);
	}
	for (int i = 0; i < num_elements; i++){
		for (int j = 0; j < col; j++){
			sum += (voronhist[i][j]-target.voronhist[i][j]) * (voronhist[i][j] - target.voronhist[i][j]);
		}
	}
	return(sum);
}

void Voroneighbors::output(const char* filename){
	std::ofstream outputfile;
	outputfile.open(filename);
	outputfile << num_elements << "\n";
	for (int i = 0; i < num_elements; i++){
                for (int j = 0; j < col; j++){
                	outputfile << anglehist[i][j] << "\t";
		}
		outputfile << "\n";
        }
	outputfile.close();
}
