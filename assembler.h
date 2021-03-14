#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "DataConverter.h"

#include<iostream>
#include<vector>
#include<regex>
using namespace std;

struct Var{
	string data_type = "int";
	string variables_name;
	int data_value = 0;
	int address_offset;
};

struct Parameter{
	string parameter_name;
	int value;
};

struct Funct{
	string function_name;
	string function_return_type;
	vector<Parameter> parameters;
	vector<Var> vars;
	vector<string> assembly_instructions;
};

class Assembler{
public:
	Assembler();
	string add_mov(string source, string destination, int size);
	int get_offset(string name, vector<Var> variables);
	void arithmetic_handler(string* source, int loc, Funct &f);
	void for_loop_handler(string* source, int loc);
	/*store c++ instructions*/
	void inputSource(const vector<string> newSource);
	/*print c++ instructions*/
	void printSource() const;
	/*check if source code is a leaf function*/
	bool check_leaf_funct() const;
 	vector<Var> vars_handler(string variable_string, int &address_offset);
 	vector<int> split(string str);
 	void test_var_handler(string str, int offset);
private:
	vector<string>source;
	vector<Funct> functions;
};
#endif