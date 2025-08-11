#ifndef PDFS_H
#define PDFS_H
#include <string>
#include <vector>
#include "misc.h"
#include "structure.h"
using namespace std;
class Pdfs{
        public:
                vector<vector<Real>> pdfhist;
                int num_elements;
                int bars;
		Real min;
		Real step;
                Pdfs();
                Pdfs(int bars, int num_elements, Real min, Real step);
                void zeros();
                int readfile(const char* filename);
                int readfile(string filename);
		void calculate_str(Structure str);
		Real calculate_error(Pdfs target);
                void output(const char* filename);
		void output(string filename);
        private:
                int col;
		int get_type(int a, int b);
};
#endif

