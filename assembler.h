#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include<iostream>
#include<vector>
using namespace std;

class Assembler{
public:
	Assembler();
	void inputSource(const vector<string> newSource);

	void printSource() const;

/* source vector stores the original c++ code;
   assembly_code stores the assembly code which is translated from source;
   address offset defaults to 0*/
private:
	vector<string> source = "";
	vector<string> assembly_codes = "";
	int add_offSet = 0;
};
#endif