#include "structure.h"
#include <string>
#include <fstream>
#include <vector>
#include <numeric>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;
Structure::Structure(){
	cell_x = 0;
	cell_y = 0;
	cell_z = 0;
	distflag = false;
	num_total = 0;
	num_elements = 0;
	vector<vector<Real>>().swap(pos);      // clear
        vector<int>().swap(atomtype);       // clear
	vector<int>().swap(num_atoms);		//clear
	vector<int>().swap(type);		//clear
}

int Structure::readstr(const char* filename){
        std::ifstream inputfile(filename);
        string elements;
	if (!inputfile) {
                cerr << "Read structure wrong, file not exist!";
                return(0);
        }
	
	if (!getline(inputfile, elements)) {
                cerr << "File .str is empty!";
                inputfile.close();
		return(0);
        };
        int atomicnum = 0;
        int elements_pos = 0;
        string element_name;
        num_elements = 0;
        while (elements_pos != string::npos) {
                elements_pos = elements.find(" ");
                element_name = elements.substr(0, elements_pos);
                atomicnum = elementnum(element_name);
                if (atomicnum == 0) {
                        cerr << "Element name illegal!";
                        inputfile.close();
			return 0;
                }
                else {
                        type.push_back(atomicnum);
			num_elements++;
                }
                elements.erase(0, elements_pos + 1);
	}
	inputfile >> cell_x >> cell_y >> cell_z;
	vector<Real> atom_temp(3);
	num_atoms = vector<int> (num_elements,0);
	int type_temp;
	string tempstring;
	vector<vector<Real>>().swap(pos);	// clear
	vector<int>().swap(atomtype);	// clear
	while (inputfile.peek()!=EOF){
		inputfile >> atom_temp[0] >> atom_temp[1] >> atom_temp[2] >> type_temp;
		getline(inputfile,tempstring);
		if ((type_temp < 0) || (type_temp >= num_elements)){
			cerr << "Atom type illegal!";
			inputfile.close();
			return(0);
		}
		atom_temp[0] += floor(atom_temp[0]);
                atom_temp[1] += floor(atom_temp[1]);
                atom_temp[2] += floor(atom_temp[2]);	//we make the fractional positions between 0 to 1
		pos.push_back(atom_temp);
		atomtype.push_back(type_temp);
		num_atoms[type_temp]++;
	}
	num_total = accumulate(num_atoms.begin(),num_atoms.end(),0);
	inputfile.close();
	return(1);
}

