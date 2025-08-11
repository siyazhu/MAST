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
	const char* flmp = "rand.lmp";
	int dohelp = 0;
	
	AskStruct options[]={
		{"","Convert Structure from LAMMPS to MAST, by Siya Zhu",TITLEVAL, NULL},
		{"-h", "Display more help", BOOLVAL, &dohelp},
		{"-str", "path and name for .str file (MAST format, Default: rand.str)", STRINGVAL, &fstr},
		{"-lmp", "path and name for .lmp file (LAMMPS format, Default: rand.lmp)", STRINGVAL, &flmp},
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
	
	if (struc.readlmp(flmp)==0){
		cerr << "\nError while reading str file!";
		return(0);
	}
	else{
		struc.outputstr(fstr);
	}
	return(1);
}
