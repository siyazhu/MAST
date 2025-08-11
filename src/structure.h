#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <string>
#include <vector>
#include "element.h"
#include "misc.h"
using namespace std;
class Structure{
	public:
		int num_total;
		int num_elements;
		vector<int> type;
		vector<int> num_atoms;
		vector<int> atomtype;
		vector<vector<Real>> pos;
		vector<vector<Real>> distance;
		bool distflag;
		Real cell_x, cell_y, cell_z;
		Structure();
		int readstr(const char* filename);
		int readstr(string filename);
		int readlmp(const char* filename);
		int readvasp(const char* filename);
		void randomgenerate(Real diameter);
		void shuffle();
		void outputstr(const char* filename);
		void outputlmp(const char* filename);
		void outputvasp(const char* filename);
		void calculate_distance();
		void swap(int a, int b);
		void move(int a, Real x_move, Real y_move, Real z_move);
		void deform(Real x_inc, Real y_inc, Real z_inc);
};
#endif
