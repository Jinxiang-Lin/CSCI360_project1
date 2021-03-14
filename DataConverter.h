#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include <string>
#include <iomanip>
#include <bitset>
#include <vector>

using namespace std;

class DataConverter
{
    public:
        static void split(const string &txt, vector<string> &output, char ch);
}