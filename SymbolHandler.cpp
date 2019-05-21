/************************************************************
 Names: Brian Rafferty -- cssc0475
        Youssef Iraqi -- cssc0428
        Adam Hancock -- cssc0435
 Project: Program Assignment 2 -- SIC/XE Disassembler
 File: SymbolHandler.cpp
 Notes: This program works by creating a linked list out of
		the .sym file. It has everything contained within
		for creating nodes, and accessing the information
		contained within the list.
 
   to run: 	 1) make  	2) ./xed <filename> 
*************************************************************/
#include "SymbolHandler.h"

/************************************************************
 FUNCTION: initNode(Node*,string,string,string)
 DESCRIPTION: This method acts as the constructor for all
				instances of nodes to be used in the future.
*************************************************************/
void initNode(struct Node *head, string s, string a, string f)
{
	head->symbol = s;
	head->address = a;
	head->flag = f;
	head->next = NULL;
}

/************************************************************
 FUNCTION: *addNode(Node*,string,string,string)
 DESCRIPTION: This method creates new nodes for the
				linked list whenever new information is
				to be added.
*************************************************************/
Node *addNode(struct Node *head, string s, string a, string f)
{
	// Create the New Node to be inserted
	Node *last = new Node();
	last->symbol = s;
	last->address = a;
	last->flag = f;
	last->next = NULL;

	if (head == NULL)
	{
		cout << "reached" << endl;
		head = last;
	}
	else
	{
		Node *temp = new Node();
		temp = head;

		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = last;
	}
	return head;
}

/************************************************************
 FUNCTION: *parseSym(string)
 DESCRIPTION: This method runs through the .sym file and 
				returns a linked list containing all of 
				the information about the symbols.
*************************************************************/
Node *parseSym(string filename)
{
	ifstream in(filename.c_str());
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
	//CREATING LINKED LIST WITH VALUES
	Node *root = new Node();
	initNode(root, symbolName[0], symbolValue[0], symbolFlag[0]);

	for (int i = 1; i < index3; i++)
	{ //start at index 1 because already handled index 0
		if (root == NULL)
		{
			cout << "Symbol Table is Empty" << endl;
		}
		else
		{
			root = addNode(root, symbolName[i], symbolValue[i], symbolFlag[i]);
		}
	}
	return root;
}

/************************************************************
 FUNCTION: getAddress(Node*,s)
 DESCRIPTION: This method takes in the label name and returns
				the symbol's address.
*************************************************************/
string getAddress(Node *head, string s)
{
	Node *cur = head;

	while (cur)
	{
		if (cur->symbol.compare(s) == 0)
		{
			return cur->address;
		}
		cur = cur->next;
	}
	return NULL;
}

/************************************************************
 FUNCTION: getSymbol(Node*,string)
 DESCRIPTION: This method takes in the label name and returns
				the symbol's name.
*************************************************************/
string getSymbol(Node *head, string a)
{
	Node *cur = head;

	while (cur)
	{
		if (cur->address.compare(a) == 0)
		{
			return cur->symbol;
		}
		cur = cur->next;
	}
	return NULL;
}

/************************************************************
 FUNCTION: getFlag(Node*,string)
 DESCRIPTION: This method takes in the label name and returns
				the symbol's flag.
*************************************************************/
string getFlag(Node *head, string s)
{
	Node *cur = head;
	while (cur)
	{
		if (cur->symbol.compare(s) == 0)
		{
			return cur->flag;
		}
		cur = cur->next;
	}
	return NULL;
}