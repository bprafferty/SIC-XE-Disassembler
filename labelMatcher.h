// header file for getting the label from a symtab given an address
#ifndef labelMatcher_H
#define labelMatcher_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

pair<string, int>  findElem(string fileName, string address);
pair<string,int> searchLit(string fileName, string address);
#endif