int Structure::readstr(string filename){
	const char* arr = filename.c_str();
	return(readstr(arr));
}
int Structure::readlmp(const char* filename){
	std::ifstream inputfile(filename);
        string elements;
        if (!inputfile) {
                cerr << "Read structure wrong, file not exist!";
                return(0);
        }
	
	int pointer = 0;
	string tempstring;
	getline(inputfile, tempstring);
	string element_name;
	num_elements = 0;
	vector<int>().swap(type);	//clear
	int atomicnum;
	while (pointer != string::npos) {
                pointer = tempstring.find(" ");
                element_name = tempstring.substr(0, pointer);
                atomicnum = elementnum(element_name);
                if (atomicnum == 0) {
                        cerr << "Element name illegal!";
                        inputfile.close();
			return 0;
                }
                else {
                        type.push_back(atomicnum);
                        num_elements++;
                }
                tempstring.erase(0, pointer + 1);
        }

	while (true){
                if (!getline(inputfile, tempstring)) {
                        cerr << "File .lmp format wrong!";
                        inputfile.close();
                        return(0);
                }
                pointer = tempstring.find("atoms");
                if (pointer != string::npos){
                        break;
                }
        }
	num_total = stoi(tempstring, nullptr);

	while (true){
                if (!getline(inputfile, tempstring)) {
                        cerr << "File .lmp format wrong!";
                        inputfile.close();
                        return(0);
                }
                pointer = tempstring.find("atom type");
                if (pointer != string::npos){
                        break;
                }
        }
        num_elements = stoi(tempstring, nullptr);

	while (true){
        	if (!getline(inputfile, tempstring)) {
                	cerr << "File .lmp format wrong!";
                	inputfile.close();
			return(0);
		}
		pointer = tempstring.find("xlo");
		if (pointer != string::npos){
			break;
		}
	}
	size_t szpointer;
	Real xlo = stof(tempstring, &szpointer);
	Real xhi = stof(tempstring.substr(szpointer));
	getline(inputfile, tempstring);
	Real ylo = stof(tempstring, &szpointer);
        Real yhi = stof(tempstring.substr(szpointer));
	getline(inputfile, tempstring);
        Real zlo = stof(tempstring, &szpointer);
        Real zhi = stof(tempstring.substr(szpointer));
	cell_x = xhi - xlo;
	cell_y = yhi - ylo;
	cell_z = zhi - zlo;

	while (tempstring.find("Atoms") == string::npos){
		if (!getline(inputfile, tempstring)) {
                        cerr << "File .lmp has no atoms!";
                        inputfile.close();
			return(0);
                }
	}
	vector<Real> atom_temp(3);
	int tempnum, type_temp;
	vector<vector<Real>>().swap(pos);      // clear
        vector<int>().swap(atomtype);       // clear
        num_atoms = vector<int>(num_elements,0);
	while ((inputfile.peek()!=EOF) && (pos.size()<num_total)){
                inputfile >> tempnum >> type_temp >> atom_temp[0] >> atom_temp[1] >> atom_temp[2] ;
                getline(inputfile, tempstring);
		if ((type_temp <= 0) || (type_temp > num_elements)){
                        cerr << "Atom type illegal!";
                        inputfile.close();
			return(0);
                }
                atom_temp[0] = (atom_temp[0]-xlo)/cell_x;
                atom_temp[1] = (atom_temp[1]-ylo)/cell_y;
                atom_temp[2] = (atom_temp[2]-zlo)/cell_z;    //we make the fractional positions between 0 to 1
                pos.push_back(atom_temp);
                atomtype.push_back(type_temp-1);
                num_atoms[type_temp-1]++;
        }
        if (pos.size()<num_total){
		cerr<<"Not enough atoms in .lmp file!";
		return(0);
	}
	
	inputfile.close();
        return(1);

}

int Structure::readvasp(const char* filename){
	std::ifstream inputfile(filename);
        string elements;
        if (!inputfile) {
                cerr << "Read structure wrong, file not exist!";
                return(0);
        }
	if (!getline(inputfile, elements)) {
                cerr << "File .str is empty!";
                inputfile.close();
		return(0);
        };
	Real latticeconstant;
	inputfile >> latticeconstant; 
	Real x1,x2,x3,y1,y2,y3,z1,z2,z3;
	inputfile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
	if ((y1!=0) || (z1!=0) || (x2!=0) || (z2!=0) || (x3!=0) || (y3!=0)){
		cerr << "MAST requires orthogonal cell!";
		inputfile.close();
		return(0);	
	}
	cell_x = latticeconstant * x1;
	cell_y = latticeconstant * y2;
	cell_z = latticeconstant * z3;
	
	int pointer = 0;
        string tempstring;
        getline(inputfile, tempstring);
        getline(inputfile, tempstring);
        string element_name;
        num_elements = 0;
        int atomicnum;
	vector<int>().swap(type);       //clear
        while (pointer != string::npos) {
                pointer = tempstring.find(" ");
                
		element_name = tempstring.substr(0, pointer);
                cout << "ELEMENT: " << element_name << endl;
		atomicnum = elementnum(element_name);
                if (atomicnum == 0) {
                        cerr << "Element name illegal!";
                        inputfile.close();
			return 0;
                }
                else {
                        type.push_back(atomicnum);
                        num_elements++;
                }
                tempstring.erase(0, pointer + 1);
        }
	int tempnum;
	vector<int>().swap(num_atoms);		//clear
	int i,j;
	for (i = 0; i < num_elements; i++){
		inputfile >> tempnum;
		num_atoms.push_back(tempnum);
	}
	vector<Real> atom_temp(3);
        vector<vector<Real>>().swap(pos);      // clear
        vector<int>().swap(atomtype);       // clear

	num_total = accumulate(num_atoms.begin(),num_atoms.end(),0);
	getline(inputfile, tempstring);
	if ((tempstring.front() == 'C')||(tempstring.front() == 'c')||(tempstring.front() == 'K')||(tempstring.front() == 'k')){
		//Cartesian coords used

		for (i = 0; i < num_elements; i++){
			for (j = 0; j < num_atoms[i]; j++){
				inputfile >> atom_temp[0] >> atom_temp[1] >> atom_temp[2];
				atom_temp[0] = atom_temp[0] * latticeconstant / cell_x;
                                atom_temp[1] = atom_temp[1] * latticeconstant / cell_y;
                                atom_temp[2] = atom_temp[2] * latticeconstant / cell_z;
				atom_temp[0] += floor(atom_temp[0]);
	 	               	atom_temp[1] += floor(atom_temp[1]);
                		atom_temp[2] += floor(atom_temp[2]);    //we make the fractional positions between 0 to 1
				pos.push_back(atom_temp);
				atomtype.push_back(i);
			}
		}
	}
	else{
		//Direct coords used
		for (i = 0; i < num_elements; i++){
                        for (j = 0; j < num_atoms[i]; j++){
                                inputfile >> atom_temp[0] >> atom_temp[1] >> atom_temp[2];
                                atom_temp[0] += floor(atom_temp[0]);
                                atom_temp[1] += floor(atom_temp[1]);
                                atom_temp[2] += floor(atom_temp[2]);    //we make the fractional positions between 0 to 1
                                pos.push_back(atom_temp);
                                atomtype.push_back(i);
                        }
                }
	}
	inputfile.close();
	return(1);
}

