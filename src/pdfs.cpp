#include "pdfs.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "mastfunc.h"
using namespace std;

Pdfs::Pdfs(){
	bars = 0;
	num_elements = 0;
	min = 0;
	step = 0;
	col = 0;
}

Pdfs::Pdfs(int bars, int num_elements, Real min, Real step){
	this->bars = bars;
	this->num_elements = num_elements;
	this->min = min;
	this->step = step;
	pdfhist.resize(bars);
	col = num_elements * (num_elements + 1) / 2;
	for (int i = 0;i<bars;i++){
		pdfhist[i].resize(col);
	}
}

void Pdfs::zeros(){
	for (int i = 0; i < bars; i++){
		for (int j = 0; j < col; j++){
			pdfhist[i][j] = 0;
		}
	}
}

int Pdfs::readfile(const char* filename){
	std::ifstream inputfile(filename);
	if (!inputfile) {
		std::cerr << "Read pdfs wrong, file not exist!";
		return(-1);
	}
	inputfile >> bars >> num_elements >> min >> step;
	col = num_elements * (num_elements + 1) / 2;
	pdfhist.resize(bars);
        for (int i = 0; i < bars; i++){
                pdfhist[i].resize(col);
        	for (int j = 0; j < col; j++){
			inputfile >> pdfhist[i][j];
		}
	}
	inputfile.close();
	return(0);
}

int Pdfs::readfile(string filename){
	const char* arr = filename.c_str();
	return(readfile(arr));
}
int Pdfs::get_type(int a, int b){
	if (a<=b){
		return(a*(num_elements*2-a+1)/2+b-a);
	}
	else{
		return(b*(num_elements*2-b+1)/2+a-b);
	}
}

void Pdfs::calculate_str(Structure str){
	//calculate Pdfs from a structure
	if (!str.distflag) str.calculate_distance();
	this->zeros();
	int i, j;
	int pos, type;
	for (i=0;i<str.num_total-1;i++)
		for (j=i+1;j<str.num_total;j++)
		{
			pos = floor((str.distance[i][j] - min)/step);
			type = get_type(str.atomtype[i],str.atomtype[j]);
			if ((pos < bars) && (pos>=0)){
				pdfhist[pos][type] = pdfhist[pos][type]+str.cell_x*str.cell_y*str.cell_z/str.num_atoms[str.atomtype[i]]/str.num_atoms[str.atomtype[j]];	
			}
		}
	for (i=0;i<bars;i++)
		for (j=0;j<col;j++){
			pdfhist[i][j] = pdfhist[i][j] / 4 / M_PI / (min+i*step+step/2.0) / (min+i*step+step/2.0)/step;
		}

	smooth(pdfhist,2,2);
}

Real Pdfs::calculate_error(Pdfs target){
	Real sum = 0;
	if ((bars != target.bars)||(num_elements!=target.num_elements)||(min!=target.min)||(step!=target.step)) {
		//parameters not match
		cerr<<"PDFs calculate_error wrong! Parameters not match!";
		return (-1);
	}
	for (int i = 0; i < bars; i++){
		for (int j = 0; j < col; j++){
		//	if ((target.pdfhist[i][j]==0)&&(pdfhist[i][j]>0)){
		//		return(100);
		//	}
			sum += (pdfhist[i][j]-target.pdfhist[i][j]) * (pdfhist[i][j] - target.pdfhist[i][j]) / (min+i*step+step/2.0)/(min+i*step+step/2.0);
		}
	}
	sum = sum * step;
	return(sum);
}

void Pdfs::output(const char* filename){
	std::ofstream outputfile;
	outputfile.open(filename);
	outputfile << bars << "\t" << num_elements << "\t" << min << "\t" << step << "\n";
	for (int i = 0; i < bars; i++){
                for (int j = 0; j < col; j++){
                	outputfile << pdfhist[i][j] << "\t";
		}
		outputfile << "\n";
        }
	outputfile.close();
}

void Pdfs::output(string filename){
	const char* arr = filename.c_str();
	output(arr);
}
