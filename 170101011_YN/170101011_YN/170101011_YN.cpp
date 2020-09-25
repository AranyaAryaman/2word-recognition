// 170101011_YN.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <conio.h>
#include <vector>
#include <cmath>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream file;
	string line,filename;
	cout<<"Enter the filename: ";
	getline(cin,filename);
	file.open(filename.c_str(),ios::in);
	if(!file)
		cout<<"Failed to open the file";
	int i=0;
	vector<long long int> samples;
	vector<long long int> energy;
	vector<long long int> zcr;
	if(file.is_open()){
		while(getline(file,line)){					// The first 5 lines are for configuration
			i++;
			if(i==6) break;
		}
		while(getline(file,line))
			samples.push_back(stof(line));
		i=0;
		double sum=0;
		for(int j=0;j<samples.size();j++){
			sum+=samples[j]*samples[j];
			i++;
			if(i==320){
				energy.push_back(sum/320.0);
				sum=0;
				i=0;
			}
		}
		sum=0;
		for(int j=0;j<samples.size()-1;j++){
			i++;
			if(samples[j]*samples[j+1]<0)
				sum++;
			if(i==320){
				zcr.push_back(sum);
				sum=0;
				i=0;
			}
		}
		
		/*for(int k=0;k<energy.size();k++){
			cout<<"Average energy of "<<k<<"th frame is: "<<energy[k]<<"\n";
			cout<<"ZCR of "<<k<<"th frame is: "<<zcr[k]<<"\n";
		}*/
		int noise_energy=0;
		int noise_zcr=0;
		for(int k=0;k<50;k++){
			noise_energy+=energy[k];
			noise_zcr+=zcr[k];
		}
		int threshold_energy=noise_energy/50;
		int threshold_zcr=noise_zcr*0.85;
		threshold_energy*=20;
		double start,stop;
		cout<<"Word    Start     Stop\n";
		for(int k=0;k<energy.size()-2;k++){
			if(energy[k]>threshold_energy && energy[k+1] >threshold_energy && energy[k+2] > threshold_energy){
				int tempzcr=0;
				int nof=1;
				start=k;
				while(energy[k+1] >threshold_energy && energy[k+2] > threshold_energy){
					tempzcr+=zcr[k];
					nof++;
					k++;
				}
				stop=k;
				tempzcr/=tempzcr/nof;
				if(zcr[k-1]>=tempzcr*5 || zcr[k-2]>=tempzcr*5 || zcr[k-3]>=tempzcr*5)
					cout<<"YES     "<<start/50.0<<"      "<<stop/50.0<<"\n";
				else
					cout<<"NO      "<<start/50.0<<"      "<<stop/50.0<<"\n";
			}
		}
	}
	return 0;

}

