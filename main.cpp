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

	/*read and push all int vars into vector<Mapping> var
	  and store the int value to var.value and address offSet into var.address_offSet*/
	assembler.set_var();
	assembler.print_var();



	return 0;
	
}