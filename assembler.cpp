#include "assembler.h"

Assembler::Assembler(){}

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
	data_type = variable_string.substr(variable_string.find("int"), variable_string.find("int")+3);

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
		for(int i = 0; i < array_size; i++){
			Var temp_var;
			temp_var.data_type = data_type;
			temp_var.variables_name = variable_name;

			temp_var.data_value = array_value[i];
			temp_var.address_offset = address_offset;
			address_offset += 4;
			variables.push_back(temp_var);
		}
	}
	//else variable_string is a single variable

	

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

