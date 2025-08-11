#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
const char* strain[]={"n30","n25","n20","n15","n10","n5","n0","p5","p10","p15","p20","p25","p30"};
int main(int argc, char* argv[]) {
	int i;
	string command, pathname;
	const char* commandchar, * pathnamechar;
	double energy0, energy;
	energy = 0; energy0 = 0;
	ofstream outfile;
	ifstream energyfile;
	outfile.open("StrainEnergy");
	for (i=0;i<13;i++){
		command = strain[i];
		pathname = strain[i];
		command = "cd isotropic//" + command + " && extract_vasp";
		pathname = "isotropic//"+pathname+"//energy";
		commandchar = command.c_str();
		pathnamechar = pathname.c_str();
		system(commandchar);
		energyfile.open(pathnamechar);
		energyfile >> energy;
		energyfile.close();
		if (i==6) energy0 = energy;
		outfile << energy;
		if (i<12) {
			outfile << "\t";
		}	
		else{
		       	outfile <<"\n";
		}
	}
	
	for (i=0;i<13;i++){
                command = strain[i];
                pathname = strain[i];
		if (i==6){
			outfile << energy0 << "\t";
			continue;
		}
                command = "cd uniaxial//x//" + command + " && extract_vasp";
                pathname = "uniaxial//x//"+pathname+"//energy";
                commandchar = command.c_str();
                pathnamechar = pathname.c_str();
                system(commandchar);
                energyfile.open(pathnamechar);
                energyfile >> energy;
                energyfile.close();
                outfile << energy;
		if (i<12) {
                        outfile << "\t";
                }
                else{
                        outfile <<"\n";
                }
        }

	for (i=0;i<13;i++){
                command = strain[i];
                pathname = strain[i];
                if (i==6){
                        outfile << energy0 << "\t";
                        continue;
                }
                command = "cd uniaxial//y//" + command + " && extract_vasp";
                pathname = "uniaxial//y/"+pathname+"//energy";
                commandchar = command.c_str();
                pathnamechar = pathname.c_str();
                system(commandchar);
                energyfile.open(pathnamechar);
                energyfile >> energy;
                energyfile.close();
                outfile << energy;
                if (i<12) {
                        outfile << "\t";
                }
                else{
                        outfile <<"\n";
                }
        }

	for (i=0;i<13;i++){
                command = strain[i];
                pathname = strain[i];
                if (i==6){
                        outfile << energy0 << "\t";
                        continue;
                }
                command = "cd uniaxial//z/" + command + " && extract_vasp";
                pathname = "uniaxial//z/"+pathname+"//energy";
                commandchar = command.c_str();
                pathnamechar = pathname.c_str();
                system(commandchar);
                energyfile.open(pathnamechar);
                energyfile >> energy;
                energyfile.close();
                outfile << energy;
                if (i<12) {
                        outfile << "\t";
                }
                else{
                        outfile <<"\n";
                }
        }

	outfile.close();
	return(1);

}
