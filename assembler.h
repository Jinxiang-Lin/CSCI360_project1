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
private:
	vector<string> source = "";
	vector<string> assembly_codes = "";
	int add_offSet = 0;
};
#endif