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


int Assembler::get_offset(string name, Funct &f){
	for (int i = f.vars.size()-1; i >=0; i--) {
		if(f.vars[i].variables_name == name){
			return f.vars[i].address_offset;
		}
	}
	return -1;
}

void Assembler::arithmetic_handler(int loc, Funct &f1){
	vector<string> temp; 
	// split by equal sign
	DataConverter::split(source[loc].substr(0, source[loc].length()-1), temp, '=');
	// get output name
	string output_name = temp.at(0);
	// get output offset
	int output_offset = get_offset(output_name, f1);

	string op1, op2, op_string;
	char operation;

	if(temp.at(1).find('+') != -1){
		operation = '+';
		op_string = "addl";
	}
	else if(temp.at(1).find('-') != -1){
		operation = '-';
		op_string = "subl";
	}
	else if(temp.at(1).find('*') != -1){
		operation = '*';
		op_string = "imul";
	}
	
	vector<string> operands;
	DataConverter::split(temp.at(1), operands, operation);
	op1 = operands.at(0);
	op2 = operands.at(1);

	int op1_offset = get_offset(op1, f1);
	
	string op_source1;
	if(op1_offset == -1){
		op_source1 = "$"+op1;
	}else{
		op_source1 = to_string(op1_offset)+"(%rbp)";
	}
	string dest1 = "eax";
	f1.assembly_instructions.push_back(add_mov(op_source1, dest1, 32)); 

	int op2_offset = get_offset(op2, f1);
	
	string op_source2;
	if(op2_offset == -1){
		//op_source2 = "$"+op2;
		
		int index_offset = -28;
		string index_offset_str = to_string(index_offset);
		f1.assembly_instructions.push_back("movl "+index_offset_str+"(%rbp),%edx");
		f1.assembly_instructions.push_back("cltq");
		string new_op2(1,op2[0]);
		int op2_arr_offset = get_offset(new_op2, f1);
		string offset_str = to_string(op2_arr_offset);
		op_source2 = offset_str + "(%rbp,rax,4)";
	}else{
		op_source2 = to_string(op2_offset)+"(%rbp)";
	}
	string dest2 = "edx";
	f1.assembly_instructions.push_back(add_mov(op_source2, dest2, 32)); 

	f1.assembly_instructions.push_back(op_string + " edx,eax");
	string op_source3 = "%eax";
	string dest3 = to_string(output_offset)+"(%rbp)";
	f1.assembly_instructions.push_back(add_mov(op_source3, dest3, 32));
}

string Assembler::for_begin_handler(int loc, Funct &f, int n){
	vector<string> temp;
	vector<string> temp1;
	vector<Var> variables;
	string iterate_string; 
/* 	variables = vars_handler("int i = 0",address_offset);
	for(auto &vars : variables)
		f.vars.push_back(vars); */
	// split by equal sign
	DataConverter::split(source[loc].substr(0, source[loc].length()-1), temp, '(');
	DataConverter::split(temp.at(1).substr(0, source[loc].length()-1), temp, ';');
	DataConverter::split(temp.at(2).substr(0, source[loc].length()-1), temp1, ')');
	//int i = 0; i < 4; i++)
	string temp_string;
	string if_string;
	string var_string;
	string var_string1;
	string comp_string;
	string s;
	/* temp 
	int i = 0
 	i < 4
 	i++) */
	/* temp1 
	i++ */
	//int i = 0
	//int j = i+1
	temp_string = temp.at(0);
	s = temp_string;
	var_string = temp_string.substr(4,1);	
	if(temp_string.size() > 9){
		var_string1 = temp_string.substr(8,1);
		cout << var_string1 << endl;
		for(auto & var : f.vars){
			if(var.variables_name == var_string1){
				int x = var.data_value + 1;
				string y = to_string(x);
				s = temp_string.substr(0,8) + y;
				string source = to_string(var.address_offset) + "(%rbp)";
				f.assembly_instructions.push_back(add_mov(source, "%eax", 32));
				f.assembly_instructions.push_back("addl %eax,1");
			}
		}
		variables = vars_handler(s,address_offset);
		for (int i = 0; i < variables.size(); i++){
			if(variables[i].variables_name == var_string){
				string z = "%eax";
				z += to_string(variables[i].address_offset);
				f.assembly_instructions.push_back(add_mov(z,"(%rbp)",32));
			}
		}
	}
	else{
		variables = vars_handler(s,address_offset);
		for (int i = 0; i < variables.size(); i++){
			if(variables[i].variables_name == var_string){
				string source = "$";
				source += to_string(variables[i].data_value);
				string dest = to_string(variables[i].address_offset) + "(%rbp)";
				f.assembly_instructions.push_back(add_mov(source,dest, 32));
			}
		}
	}
	//i < 4
	if_string = temp.at(1);
	comp_string = if_string.substr(4,if_string.length()-1);
	int comp = stoi(comp_string);
	comp --;
	comp_string = to_string(comp);
	string n_string = to_string(n);
	f.assembly_instructions.push_back(".BEGIN" + n_string);
	string dest = to_string(variables[0].address_offset)+"(%rbp)";
	f.assembly_instructions.push_back("cmpl $" + comp_string + "," +  dest);
	f.assembly_instructions.push_back("jg .END" + n_string);
	//i++
	iterate_string = temp1.at(0); 
	for(auto &vars : variables)
		f.vars.push_back(vars);
	return iterate_string;

}


