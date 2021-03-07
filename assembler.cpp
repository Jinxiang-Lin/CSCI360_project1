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
