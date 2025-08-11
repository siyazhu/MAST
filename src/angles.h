#ifndef ANGLES_H
#define ANGLES_H
#include <string>
#include <vector>
#include "misc.h"
#include "structure.h"
using namespace std;
class Angles{
	public:
		vector<vector<Real>> anglehist;
		int num_atoms;
		int num_elements;
		int bars; //number of bars in hist. Example: bars=101 means we evenly divide angle range from 0 to pi for 100 parts.
		Angles();
		Angles(int bars, int num_elements);
		void zeros();
		int readfile(const char* filename);
		int readfile(string filename);
		Real calculate_error(Angles target);
		void output(const char* filename);
		void output(string filename);
		void calculate_str(Structure struc, Real max_dis);
	private:
		int col;
		int get_type(int mid, int a, int b);
};
#endif