void Assembler::if_statement_handler(string input, int loc, Funct &f){
	vector<string> temp;
	DataConverter::split(source[loc].substr(0, source[loc].length()-1), temp, '(');
	DataConverter::split(temp.at(1).substr(0, source[loc].length()-1), temp, '<');
	string temp1;
	int arr_value1;
	int arr_address_offset1;
	int arr_value2;
	int arr_address_offset2;
	int i = 0;
	while(temp.at(i) != ")"){
		temp1 += temp.at(i);
		i++;
	}
	for (auto & var : f.vars){
		if(var.variables_name == temp.at(2)){
			arr_value1 = var.data_value;
			arr_address_offset1 = var.address_offset;
		}
	}
	f.assembly_instructions.push_back(add_mov(arr_address_offset1 + "(%rbp", "%eax", 32));
	f.assembly_instructions.push_back("cltq");
	f.assembly_instructions.push_back(add_mov("0(%rbp,%rax,4)", "%eax", 32));
	for (auto & var : f.vars){
		if(var.variables_name == temp.at(9)){
			arr_value2 = var.data_value;
			arr_address_offset2 = var.address_offset;
		}
	}
	f.assembly_instructions.push_back(add_mov(arr_address_offset2 + "(%rbp", "%eax", 32));
	f.assembly_instructions.push_back("cltq");
	f.assembly_instructions.push_back(add_mov("0(%rbp,%rax,4)", "%eax", 32));
	f.assembly_instructions.push_back("cmpl " + 'edx, eax');
	f.assembly_instructions.push_back("jge .IF");
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
		 	return 0;
	}
	return 1;
}

/*variable_handler: read a variable string; 
					check if the variable string is a variable or if it is a array; 
					extracts data from the string and stores into va 	
*/


