#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include "getvalue.h"
#include <numeric>
#include "mastfunc.h"
void smooth(vector<vector<Real>>& vec, int range, int times){
    while(times>0){
	int col = vec[0].size();
	int row = vec.size();
	vector<Real> zeros(range,0);
	int i, j, length;
       	length = 2* range +1;
	vector<Real>value;
	for (i=0;i<col;i++){
		value.clear();
		for (j=0;j<row;j++){
			value.push_back(vec[j][i]);
		}
		value.insert(value.begin(),zeros.begin(),zeros.end());
		value.insert(value.end(),zeros.begin(),zeros.end());
		vec[0][i]=0;
		for (j=range;j<length;j++){
			vec[0][i] += value[j];
		}
		for (j=0;j<row-1;j++){
			vec[j+1][i] = vec[j][i] - value[j] + value[j+length];
		}
		for (j=0;j<range;j++){
			vec[j][i] = vec[j][i] / (range + j + 1);
			vec[row - j-1][i] = vec[row - j-1][i] / (range + j + 1);
		}
		for (j=range;j<row-range;j++){
			vec[j][i] = vec[j][i] / length;
		}
	}	
	times--;
    }

}
/*
int main(){
	vector<vector<Real>> vec
	{
		{1,2},
		{1,4},
		{2,5},
		{3,9},
		{2,2}
	};
	smooth(vec,1,1);
	for (int i = 0;i<5;i++){
		for(int j = 0;j<2;j++){
			std::cout<<vec[i][j]<<"\t";
		}
		std::cout<<endl;
	}
	return 0;
}*/
