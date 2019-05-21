/************************************************************
 Names: Brian Rafferty -- cssc0475
        Youssef Iraqi -- cssc0428
        Adam Hancock -- cssc0435
 Project: Program Assignment 2 -- SIC/XE Disassembler
 File: labelMatcher.cpp
 Notes: This program has the primary function of returning
		pair results of labels and their addresses for other
		classes that require that functionality. 
 
   to run: 	 1) make  	2) ./xed <filename> 
*************************************************************/
#include "labelMatcher.h"

/************************************************************
 FUNCTION: findElem(string,string)
 DESCRIPTION: This method performs as a parser of the .sym
				file, it will open it and search if an 
				address that was passed in matches a label.
				It will return a label/address pair if the 
				search was successful.
*************************************************************/
pair<string, int> findElem(string fileName, string address)
{
	int aSize = address.length();

	if (aSize < 6)
	{
		for (int i = 0; i < 6 - (aSize); i++)
		{
			address.insert(address.begin(), '0');
		}
	}

	ifstream in(fileName.c_str());
	if (!in)
	{
		cout << "Cannot open input file.\n";
	}
	//getting all lines from file
	char str[255];
	string lines[255];
	int index = 0;
	while (in)
	{
		in.getline(str, 255);
		if (in)
		{
			lines[index] = str;
			index++;
		}
	}
	in.close();

	//getting only symbols
	string *symbols = new string[index];
	int index2 = 0;
	for (int i = 0; i < index; i++)
	{
		string line = lines[i];
		char letter = line[1];
		if (letter >= 'A' && letter <= 'Z')
		{
			symbols[index2] = lines[i];
			index2++; //keeps track of elements in symbols array
		}
	}

	//Splitting the symbol Name, Value, and Flag into separate arrays
	string *symbolName = new string[index2];
	string *symbolValue = new string[index2];
	string *symbolFlag = new string[index2];
	int counter = 0;
	int index3 = 0;
	string s = symbols[0];
	string delimiter = " ";
	size_t pos = 0;
	string token;

	for (int i = 0; i < index2; i++)
	{
		string s = symbols[i];
		while ((pos = s.find(delimiter)) != string::npos)
		{
			token = s.substr(0, pos);
			if (token.compare(""))
			{
				if (counter == 0)
				{
					symbolName[index3] = token;
				}
				if (counter == 1)
				{
					symbolValue[index3] = token;
				}
				counter++;
			}
			s.erase(0, pos + delimiter.length());
		}
		symbolFlag[index3] = s;
		counter = 0;
		index3++;
	}

	int length;
	for (int i = 0; i < index3; i++)
	{
		if (address.compare(symbolValue[i]) == 0)
		{
			stringstream ss;
			ss << hex << symbolValue[i];
			ss >> length;
			return make_pair(symbolName[i], length);
		}
	}

	//separating literal information into different arrays
	return make_pair("NOT FOUND", 0);
}

/************************************************************
 FUNCTION: searchLit(string,string)
 DESCRIPTION: This method will also parse the file passed
				in and will split all of the data within. 
				It aids in creating the linked list of the
				symtab, by storing things by literal location.
				Also performs as a litTab searcher, and will
				return a label/address pair if successful.
*************************************************************/
pair<string, int> searchLit(string fileName, string address)
{
	int aSize = address.length();
	if (aSize < 6)
	{
		for (int i = 0; i < 6 - (aSize); i++)
		{
			address.insert(address.begin(), '0');
		}
	}
	// cout << "The address is " << address<< endl;
	ifstream in(fileName.c_str());
	if (!in)
	{
		cout << "Cannot open input file.\n";
	}
	//getting all lines from file
	char str[255];
	string lines[255];
	int index = 0;
	while (in)
	{
		in.getline(str, 255);
		if (in)
		{
			lines[index] = str;
			index++;
		}
	}
	in.close();

	//separting literals
	int indexLit = 0;
	string *literals = new string[index];
	int index2 = 0;
	for (int i = 0; i < index; i++)
	{
		string line = lines[i];
		char letter = line[1]; //Labels are all in caps
		if (!line.empty())
		{
			//takes off significant markers for the symtab and littab
			if (letter >= 'A' && letter <= 'Z')
			{
				continue;
			}
			else if (!(letter >= 'a' && letter <= 'z') && letter != '-')
			{
				literals[indexLit] = lines[i];
				indexLit++;
			}
		}
	}

	string *literalName = new string[indexLit];
	string *literalLength = new string[indexLit];
	string *literalAddress = new string[indexLit];
	//a literal's name, length, and address are linked by location in arrays
	int index3 = 0;
	int counter3 = 0;
	string s = literals[0];
	string delimiter = " ";
	size_t pos = 0;
	string token;

	for (int i = 0; i < indexLit; i++)
	{
		string s(literals[i]);
		istringstream iss(s);
		do
		{
			string sub;
			iss >> sub;
			if (counter3 == 0)
			{
				literalName[index3] = sub;
				counter3++;
			}
			else if (counter3 == 1)
			{
				literalLength[index3] = sub;
				counter3++;
			}
			else if (counter3 == 2)
			{
				literalAddress[index3] = sub;
				counter3++;
			}
			else
			{
				counter3 = 0;
				index3++;
			}

		} while (iss);
	}

	//checking to see if address passed is in the LITTAB
	int length;
	string label;
	for (int i = 0; i < index3; i++)
	{
		if (address.compare(literalAddress[i]) == 0)
		{
			label = literalName[i];
			if (literalName[i].at(1) == 'X')
			{
				//CONVERT THE LENGTH TO int then perform some operation and then turn it back to string
				stringstream i_val(literalLength[i]);
				i_val >> length;
				length = length / 2;
			}
			else if (literalName[i].at(1) == 'C')
			{
				stringstream i_val(literalLength[i]);
				i_val >> length;
			}
			return make_pair(label, length);
		}
	}
	return make_pair("NOT FOUND", 0);
}