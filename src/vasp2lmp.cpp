#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "structure.h"
#include "getvalue.h"
using namespace std;

int main(int argc, char* argv[]) {

	const char* flmp = "rand.lmp";
	const char* fvasp = "rand.vasp";
	int dohelp = 0;
	
	AskStruct options[]={
		{"","Convert Structure from VASP to LAMMPS, by Siya Zhu",TITLEVAL, NULL},
		{"-h", "Display more help", BOOLVAL, &dohelp},
		{"-lmp", "path and name for .lmp file (LAMMPS format, Default: rand.lmp)", STRINGVAL, &flmp},
		{"-vasp", "path and name for .lmp file (VASP format, Default: rand.vasp)", STRINGVAL, &fvasp},
	};
  	if (!get_values(argc,argv,countof(options),options)) {
		display_help(countof(options),options);
   		return 1;
	}
  	if (dohelp) {
    		cout << "NEED TO WRITE MORE HELP";
    		return 1;
  	}

	Structure struc;
	
	if (struc.readvasp(fvasp)==0){
		cerr << "\nError while reading str file!";
		return(0);
	}
	else{
		struc.outputlmp(flmp);
	}
	return(1);
}
