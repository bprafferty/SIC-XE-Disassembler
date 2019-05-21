/************************************************************
 Names: Brian Rafferty -- cssc0475
        Youssef Iraqi -- cssc0428
        Adam Hancock -- cssc0435
 Project: Program Assignment 2 -- SIC/XE Disassembler
 File: XEDissasemblerMain.cpp
 Notes: The XE disassembler program checks for arguments from the 
 		command line, if it contains two arguments and they are 
        correct, it opens an XE object file and its accompanying
        symbol file, then sends it to the sicGenerator and the 
        lisGenerator
 
   to run: 	 1) make  	2) ./xed <filename> 
*************************************************************/
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include "Instructions.h"
#include "labelMatcher.h"
#include "SymbolHandler.h"
#include "transformer.cpp"
#include "sicGenerator.cpp"
#include "lisGenerator.cpp"


using namespace std;

/************************************************************
 FUNCTION: startProg(string)
 DESCRIPTION: This method acts as the function to take in 
 				the user's file and add the appropriate 
                extensions. The correct files are then sent
                to their repsective generators.
*************************************************************/
void startProg(string userFile){
    string obj = userFile + ".obj";
    string sym = userFile + ".sym";
    string sic = userFile + ".sic";
    string lis = userFile + ".lis";

    sicGenerator(obj,sym,sic);
    lisGenerator(obj,sym,lis);
}

/************************************************************
 FUNCTION: main(int,char*)
 DESCRIPTION: This is the main method, which reads in input
 				from the user on the command line. It checks
                for errors in the number of arguments entered.
                If correct, it sends the user's file to the 
                method startProg to begin processing.
*************************************************************/
int main(int argc,char *argv[]){
    //check for error
  	if(argc != 2){
        cout<<"Correct format: "<<argv[0]<<" <userFile>\n";
        exit(1); 
    }
    
    //if correct: read in user file
    string userFile = argv[1];
    startProg(userFile);
    return 0;
}