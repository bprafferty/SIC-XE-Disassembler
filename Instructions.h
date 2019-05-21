//Instructions header file
#ifndef Instructions_H
#define Instructions_H

#include <string>
#include <map>
using namespace std;

class Instructions{
	public:
		Instructions();
		
		struct instr{
			string opcode;
			struct data{
				string mnemonic;
				string format;
			} data;
		};
		
		static const instr insTab[59];
		
		pair<string, string> findInstruction(string op);
		bool isInTable(string op);
		
		//for testing purposes only
		int tableSize();
		
	private:
		map<string, struct instr::data> mapTab;
	
};	
#endif