#ifndef VORONEIGHBORS_H
#define VORONEIGHBORS_H
#include <string>
#include <vector>
#include "misc.h"
using namespace std;
class Voroneighbors{
	public:
		vector<vector<Real>> voronhist;
		int num_elements;
		Voroneighbors();
		Voroneighbors(int num_elements);
		void zeros();
		int readfile(const char* filename);
		Real calculate_error(Voroneighbors target);
		void output(const char* filename);
	private:
		int col;
};
#endif
