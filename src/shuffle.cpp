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
	int dohelp = 0;
	AskStruct options[]={
		{"","Randomly shuffle structure, by Siya Zhu",TITLEVAL, NULL},
		{"-h", "Display more help", BOOLVAL, &dohelp},
		{"-str", "path and name for the output .str file (MAST format, Default: rand.str)", STRINGVAL, &fstr},
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
	struc.readstr(fstr);	
	struc.shuffle();
	
	struc.outputstr(fstr);
	return(1);
}