vector<Var> Assembler::vars_handler(string variable_string, int &address_offset){
	vector<Var> variables;
	//string data_type;

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

		//address_offset *= array_size;
		//int temp_offset = address_offset;

		//red_zone_size += array_size;
		for(int i = 0; i < array_size; i++){
			Var temp_var;
			//temp_var.data_type = data_type;
			temp_var.variables_name = variable_name;

			temp_var.data_value = array_value[i];
			temp_var.address_offset = address_offset;
			address_offset -= 4;
			variables.push_back(temp_var);
		}
	}
	//else variable_string is a single variable
	else{
		//red_zone_size++;
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

void Assembler::function_handler(){
	/*section 1: handling variables and function*/
	Funct f;
	variables_handler_versionTwo(f);
	/*main function*/
	f.assembly_instructions.push_back("main:");
	f.assembly_instructions.push_back("pushq %rbp");
	f.assembly_instructions.push_back("movq  %rsp, %rbp");

	if(check_leaf_funct() == 0) { f.assembly_instructions.push_back("subq  $48, %rbp"); }

	/*all variables assignment to assembly*/
	for(int i = 0; i < f.vars_storage.size(); i++){
		for(int j = 0; j < f.vars_storage[i].size(); j++){
			string source = to_string(f.vars_storage[i][j].data_value);
			source = " $"+source;
			string dest = to_string(f.vars_storage[i][j].address_offset) + "(%rbp)";
			f.assembly_instructions.push_back(add_mov(source, dest, 32));
			f.vars.push_back(f.vars_storage[i][j]);
		}
	}

	//check if main calls another function
	if(check_leaf_funct() == 0){
		string function_head = source[0];
		string return_type = function_head.substr(0, function_head.find(' '));
		string temp = function_head.substr(function_head.find(' ')+1, function_head.length());
		string function_name = temp.substr(0, temp.find('('));
		//cout << return_type << " " << function_name << endl;
		int main_start = find_main_start();
		int main_end = find_main_end();
		int function_call_index;

		for(int i = main_start; i < main_end; i++){
			if(source[i].find(function_name)){
				function_call_index = i;
			}
		}
		string parameters_str = source[0].substr(source[0].find('(')+1, source[0].find(')')-source[0].find('(')-1);
		//cout << parameters_str << endl;

		vector<string>parameters;
		DataConverter::split(parameters_str, parameters, ',');
		if(parameters.size() > 6){
			string argument[6] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
			int offset;

			//push the first 6 to arguments;
			for(int i = 0; i < 6; i++){

				if(parameters[i].length() == 5){
					//string name1 = "a";
					string name(1, parameters[i][4]);
					offset = get_offset(name,f);

					Parameter p;
					p.variables_name = name;
					p.argument_name = argument[i];
					p.address_offset = offset;
					f.parameters.push_back(p);

					string offset_str = to_string(offset);
					string temp = "movl  ";
					temp.append(offset_str);
					temp.append("(%rbp), ");
					temp.append("%");
					temp.append(argument[i]);
					f.assembly_instructions.push_back(temp);

				}
				else{
					string name(1, parameters[i][4]);
					offset = get_offset(name,f);

					Parameter p;
					p.variables_name = name;
					p.argument_name = argument[i];
					p.address_offset = offset;
					f.parameters.push_back(p);

					string offset_str = to_string(offset);
					string temp = "levl  ";
					temp.append(offset_str);
					temp.append("(%rbp), ");
					temp.append("%");
					temp.append(argument[i]);
					f.assembly_instructions.push_back(temp);
				}
			}
			//push the rest of arguments
			for(int i = 6; i < parameters.size(); i++){
				if(parameters[i].length() == 5){
					//string name1 = "a";
					string name(1, parameters[i][4]);
					offset = get_offset(name,f);

					Parameter p;
					p.variables_name = name;
					p.argument_name = "";
					p.address_offset = offset;
					f.rest_p.push_back(p);

					string offset_str = to_string(offset);
					string temp = "pushq  ";
					temp.append(offset_str);
					temp.append("(%rbp)");
					f.assembly_instructions.push_back(temp);

				}
				else{
					string name(1, parameters[i][4]);
					offset = get_offset(name,f);

					Parameter p;
					p.variables_name = name;
					p.argument_name = "";
					p.address_offset = offset;
					f.rest_p.push_back(p);

					string offset_str = to_string(offset);
					string temp = "pushq  ";
					temp.append(offset_str);
					temp.append("(%rbp)");
					
					f.assembly_instructions.push_back(temp);
				}				
			}

		}
		f.assembly_instructions.push_back("call   " + function_name);
		f.assembly_instructions.push_back("add   rsp, 16");
		f.assembly_instructions.push_back("movl  eax, -48(%rbp)");
		f.assembly_instructions.push_back("movl  $0, %eax");
		f.assembly_instructions.push_back("leave");
		f.assembly_instructions.push_back("ret");
		
		//test function start here;
		
		f.assembly_instructions.push_back(function_name+":");
		f.assembly_instructions.push_back("pushq  rbp");
		f.assembly_instructions.push_back("movq  %rsp, %rbp");
		int loc = 2;
		int add_offset2 = -4;
		for(int i = 0; i < f.parameters.size(); i++){
			string argument = "%";
			argument += f.parameters[i].argument_name;
			string address_offset =to_string(add_offset2);
			address_offset +="(%rbp)";
			f.assembly_instructions.push_back("movl  "+argument+ " " +address_offset);
			add_offset2 -=4;
		}

		while(source[loc] != "}"){
			if(source[loc].find("int") == 0){
				vector<Var> vars = vars_handler(source[loc], add_offset2);
				for(auto & var : vars){
					f.vars.push_back(var);
					string source = "$";
					source+= to_string(var.data_value);
					string dest = to_string(var.address_offset)+"(%rbp)";
					f.assembly_instructions.push_back(add_mov(source, dest, 32));
				}
				loc++;
			}
			else if(source[loc].find("return") == 0){
				f.assembly_instructions.push_back("leave");
				f.assembly_instructions.push_back("ret");
				loc++;
			}
			else{
				arithmetic_handler(loc, f);
				loc++;
			}

		}

	}
	/*
	for(int i = 0; i < f.parameters.size(); i++){
		cout << f.parameters[i].variables_name << endl;
		cout << f.parameters[i].argument_name << endl;
		cout << f.parameters[i].address_offset << endl;
	}*/

	print_assembly_instructions(f);
	//print_variable_information(f);
	//print_vars(f);


	/*section 2: handling loop and condition*/

}

void Assembler::print_assembly_instructions(Funct &funct){
	for(auto x : funct.assembly_instructions)
		cout << x << endl;
}
/*
int Assembler::red_zone(){
	return red_zone_size*4;
}*/
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

//alternative version start here
void Assembler::variables_handler_versionTwo(Funct &funct){

	int main_start = find_main_start();
	int main_end = find_main_end();
	vector<string> variables_str;
	for(int i = main_start; i < main_end; i++){
		if(source[i].find("int") == 0){
			variables_str.push_back(source[i]);
		}
	}


	vector<Var>temp;
	for(int i = 0; i < variables_str.size(); i++){
		temp = vars_handler(variables_str[i], address_offset);
		//variables_information.push_back(temp);
		funct.vars_storage.push_back(temp);
	}
	
}
void Assembler::print_variable_information(Funct &funct){
	for(int i =0; i < funct.vars_storage.size(); i++){
		for(int j = 0; j < funct.vars_storage[i].size(); j++){
			cout << "data type is "<<funct.vars_storage[i][j].data_type << endl;
			cout << "variable_name is "<<funct.vars_storage[i][j].variables_name<<endl;
			cout << "value is "<<funct.vars_storage[i][j].data_value << endl;
			cout << "address_offset is" << funct.vars_storage[i][j].address_offset<<endl;
			cout << endl;
		}
	}
}

void Assembler::print_vars(Funct &funct){
	for(int j = 0; j < funct.vars.size(); j++){
			cout << "data type is "<<funct.vars[j].data_type << endl;
			cout << "variable_name is "<<funct.vars[j].variables_name<<endl;
			cout << "value is "<<funct.vars[j].data_value << endl;
			cout << "address_offset is" << funct.vars[j].address_offset<<endl;
			cout << endl;
		}
}
int Assembler::find_main_start(){
	int main_start = 0;
	for(int i = 0; i < source.size(); i++){
		if(source[i] == "int main()"){
			main_start = i+2;
			break;
		}
		
	}
	return main_start;
}

int Assembler::find_main_end(){
	int main_start = find_main_start();
	int main_end;
	for(int i = main_start; i < source.size(); i++){
		if(source[i] == "}"){
			main_end = i;
			break;
		}
	}
	return main_end;
}
// alternative version end here;

