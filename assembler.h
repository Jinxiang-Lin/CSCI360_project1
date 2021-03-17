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
	string for_begin_handler(string* source, int loc, Funct &f);
	/*store c++ instructions*/
	void inputSource(const vector<string> newSource);
	/*print c++ instructions*/
	void printSource() const;
	/*check if source code is a leaf function*/
	bool check_leaf_funct() const;
	//int red_zone(); // do red_zoom and test arithmetic handler
 	vector<Var> vars_handler(string variable_string, int &address_offset);
 	vector<int> split(string str);
 	void test_var_handler(string str, int offset);
 	void function_handler();

 	/*this is the alternative for handling variables
 	  it'll read through the main.cpp and finds all variables
 	  assignment. this stores all variables information to variables_information vector
 	  and translates them to assembly.
 	*/
 	void variables_handler_versionTwo();
 	int find_main_start();
 	int find_main_end();
 	void print_variable_information();
private:
	vector<string>source;
	vector<Funct> functions;
	int address_offset = -4;
	/*variables section*/
	vector<string>assembly_instructions_for_variables;
	vector<vector<Var>> variables_information;

	//int red_zone_size = 0;
	/*rbp: base pointer during function calls
	  rsp: point to the curent top of the stack*/
};
#endif