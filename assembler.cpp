#include "assembler.h"

Assembler::Assembler(){}

string Assembler::add_mov(string source, string dest, int size){
    string opcode;
	if(size == 64){
		opcode = "movq"; 
	}else if(size == 32){
		opcode = "movl";
	}
	return opcode + " " + source + ","+ dest;
} 

int Assembler::get_offset(string name, vector<Var> variable){
}

void Assembler::arithmetic_handler(string* source, int loc, Funct &f1){
	vector<string> temp; 
	// split by equal sign
	DataConverter::split(source[loc].substr(0, source[loc].length()-1), temp, '=');
	// get output name
	string output_name = temp.at(0);
	// get output offset
	int output_offset = get_offset(output_name, f1.variables);

	string op1, op2, op_string;
	char operation;

	if(temp.at(1).find('+') != -1){
		operation = '+';
		op_string = "addl";
	}else if(temp.at(1).find('-') != -1){
		operation = '-';
		op_string = "subl";
	}else if(temp.at(1).find('*') != -1){
		operation = '*';
		op_string = "imul";
	}
	
	vector<string> operands;
	DataConverter::split(temp.at(1), operands, operation);
	op1 = operands.at(0);
	op2 = operands.at(1);

	int op1_offset = get_offset(op1, f1.variables);
	string op_source1;
	if(op1_offset == -1){
		op_source1 = "$"+op1;
	}else{
		op_source1 = to_string(op1_offset)+"(%rbp)";
	}
	string dest1 = "eax";
	f1.assembly_instructions.push_back(add_mov(op_source1, dest1, 32)); 

	int op2_offset = get_offset(op2, f1.variables);
	string op_source2;
	if(op2_offset == -1){
		op_source2 = "$"+op2;
	}else{
		op_source2 = to_string(op2_offset)+"(%rbp)";
	}
	string dest2 = "edx";
	f1.assembly_instructions.push_back(add_mov(op_source1, dest1, 32)); 

	f1.assembly_instructions.push_back(op_string + " edx,eax");
	string op_source3 = "%eax";
	string dest3 = to_string(output_offset)+"(%rbp)";
	f1.assembly_instructions.push_back(add_mov(op_source3, dest3, 32));
}

void for_loop_handler(string* source, int loc){

}
/*store c++ instructions*/
void Assembler::inputSource(const vector<string> newSource)
{
	for(int i = 0; i < newSource.size(); i++)
	{
		source.push_back(newSource[i]);
	}
}

/*print c++ instructions*/
void Assembler::printSource() const{
	for(int i = 0; i < source.size(); i++)
	{
		cout << source[i] << endl;
	}
}