void Structure::outputstr(const char* filename){
	ofstream outputfile(filename);
	int i;
	for (i = 0; i < num_elements - 1; i++){
		outputfile << PERIODICTABLE[type[i]-1] <<" ";
	}
	outputfile << PERIODICTABLE[type[num_elements-1]-1] << "\n";
	outputfile << cell_x << " " << cell_y <<" " <<cell_z <<"\n";
	for (i = 0; i < num_total; i++){
		outputfile << pos[i][0] << " " << pos[i][1] << " " << pos[i][2] << " " << atomtype[i] << "\n";
	}
	outputfile.close();
}

void Structure::outputlmp(const char* filename){
	ofstream outputfile(filename);
        int i;
	for (i = 0; i < num_elements - 1; i++){
                outputfile << PERIODICTABLE[type[i]-1] <<" ";
        }
        outputfile << PERIODICTABLE[type[num_elements-1]-1] << "\n\n";
	outputfile << "\t" << num_total << " atoms\n";
	outputfile << "\t" << num_elements << " atom types\n";
	outputfile << "0 " << cell_x << " xlo xhi\n";
	outputfile << "0 " << cell_y << " ylo yhi\n";
	outputfile << "0 " << cell_z << " zlo zhi\n";
	outputfile << "\nAtoms\n\n";
        for (i = 0; i < num_total; i++){
                outputfile << i+1 << " " << atomtype[i]+1 <<" "<< pos[i][0]*cell_x << " " << pos[i][1]*cell_y << " " << pos[i][2]*cell_z << "\n";
        }
        outputfile.close();
}

void Structure::outputvasp(const char* filename){
	ofstream outputfile(filename);
        int i;
	for (i = 0; i < num_elements - 1; i++){
                outputfile << PERIODICTABLE[type[i]-1] <<" ";
        }
        outputfile << PERIODICTABLE[type[num_elements-1]-1] << "\n";
	outputfile << "1.0\n";
	outputfile << cell_x <<" 0 0\n0 " << cell_y << " 0\n0 0 " << cell_z <<"\n";
        for (i = 0; i < num_elements - 1; i++){
                outputfile << PERIODICTABLE[type[i]-1] <<" ";
        }
        outputfile << PERIODICTABLE[type[num_elements-1]-1] << "\n";
        for (i = 0; i < num_elements - 1; i++){
                outputfile << num_atoms[i] <<" ";
        }
        outputfile << num_atoms[num_elements-1] << "\n";
	outputfile << "Direct\n";
	int kind;
	for (kind = 0; kind < num_elements; kind++){

		for (i = 0; i < num_total; i++){
               		if (atomtype[i] == kind){
				outputfile << pos[i][0] << " " << pos[i][1] << " " << pos[i][2] << "\n";
			}
		}
	}
        outputfile.close();

}

