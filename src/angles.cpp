#include "angles.h"
#include <iostream>
#include <fstream>
#include "mastfunc.h"
#include <math.h>
using namespace std;

Angles::Angles(){
	bars = 0;
	num_elements = 0;
	col = 0;
}

Angles::Angles(int bars, int num_elements){
	this->bars = bars;
	this->num_elements = num_elements;
	anglehist.resize(bars);
	col = num_elements * num_elements * (num_elements + 1) / 2;
	for (int i = 0;i<bars;i++){
		anglehist[i].resize(col);
	}
}

void Angles::zeros(){
	for (int i = 0; i < bars; i++){
		for (int j = 0; j < col; j++){
			anglehist[i][j] = 0;
		}
	}
}

int Angles::readfile(const char* filename){
	std::ifstream inputfile(filename);
	if (!inputfile) {
		std::cerr << "Read angles wrong, file not exist!";
		return(-1);
	}
	inputfile >> bars >> num_elements;
	col = num_elements * num_elements * (num_elements + 1) / 2;
	anglehist.resize(bars);
        for (int i = 0; i < bars; i++){
                anglehist[i].resize(col);
        	for (int j = 0; j < col; j++){
			inputfile >> anglehist[i][j];
		}
	}
	inputfile.close();
	return(0);
}


int Angles::readfile(string filename){
	const char *arr = filename.c_str();
	return(readfile(arr));
}	
Real Angles::calculate_error(Angles target){
	Real sum = 0;
	if ((bars != target.bars)||(num_elements!=target.num_elements)) {
		//parameters not match
		cerr<<"Angles calculate_error wrong! Parameters not match!";
		return (-1);
	}
	for (int i = 0; i < bars; i++){
		for (int j = 0; j < col; j++){
			sum += (anglehist[i][j]-target.anglehist[i][j]) * (anglehist[i][j] - target.anglehist[i][j]);
		}
	}
	sum = sum * M_PI / bars;
	return(sum);
}

int Angles::get_type(int mid, int a, int b){
        if (a<=b){
                return(mid*(num_elements*(num_elements+1)/2) + a*(num_elements*2-a+1)/2+b-a);
        }
        else{
                return(mid*(num_elements*(num_elements+1)/2) +b*(num_elements*2-b+1)/2+a-b);
        }
}
void Angles::calculate_str(Structure struc, Real max_dis){
	int i, j, k;
	Real angle_temp;
	int angle_type, angle_value;
	int type1, type2, type3;
	Real temp_length1, temp_length2, temp_length3;
	Real inc;
	num_atoms = struc.num_total;
	zeros();
	struc.calculate_distance();
	inc = struc.cell_x * struc.cell_y * struc.cell_z *struc.num_total/M_PI*bars;
	vector<int> num_bonds(num_atoms,0);

	vector<vector<int>> bonding;	
	bonding.resize(num_atoms);

	for (i = 0; i < num_atoms-1; i++){
		for (j = i+1; j< num_atoms;j++){
			if (struc.distance[i][j]<=max_dis){
				num_bonds[i]++;
				num_bonds[j]++;
				bonding[i].push_back(j);
				bonding[j].push_back(i);
			}
		}
		if (num_atoms>10000){
                        cout<<"Bonding calculating, "<<i*100.0/num_atoms<<"%% done."<<endl;
                	cout.flush();
		}

	}
	for (i = 0; i < num_atoms; i++) {
		for (j = 0; j < num_bonds[i] - 1; j++) {
			for (k = j + 1; k < num_bonds[i]; k++) {
				temp_length1 = struc.distance[i][bonding[i][j]];
				temp_length2 = struc.distance[i][bonding[i][k]];
				temp_length3 = struc.distance[bonding[i][j]][bonding[i][k]];
				angle_temp = (temp_length1 * temp_length1 + temp_length2 * temp_length2 - temp_length3 * temp_length3) / 2 / temp_length1 / temp_length2;
				/*in case of numerical errors for acos*/
				if (angle_temp > 1) angle_temp = 1;
				if (angle_temp < -1) angle_temp = -1;

				angle_value = floor(acos(angle_temp) * bars / 3.1415927);
				angle_type = get_type(struc.atomtype[i], struc.atomtype[bonding[i][j]],struc.atomtype[bonding[i][k]]); 
				//cout<<"Angle_value = "<<angle_value<<"  Angle_type = " << angle_type<<endl;
				//cout.flush();
				anglehist[angle_value][angle_type] += inc / struc.num_atoms[struc.atomtype[i]]/ struc.num_atoms[struc.atomtype[bonding[i][j]]]/struc.num_atoms[struc.atomtype[bonding[i][k]]];
				}
			}
		if (num_atoms>10000){
			cout<<"Angles calculating, "<<(i+1)*100.0/num_atoms<<"%% done."<<endl;
			cout.flush();
		}
	}
	smooth(anglehist,2,2);
	
}



void Angles::output(const char* filename){
	std::ofstream outputfile;
	outputfile.open(filename);
	outputfile << bars << "\t" << num_elements << "\n";
	for (int i = 0; i < bars; i++){
                for (int j = 0; j < col; j++){
                	outputfile << anglehist[i][j] << "\t";
		}
		outputfile << "\n";
        }
	outputfile.close();
}
void Angles::output(string filename){
	const char* arr = filename.c_str();
	output(arr);
}
