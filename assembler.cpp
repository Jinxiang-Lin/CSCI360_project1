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

/*check if a function is a leaf function.
  a function is a leaf function if it doesn't
  call any other function*/
bool Assembler::check_leaf_funct() const{
	regex function_check(".*[a-z]*=?[a-z]+\\(([a-z]+,?)+\\);");
	
	for(auto &x : source){
		if(regex_match(x, function_check))
		 	return 1;
	}
	return 0;
}

/*variable_handler: read a variable string; 
					check if the variable string is a variable or if it is a array; 
					extracts data from the string and stores into va 	
*/
vector<Var> Assembler::vars_handler(string variable_string, int &address_offset){
	vector<Var> variables;
	string data_type;

	//find data type int;
	//data_type = variable_string.substr(variable_string.find("int"), variable_string.find("int")+3);

	//if variable_string is an array
	if(variable_string.find('[') != string::npos){
		int array_size = 0;
		vector<int> array_value;
		string variable_name;

		array_size = stoi(variable_string.substr(variable_string.find('[')+1, variable_string.find(']')-2));
		array_value = split(variable_string.substr(variable_string.find('{'), variable_string.find('}')));

		int index = 0;
		index = variable_string.find("int")+4;
		variable_name = variable_string[index];

		address_offset *= array_size;
		int temp_offset = address_offset;
		for(int i = 0; i < array_size; i++){
			Var temp_var;
			//temp_var.data_type = data_type;
			temp_var.variables_name = variable_name;

			temp_var.data_value = array_value[i];
			temp_var.address_offset = address_offset;
			address_offset += 4;
			variables.push_back(temp_var);
		}
		address_offset = temp_offset -4;
	}
	//else variable_string is a single variable
	else{
		Var temp_var;

		//get the variable type
		//temp_var.data_type = data_type;

		//get the variable name
		string variable_name;
		int index = 0;
		index = variable_string.find("int")+4;
		variable_name = variable_string[index];
		temp_var.variables_name = variable_name;

		//get the value
		temp_var.data_value = stoi(variable_string.substr(variable_string.find('=')+1, variable_string.length()));

		//get the offset
		temp_var.address_offset = address_offset;
		variables.push_back(temp_var);

		address_offset-= 4;

	}
	

	return variables;
}

vector<int> Assembler::split(string str){
	//{11,2,30}
	vector<int> int_values;
	string temp = "";
	for(int i = 1; i < str.size(); i++){
		if(str[i] == ',' || str[i] == '}')
		{
			int_values.push_back(stoi(temp));
			temp = "";
		}
		else
			temp = temp + str[i];
	}
	return int_values;
}

//test function
void Assembler::test_var_handler(string str, int offset){
	vector<Var> test_var = vars_handler(str, offset);
	for(int i = 0; i < test_var.size(); i++){
		cout << " data_type is " << test_var[i].data_type << endl;
		cout << " variable name is " << test_var[i].variables_name << endl;
		cout << " value is " << test_var[i].data_value << endl;
		cout <<" address_offset is " << test_var[i].address_offset << endl;
		cout << endl;
	}
}