void Structure::randomgenerate(Real diameter){
	int i, j, k, n_length, random_atom, count;
	n_length = ceil(pow(num_total/2.0, 1.0 / 3));

	cell_x = n_length * diameter;
	cell_y = n_length * diameter;
	cell_z = n_length * diameter;
	vector<int> rest;
	rest = num_atoms;
	count = 0;
	vector<Real> atomtemp(3);
	vector<vector<Real>>().swap(pos);      // clear
        vector<int>().swap(atomtype);       // clear
	for (i = 0; i < n_length; i++) {
		for (j = 0; j < n_length; j++) {
			if (count == num_total) break;
			for (k = 0; k < n_length; k++) {
				if (count == num_total) break;
                                do {
                                        random_atom = rand() % num_elements;
                                } while (rest[random_atom] == 0);
                                atomtemp[0] = i*1.0/n_length;
                                atomtemp[1] = j*1.0/n_length;
                                atomtemp[2] = k*1.0/n_length;
				pos.push_back(atomtemp);
                                atomtype.push_back(random_atom);
                                rest[random_atom]--;
                                count++;

				if (count == num_total) break;
				do {
					random_atom = rand() % num_elements;
				} while (rest[random_atom] == 0);
				atomtemp[0] = (i+0.5)/n_length;
				atomtemp[1] = (j+0.5)/n_length;
				atomtemp[2] = (k+0.5)/n_length;
				pos.push_back(atomtemp);
				atomtype.push_back(random_atom); 	
				rest[random_atom]--;
				count++;
			}
		}
	}
	distflag=false;
}

void Structure::shuffle(){
	int i,a,b;
	int shuffle_times=10;
	srand((unsigned)time(NULL));
	cout<<num_total<<endl;
	for (i=0;i<shuffle_times;i++){
		a=rand()%num_total;
		b=rand()%num_total;
		cout << "we swap  " << a << " & " <<b<<endl;
		swap(a,b);
	}
}
void Structure::calculate_distance(){
	if (distflag) return;		//already calculated
	int i, j;
	Real dis, dis1;
	int xt, yt, zt;
	Real sq_cell_x, sq_cell_y, sq_cell_z;
	sq_cell_x = cell_x * cell_x;
	sq_cell_y = cell_y * cell_y;
	sq_cell_z = cell_z * cell_z;
	distance = vector<vector<Real>>(num_total,vector<Real>(num_total,0));
	for (i = 0; i < num_total - 1; i++) {
		for (j = i+1; j < num_total; j++){
			dis = cell_x + cell_y + cell_z;
			for (xt = -1; xt < 1; xt++)
				for (yt = -1; yt < 1; yt++)
					for (zt = -1; zt < 1; zt++){
						dis1 = sqrt((pos[i][0] - pos[j][0] + xt) * (pos[i][0] - pos[j][0] + xt) * sq_cell_x + (pos[i][1] - pos[j][1] + yt) * (pos[i][1] - pos[j][1] + yt) * sq_cell_y + (pos[i][2] - pos[j][2] + zt) * (pos[i][2] - pos[j][2] + zt) * sq_cell_z);
						if (dis1 < dis) dis = dis1;
					}
			distance[i][j] = dis;
			distance[j][i] = dis;
		}
	}

	distflag = true;
}

void Structure::swap(int a, int b){
	int temptype;
	temptype = atomtype[a];
	atomtype[a] = atomtype[b];
	atomtype[b] = temptype;
}

void Structure::move(int a, Real x_move, Real y_move, Real z_move){
	pos[a][0] += x_move/cell_x;
	pos[a][1] += y_move/cell_y;
	pos[a][2] += z_move/cell_z;
	
	pos[a][0] = pos[a][0] - floor(pos[a][0]);
        pos[a][1] = pos[a][1] - floor(pos[a][1]);
        pos[a][2] = pos[a][2] - floor(pos[a][2]);

	distflag = false;
	


}

void Structure::deform(Real x_inc, Real y_inc, Real z_inc){
	cell_x += x_inc;
	cell_y += y_inc;
	cell_z += z_inc;
	distflag = false;
}



