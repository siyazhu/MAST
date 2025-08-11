#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "structure.h"
#include <math.h>
#include "getvalue.h"
#include <numeric>
#include "pdfs.h"
#include "angles.h"
#include <time.h>
using namespace std;

bool Accept(Real error1, Real error2, Real temp){
	if ((double)rand()/(RAND_MAX) < exp ((error1-error2)/temp)){
			return(true);
			}
			else{
			return(false);
			}
}

int main(int argc, char* argv[]) {

	const char* frmcin = "rmc.in";
	int dohelp = 0;
	AskStruct options[]={
		{"","Reverse Monte Carlo program, by Siya Zhu",TITLEVAL, NULL},
		{"-h", "Display more help", BOOLVAL, &dohelp},
		{"-in", "path and name for the input file (Default: rmc.in)", STRINGVAL, &frmcin},
	};
  	if (!get_values(argc,argv,countof(options),options)) {
		display_help(countof(options),options);
   		return 1;
	}
  	if (dohelp) {
    		cout << "NEED TO WRITE MORE HELP";
    		return 1;
  	}

	std::ifstream inputfile(frmcin);
        if (!inputfile) {
                cerr << "Read inputfile wrong, file not exist!";
                return(0);
        }
	string strin;
        if (!getline(inputfile, strin)) {
                cerr << "RMC inputfile is empty!";
                inputfile.close();
                return(0);
        };
	
	bool USE_CURRENT = false;
	if (strin == "USE_CURRENT") USE_CURRENT = true;
	
	Pdfs pdfs_target;
	Angles angles_target;
	//Voroneighbors voroneighbors_target;
	
	Structure struc, struc_target;
	Pdfs pdfs;
	Angles angles;
	//Voroneighbors voroneighbors;
	 Real angles_max_dis;
                int angles_bars;
	if (!USE_CURRENT){
		std::ifstream inputfstrin(strin);
		if(struc_target.readstr(strin)==0){
			cerr << "Error while reading structure file!\n";
			inputfile.close();
			return(0);
		}
	}

		
	if (!getline(inputfile, strin)) {
                cerr << "Need current structure file!\n";
                inputfile.close();
                return(0);
        };

	if(struc.readstr(strin)==0){
                cerr << "Error while reading current structure file!\n";
                inputfile.close();
                return(0);
        }
	
	Real weight_pdfs, weight_angles, weight_voroneighbors;
	string tempstr;
        getline(inputfile, tempstr);
        if (tempstr!="[PDFS]"){
                cerr << "RMC inputfile format error!\n";
                inputfile.close();
                return(0);
        }
	
	inputfile >> weight_pdfs;
	getline(inputfile, tempstr);
	if (weight_pdfs < 0) {
		cerr << "Weight cannot be smaller than 0.\n";
		inputfile.close();
		return(0);
	}

	if (weight_pdfs > 0) {
		// we use pdfs in error function
		if (!USE_CURRENT){
			int bars;
			Real min, step;
			inputfile >> bars >> min >> step;
			getline(inputfile, tempstr);
			pdfs_target = Pdfs(bars,struc_target.num_elements,min,step);
			pdfs_target.calculate_str(struc_target);
			struc_target.outputvasp("Target_VASP.out");
			pdfs_target.output("Target_PDFS.out");
		}
		else{
			getline(inputfile, tempstr);
			if (pdfs_target.readfile(tempstr)==0){
				cerr<<"Error while reading PDFS file! \n";
				inputfile.close();
				return(0);
			}
		}
		pdfs = Pdfs(pdfs_target.bars, pdfs_target.num_elements, pdfs_target.min, pdfs_target.step);
	}
	
	getline(inputfile, tempstr);
        if (tempstr!="[ANGLES]"){
                cerr << "RMC inputfile format error!\n";
                inputfile.close();
                return(0);
        }
	
	inputfile >> weight_angles;
        getline(inputfile, tempstr);
        if (weight_angles < 0) {
                cerr << "Weight cannot be smaller than 0.\n";
                inputfile.close();
                return(0);
        }

        if (weight_angles > 0) {
                // we use angles in error function
		if (!USE_CURRENT){
                        inputfile >> angles_bars >> angles_max_dis;
                        getline(inputfile, tempstr);
                        angles_target = Angles(angles_bars,struc_target.num_elements);
			cout << "ready to calculate angles"<<endl;
			cout.flush();
			angles_target.calculate_str(struc_target, angles_max_dis);
			angles_target.output("Target_Angles.out");
                	cout<<"Target Angles calculated."<<endl;
			cout.flush();
		}
                else{
                        inputfile >> angles_bars >> angles_max_dis;
			getline(inputfile, tempstr);
			getline(inputfile, tempstr);
                        if (angles_target.readfile(tempstr)==0){
                                cerr<<"Error while reading ANGLES! \n";
                                inputfile.close();
                                return(0);
			}
			
		}
		angles = Angles(angles_target.bars,angles_target.num_elements);
	}
 	cout<<"RMC Process"<<endl;
	cout.flush();	
	int iterations, MAX_ITER;
	srand((unsigned)time(NULL));
	Real temp_factor = 100; 
	inputfile >> MAX_ITER >> temp_factor;
	inputfile.close();
	Real step = 0.1;
	int P_swap, P_move, P_reshape;
	iterations = 0;
	P_swap = 30;
	P_move = 70;
	P_reshape = 0;
	Real pdfs_error, angles_error, error_current, error_new;
	int atom_a, atom_b, choice;
	Real move_x, move_y, move_z;
	ifstream flagfile;
	ofstream outfile;
	cout<<"START RMC"<<endl;
	pdfs.calculate_str(struc);
        angles.calculate_str(struc, angles_max_dis);
        pdfs_error = pdfs.calculate_error(pdfs_target);
        angles_error = angles.calculate_error(angles_target);
	error_current = weight_pdfs * pdfs_error + weight_angles *angles_error;
	while (iterations<=MAX_ITER){
		flagfile.open("WORKING",std::ifstream::in);
		while (flagfile){
			flagfile.close();
			flagfile.open("WORKING",std::ifstream::in);
		}
	//	flagfile.open("Iterations",std::ifstream::in);
	//	flagfile >> iterations;
	//	cout <<"Iteration " << iterations <<"\n";
		flagfile.close();
		
		//flagfile.open("Struc.out",std::ifstream::in);
		//if (flagfile){
		//	struc.readstr("Struc.out");
		//}

/*		flagfile.open("Errorvalue",std::ifstream::in);
		if (!flagfile){
			pdfs.calculate_str(struc);
//			angles.calculate_str(struc);
			pdfs.output("Pdfs.out");
//			angles.output("Angles.out");
			error_current = weight_pdfs * pdfs.calculate_error(pdfs_target);
//			error_current = weight_pdfs * pdfs.calculate_error(pdfs_target) + weight_angles * angles.calculate_error(angles_target);
			outfile.open("Errorvalue", std::ofstream::out);
			outfile << error_current;
			outfile.close();
		}
		else{
			flagfile >> error_current;
			flagfile.close();
		}
*/				
		choice = rand()%(P_swap+P_move+P_reshape);
		if (choice < P_swap){
			//we do swap
			atom_a = rand()%struc.num_total;
			atom_b = rand()%struc.num_total;
			while(struc.atomtype[atom_a] == struc.atomtype[atom_b]){
				atom_a = rand()%struc.num_total;
	                        atom_b = rand()%struc.num_total;
			}
			struc.swap(atom_a, atom_b);
			pdfs.calculate_str(struc);
			angles.calculate_str(struc, angles_max_dis);
//error_new = weight_pdfs * pdfs.calculate_error(pdfs_target);
//cout<<"ERROR_OLD = " << error_current <<"    ERROR_NEW = "<<error_new<<endl;
			pdfs_error = pdfs.calculate_error(pdfs_target);
			angles_error = angles.calculate_error(angles_target);
			error_new = weight_pdfs * pdfs_error + weight_angles *angles_error;		
			if (Accept(error_current, error_new, temp_factor)){
				outfile.open("WORKING", std::ofstream::out);
				outfile << "WORKING";
				outfile.close();
			 	cout <<"Iteration " << iterations <<"\n";
				cout <<"SWAP ACCEPTED, error = " << error_new << "\n";	
				cout <<"PDFS error * weight = " <<  pdfs_error*weight_pdfs<<endl;
				cout <<"Angles error * weight = " << angles_error*weight_angles<<endl;
				pdfs.output("Pdfs.out");
				angles.output("Angles.out");
				outfile.open("Errorvalue", std::ofstream::out);
	                        outfile << error_current;
        	                outfile.close();
				struc.outputstr("Struc.out");
				outfile.open("Iterations", std::ofstream::out);
				iterations++;
				outfile <<iterations;
				outfile.close();
				remove("WORKING");
				error_current = error_new;
			}
			else{
				struc.swap(atom_a,atom_b);
			}

		}
		else{
			if (choice < P_swap+P_move){
				//we do move
				step=0.1;
				atom_a = rand()%struc.num_total;
                        	move_x = ((double) rand() / RAND_MAX *2 -1)*step;
				move_y = ((double) rand() / RAND_MAX *2 -1)*step;
				move_z = ((double) rand() / RAND_MAX *2 -1)*step;
				struc.move(atom_a, move_x, move_y, move_z);
				struc.outputstr("Struc.out");
				pdfs.calculate_str(struc);
                        	angles.calculate_str(struc, angles_max_dis);
//error_new = weight_pdfs * pdfs.calculate_error(pdfs_target);
				pdfs_error = pdfs.calculate_error(pdfs_target);
	                        angles_error = angles.calculate_error(angles_target);
        	                error_new = weight_pdfs * pdfs_error + weight_angles *angles_error;
				if (Accept(error_current, error_new, temp_factor)){
                                	outfile.open("WORKING", std::ofstream::out);
                                	outfile << "WORKING";
                               	 	outfile.close();
					cout <<"Iteration " << iterations <<"\n";
					cout <<"MOVE ACCEPTED, error = " << error_new << "\n";
					cout <<"PDFS error * weight = " <<  pdfs_error*weight_pdfs<<endl;
                               		cout <<"Angles error * weight = " << angles_error*weight_angles<<endl;

                                	pdfs.output("Pdfs.out");
                                	angles.output("Angles.out");
                                	outfile.open("Errorvalue", std::ofstream::out);
                                	outfile << error_current;
                                	outfile.close();
                                	struc.outputstr("Struc.out");
					outfile.open("Iterations", std::ofstream::out);
                                	iterations++;
					outfile <<iterations;
                                	outfile.close();
					error_current = error_new;
                                	remove("WORKING");

                        	}
                        	else{
                                	struc.move(atom_a,-move_x, -move_y, -move_z);
					
				}
			}
			else{
				//we do reshape
                                step = 0.1;
				move_x = ((double) rand() / RAND_MAX *2 -1)*step;
                                struc.deform(move_x, move_x, move_x);
                                struc.outputstr("Struc.out");
                                pdfs.calculate_str(struc);
//                              angles.calculate_str(struc);
error_new = weight_pdfs * pdfs.calculate_error(pdfs_target);
//                              error_new = weight_pdfs * pdfs.calculate_error(pdfs_target) + weight_angles * angles.calculate_error(angles_target);
                                cout.flush();
                                if (Accept(error_current, error_new, temp_factor)){
                                        outfile.open("WORKING", std::ofstream::out);
                                        outfile << "WORKING";
                                        outfile.close();
                                        cout <<"MOVE ACCEPTED, error = " << error_new << "\n";

                                        pdfs.output("Pdfs.out");
//                                      angles.output("Angles.out");
                                        outfile.open("Errorvalue", std::ofstream::out);
                                        outfile << error_current;
					outfile.close();
                                        struc.outputstr("Struc.out");
                                        outfile.open("Iterations", std::ofstream::out);
                                        iterations++;
                                        outfile <<iterations;
                                        outfile.close();
                                        error_current = error_new;
                                        remove("WORKING");

                                }
                                else{
                                        struc.deform(-move_x, -move_x, -move_x);

                                }

			
			}


		}
			

	}
	cout.flush();
	return(1);
}
