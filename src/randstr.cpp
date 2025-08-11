#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "structure.h"
#include <math.h>
#include "getvalue.h"
#include <numeric>
using namespace std;

int main(int argc, char* argv[]) {

	const char* fstr = "rand.str";
	Real diameter = 3.;
	int dohelp = 0;
	const char* elements;
	const char* elenum;	
	AskStruct options[]={
		{"","Random structure generate, by Siya Zhu",TITLEVAL, NULL},
		{"-h", "Display more help", BOOLVAL, &dohelp},
		{"-str", "path and name for the output .str file (MAST format, Default: rand.str)", STRINGVAL, &fstr},
		{"-d", "distance between the nearest grid points in angstrom. (Default: 3)", REALVAL, &diameter},
		{"-e", "Elements in the system. (Example: Cu,Zn)", STRINGVAL,&elements},
		{"-n", "Number of atoms of each type. (Example: 50,50)", STRINGVAL, &elenum},
	};
  	if (!get_values(argc,argv,countof(options),options)) {
		display_help(countof(options),options);
   		return 1;
	}
  	if (dohelp) {
    		cout << "NEED TO WRITE MORE HELP";
    		return 1;
  	}
	string elements_str = elements;
	string elenum_str = elenum;
	Structure struc;
 	int elements_pos = 0;
	string element_name;
	int atomicnum = 0;
        cout << "Elements:" << elements <<endl;
	cout << "Numbers:" << elenum << endl;
	cout << "Diameter:" << diameter << endl;	
	while (elements_pos != string::npos) {
                elements_pos = elements_str.find(",");
                element_name = elements_str.substr(0, elements_pos);
                atomicnum = elementnum(element_name);
                if (atomicnum == 0) {
                        cerr << "Element name illegal!";
                	return(0);
		}
                else {
                        struc.type.push_back(atomicnum);
                        struc.num_elements++;
                }
                elements_str.erase(0, elements_pos + 1);
        }
	
	int numpos = 0;
	string numtemp;
	int num_inttemp;
	while (numpos != string::npos) {
                numpos = elenum_str.find(",");
                numtemp = elenum_str.substr(0, numpos);
                num_inttemp = stoi(numtemp);
                if (num_inttemp <= 0) {
                        cerr << "Number of Atoms illegal!";
                        return(0);
                }
                else {
                  	struc.num_atoms.push_back(num_inttemp);
		}
                elenum_str.erase(0, numpos + 1);
        }
	if (struc.num_atoms.size() != struc.num_elements){
		cerr << "Number of elements not match!";
		return(0);
	}
	struc.num_total = accumulate(struc.num_atoms.begin(),struc.num_atoms.end(),0);
	cout << "Total atoms :" << struc.num_total <<endl;

	struc.randomgenerate(diameter);
	struc.shuffle();
	
	struc.outputstr(fstr);
	return(1);
}
