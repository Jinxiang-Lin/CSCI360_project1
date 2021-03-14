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
	return 0;
	
}