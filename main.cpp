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
	Funct f; 
	assembler.for_begin_handler(4,f,1);
	assembler.for_begin_handler(7,f,2);
	assembler.print_assembly_instructions(f);
	//assembler.function_handler();
	
	return 0;
	
}