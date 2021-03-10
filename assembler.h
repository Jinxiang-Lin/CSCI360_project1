#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include<iostream>
#include<vector>
#include<sstream>
using namespace std;

struct INT_registers
{
	int edi = 0;
	int esi = 0;
	int edx = 0;
	int ecx = 0;
	int r8d = 0;
	int r9d = 0;
};

struct Mapping
{
	int address_offset = 0;
	int value = 0;
};

class Assembler{
public:
	Assembler();
	void inputSource(const vector<string> newSource);
	void printSource() const;
	void set_var();
	/* reads txt file and finds the starting index and the ending index of main*/
	int get_main_starting_loc();
	int get_main_ending_loc();
	/*check if its an int declaration*/
	bool find_INT(int index);
	/*check if its an array*/
	bool find_arr(int index);
	/*check if its a func*/
	bool find_func(int index);
	/*get number from a line of string and push the value and it's offset to
	  vector<Mapping> var*/
	void getNumFromStr_set_var(string s);
	void print_var();

/* source vector stores the original c++ code;
   assembly_code stores the assembly code which is translated from source;
   address offset defaults to 0*/
private:
	vector<string> source;
	vector<string> assembly_codes;
	int add_offSet = 0;
	INT_registers parameters;
	vector<Mapping> var;
};
#endif