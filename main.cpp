#include <iostream>
#include <fstream>
#include <vector>
#include "assembler.h"

using namespace std;

int main(){


	string inputFile;
	cout << "Enter the file name: ";
	cin >> inputFile;

	ifstream readFile(inputFile);

	string readLine;
	vector<string> source;


// store source from inputfile line by line to a vector;
	while(getline(readFile, readLine)){

		source.push_back(readLine);
	}

	Assembler assembler;
	assembler.inputSource(source);
	assembler.printSource();
	assembler.variables_handler_versionTwo();
	
/*
	//here is only for testing function
	int add_off = -4;
	string str = "    int i[3] = {11,2,3};";
	assembler.test_var_handler(str, add_off);
*/	

	return 0;
	
}