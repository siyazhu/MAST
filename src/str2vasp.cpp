#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "structure.h"
#include "getvalue.h"
using namespace std;

int main(int argc, char* argv[]) {

	const char* fstr = "rand.str";
	const char* fvasp = "rand.vasp";
	int dohelp = 0;
	
	AskStruct options[]={
		{"","Convert Structure from MAST to VASP, by Siya Zhu",TITLEVAL, NULL},
		{"-h", "Display more help", BOOLVAL, &dohelp},
		{"-str", "path and name for .str file (MAST format, Default: rand.str)", STRINGVAL, &fstr},
		{"-vasp", "path and name for .vasp POSCAR file (POSCAR format, Default: rand.vasp)", STRINGVAL, &fvasp},
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
	
	if (struc.readstr(fstr)==0){
		cerr << "\nError while reading str file!";
		return(0);
	}
	else{
		struc.outputvasp(fvasp);
	}
	return(1);
}
