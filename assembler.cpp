#include "assembler.h"

Assembler::Assembler(){}

void Assembler::inputSource(const vector<string> newSource)
{
	for(int i = 0; i < newSource.size(); i++)
	{
		source.push_back(newSource[i]);
	}
}

void Assembler::printSource() const{
	for(int i = 0; i < source.size(); i++)
	{
		cout << source[i] << endl;
	}
}

int Assembler::get_main_starting_loc(){
	int loc = 0;
	for(int i = 0; i < source.size(); i++){
		if(source[i] == "int main()"){
			loc = i+2;
			break;
		}
	}
	return loc;
}

int Assembler::get_main_ending_loc(){
	int start = get_main_starting_loc();
	int end = 0;
	for(int i = start; i < source.size(); i++){
		if(source[i] == "}"){
			end = i;
		}
	}
	return end;
}

bool Assembler::find_INT(int index){
	if(source[index].find("int") != string::npos)
		return 1;
	else
		return 0;
}
bool Assembler::find_arr(int index){
	if(source[index].find("[") != string::npos)
		return 1;
	else
		return 0;
}
bool Assembler::find_func(int index){
	if(source[index].find("(") != string::npos)
		return 1;
	else
		return 0;	
}
void Assembler::getNumFromStr_set_var(string s){
	
	Mapping m;
   	stringstream strm;
   	strm << s; 
   	string temp_str;
   	int temp_int;
   	while(!strm.eof()) {
     	strm >> temp_str; 
      	if(stringstream(temp_str) >> temp_int) { 
        	m.value = temp_int;
        	add_offSet = add_offSet - 4;
        	m.address_offset = add_offSet;
        	var.push_back(m);
      	}
      	temp_str = ""; 
   	}

}
void Assembler::set_var(){
	int start = get_main_starting_loc();
	int end = get_main_ending_loc();
	bool check_INT;
	bool check_arr;
	bool check_fun;
	Mapping temp;
	for(int i = start; i < end; i++)
	{	
		check_INT = find_INT(i);
		if(check_INT == 1){
			check_arr = find_arr(i);
			check_fun = find_func(i);
			if(check_arr == 0 && check_fun == 0){
				//cout << source[i] << endl;
				getNumFromStr_set_var(source[i]);
			}
		}
	}
}
void Assembler::print_var(){
	for(int i = 0; i < var.size(); i++){
		cout << "value is " << var[i].value << endl;
		cout << "address offset is " << var[i].address_offset << endl;
	}
}
