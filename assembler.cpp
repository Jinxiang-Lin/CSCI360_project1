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