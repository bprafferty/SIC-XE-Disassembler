/************************************************************
 Names: Brian Rafferty -- cssc0475
        Youssef Iraqi -- cssc0428
        Adam Hancock -- cssc0435
 Project: Program Assignment 2 -- SIC/XE Disassembler
 File: lisGenerator.cpp
 Notes: This program is the workhorse responsible for turning
        .obj and .sym files into a generated .lis file. It 
        reads in the .obj file one line at a time and follows
        protocol described below to output the correctly
        assembled program based upon the opcodes and addresses
        given.
 
   to run: 	 1) make  	2) ./xed <filename> 
*************************************************************/
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

/************************************************************
 FUNCTION: lisAllocationPrinter(string,int,int)
 DESCRIPTION: This method is responsible for the printing of
                all the memory allocations at the bottom of
                the assembly file, just before the END
                assembly directive.
*************************************************************/
void lisAllocationPrinter(string fileName, int objLength, int progCounter, string LC)
{
    Node *symlist = parseSym(fileName);
    Node *pointer = symlist;

    int address;
    int nextAddress;
    int instrLengthBytes;

    //iterate until linked list ends
    while (pointer != NULL)
    {

        address = hexConversion(pointer->address);

        if (address >= progCounter)
        {
            //determines size of instruction
            if (pointer->next != NULL)
            {
                nextAddress = hexConversion(pointer->next->address);
                instrLengthBytes = nextAddress - address;
            }
            else
            {
                instrLengthBytes = objLength - address;
            }

            //convert address to hex
            stringstream stream;
            stream << uppercase << hex << address;
            string result = stream.str();

            //start by printing label
            if (result.length() == 1)
            {
                outputFile << "000" << result << "\t" << pointer->symbol << "\t";
            }
            else if (result.length() == 2)
            {
                outputFile << "00" << result << "\t" << pointer->symbol << "\t";
            }
            else if (result.length() == 3)
            {
                outputFile << "0" << result << "\t" << pointer->symbol << "\t";
            }
            else
            {
                outputFile << result << "\t" << pointer->symbol << "\t";
            }

            //print bytes
            if (instrLengthBytes == 1)
            {
                if (pointer->flag.compare("A") == 0)
                {
                    outputFile << "BYTE\t1\n";
                }
                else
                {
                    outputFile << "RESB\t1\n";
                }
            }

            //else print words
            else
            {
                int instrLengthWords = instrLengthBytes / 3;
                if (pointer->flag.compare("A") == 0)
                {
                    outputFile << "WORD\t";
                }
                else
                {
                    outputFile << "RESW\t";
                }
                outputFile << instrLengthWords << endl;
            }
        }
        //move pointer to next node in symbol list
        pointer = pointer->next;
    }
}

/************************************************************
 FUNCTION: lisGenerator(string,string,string)
 DESCRIPTION: Generates the .lic file by reading in from the
                .obj and .sym files, and then translates that
                information into a completely assembled file.
                Begins by moving line by line in the .obj file
                and records appropriate instructions.
*************************************************************/
void lisGenerator(string obj, string sym, string lis)
{
    //open .obj and .sym files
    ifstream objectFile(obj.c_str()); 
    ifstream symtab(sym.c_str());     
    outputFile.open(lis.c_str(), std::ofstream::out | std::ofstream::trunc);

    //look for errors while setting up files
    if (!symtab.is_open() || !objectFile.is_open())
    {
        cout << "Error";
        exit(1);
    }

    //instantiate variables for future use
    int PC;
    string LC;
    int TA = 0;
    int base = 0;
    int startAddress;
    string initialInstruction;
    string currentLine;
    stringstream result_line;

    vector<LisLine> infoLis;
    Instructions Instructions;
    pair<string, string> opData;
    int index;
    int objectLen;

    while (getline(objectFile, currentLine))
    {
        //format: address , label, operation, operand, opcode
        //ex:     0000      FIRST       LDA     #0      020000
        
        //set up instance of objects
        LisLine *obj = new LisLine();

        int i = 0;
        //start analyzing the .obj file line by line
        if (currentLine.at(i) == 'H')
        {
            //record necessary information from header
            string controlBlockLabel = currentLine.substr(i + 1, 6);
            obj->label = controlBlockLabel;
            i = 6;
            string firstAddress = currentLine.substr(i + 1, 6);
            i = i + 6;
            string length = currentLine.substr(i + 1, 6);
            objectLen = hexConversion(length);
            pair<string, int> firstSymbol = findElem(sym, firstAddress);
            initialInstruction = firstSymbol.first;
            opData = Instructions.findInstruction(firstAddress);
            if (firstAddress == "000000")
            {
                firstAddress.erase(firstAddress.begin() + 1, firstAddress.end());
            }
            obj->operation = "START";
            obj->operand = firstAddress;
            obj->address = firstAddress;
            LC = obj->address;
            obj->opcode = "";
            startAddress = hexConversion(firstAddress);
            PC = startAddress;
            //print the first line to the .lis file
            if (LC.length() == 1)
            {
                outputFile << "000" << obj->address << "\t" << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << endl;
            }
            else if (LC.length() == 2)
            {
                outputFile << "00" << obj->address << "\t" << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << endl;
            }
            else if (LC.length() == 3)
            {
                outputFile << "0" << obj->address << "\t" << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << endl;
            }
            else
            {
                outputFile << obj->address << "\t" << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << endl;
            }
            continue;
        }

        //move on to T records
        else if (currentLine.at(i) == 'T')
        {
            if (i == 0)
            {
                index = i + 8;
            }
            while (index + 1 < currentLine.length())
            {
                //update location address based upon program counter
                stringstream loc_ss;
                loc_ss << std::uppercase << hex << PC;
                loc_ss >> LC;

                //print it to file
                if (LC.length() == 1)
                {
                    outputFile << "000" << LC << "\t";
                }
                else if (LC.length() == 2)
                {
                    outputFile << "00" << LC << "\t";
                }
                else if (LC.length() == 3)
                {
                    outputFile << "0" << LC << "\t";
                }
                else
                {
                    outputFile << LC << "\t";
                }

                string targetAddr;
                string opCode = "";
                bool isLit = false;
                pair<string, int> symbol;
                symbol = findElem(sym, LC);
                //if label does not exist, set it to a blank
                if (symbol.first == "NOT FOUND")
                {
                    symbol.first = "";
                }
                obj->label = symbol.first;

                //set up potential opcodes for future comparisons
                string format3 = currentLine.substr(index + 1, 6);
                string format4 = currentLine.substr(index + 1, 8);
                //read the first two
                string firstTwoBytes = currentLine.substr(index + 1, 2);
                index += 2; 
                //incremement index to read next bytes
                int temp = characterConversion(firstTwoBytes[1]);
                //calculation will yield bits n & i
                int nAndiBits = temp % 4; 
                bool nBit = false;
                bool iBit = false;
                
                //set flags if bits are active
                if ((nAndiBits & 1) == 1)
                {
                    iBit = true;
                }
                if ((nAndiBits >> 1) == 1)
                {
                    nBit = true;
                }
                //set operation based upon first 2 bytes of opcode
                temp = temp - nAndiBits; 
                stringstream ss;
                ss << firstTwoBytes[0] << numberConversion(temp);
                ss >> opCode;
                opData = Instructions.findInstruction(opCode);

                //if format is incorrect
                if (opData.second == "NOT FOUND")
                {
                    cout << "Incorrect Object File" << endl;
                    exit(1);
                }
                //check for format 1
                if (opData.second == "1")
                {
                    obj->operation = opData.first;
                    PC = PC + 1;
                }
                //check for format 2
                else if (opData.second == "2")
                {
                    string registers = currentLine.substr(index + 1, 2);
                    index += 2;
                    string firstReg = getReg(registers[0]);
                    string secondReg = getReg(registers[1]);
                    obj->operation = opData.first;
                    obj->operand = firstReg + "," + secondReg;
                    PC = PC + 2;
                }
                //else format 3 or 4
                else
                {
                    string operate = opData.first;

                    obj->operation = operate;
                    //cout << "operation: " << obj->operation << endl;
                    bool xBit = false;
                    bool bBit = false;
                    bool pBit = false;
                    bool eBit = false;
                    //analyze next byte to see which are active
                    temp = characterConversion(currentLine[index + 1]);
                    index = index + 1;
                    //bit operations to test for flags
                    if ((temp & 1) == 1)
                    {
                        eBit = true;
                    }
                    if (((temp >> 1) & 1) == 1)
                    {
                        pBit = true;
                    }
                    if (((temp >> 2) & 1) == 1)
                    {
                        bBit = true;
                    }
                    if (((temp >> 3) & 1) == 1)
                    {
                        xBit = true;
                    }

                    //format 4
                    if (eBit == true)
                    {
                        PC = PC + 4;
                        obj->opcode = format4;
                    }
                    //format 3
                    else
                    {
                        PC = PC + 3;
                        obj->opcode = format3;
                    }
                    
                    //for symtab retrieval
                    pair<string, int> symbolInfo;

                    //find correct calculations for TA based upon conditions
                    if ((bBit == true) && (pBit == false) && (eBit == false))
                    {
                        string disparityBits = currentLine.substr(index + 1, 3);
                        index += 3;
                        int disparityValue = hexConversion(disparityBits);
                        TA = disparityValue + base;
                        stringstream new_s;
                        new_s << std::uppercase << hex << TA;
                        new_s >> targetAddr;
                    }
                    else if ((bBit == false) && (pBit == true) && (eBit == false))
                    {
                        string disparityBits = currentLine.substr(index + 1, 3);
                        index += 3;
                        int disparityValue = hexConversion(disparityBits, true);
                        TA = disparityValue + PC;
                        stringstream new_s;
                        new_s << std::uppercase << hex << TA;
                        new_s >> targetAddr;
                    }
                    else if ((bBit == false) && (pBit == false) && (eBit == true))
                    {
                        targetAddr = currentLine.substr(index + 1, 5);
                        TA = hexConversion(targetAddr);
                        index += 5;
                        operate.insert(operate.begin(), '+');
                        obj->operation = operate;
                    }
                    else if ((bBit == false) && (pBit == false) && (eBit == false))
                    {
                        targetAddr = currentLine.substr(index + 1, 3);
                        index += 3;
                    }

                    if (nBit == true && iBit == false)
                    {
                        symbolInfo = findElem(sym, targetAddr);
                        if (symbolInfo.first == "NOT FOUND")
                        {
                            symbolInfo = searchLit(sym, targetAddr);
                            if (symbolInfo.first == "NOT FOUND")
                            {
                                cout << "Label not found." << endl;
                                exit(1);
                            }
                            else
                            {
                                isLit = true;
                            }
                        }
                        targetAddr = symbolInfo.first;
                        targetAddr.insert(targetAddr.begin(), '@');
                    }
                    else if (iBit == true && nBit == false)
                    {
                        if (bBit == false && pBit == false && eBit == false)
                        {
                            int temp = hexConversion(targetAddr);
                            stringstream new_s;
                            new_s << std::uppercase << temp;
                            new_s >> targetAddr;
                            targetAddr.insert(targetAddr.begin(), '#');
                        }
                        else if (eBit == true)
                        {
                            symbolInfo = findElem(sym, targetAddr);
                            if (symbolInfo.first == "NOT FOUND")
                            {
                                int temp = hexConversion(targetAddr);
                                stringstream new_s;
                                new_s << std::uppercase << temp;
                                new_s >> targetAddr;
                                targetAddr.insert(targetAddr.begin(), '#');
                            }
                            else
                            {
                                targetAddr = symbolInfo.first;
                                targetAddr.insert(targetAddr.begin(), '#');
                            }
                        }
                        else
                        {
                            symbolInfo = findElem(sym, targetAddr);
                            if (symbolInfo.first == "NOT FOUND")
                            {
                                cout << "Label not found." << endl;
                                exit(1);
                            }
                            else
                            {
                                targetAddr = symbolInfo.first;
                                targetAddr.insert(targetAddr.begin(), '#');
                            }
                        }
                    }

                    //direct addressing mode
                    else
                    {
                        symbolInfo = findElem(sym, targetAddr);
                        if (symbolInfo.first == "NOT FOUND")
                        {
                            symbolInfo = searchLit(sym, targetAddr);
                            if (symbolInfo.first == "NOT FOUND")
                            {
                                cout << "Label not found." << endl;
                                exit(1);
                            }
                            else
                            {
                                isLit = true;
                            }
                        }
                        targetAddr = symbolInfo.first;
                    }

                    //index mode
                    if (xBit == true)
                    {
                        targetAddr.append(",X");
                    }
                    if (operate.find("RSUB") != std::string::npos)
                    {
                        targetAddr.clear();
                    }

                    obj->operation = symbol.first;
                    obj->operation = operate;
                    obj->operand = targetAddr;

                    if (obj->operation.length() < 4 && obj->operand.length() < 4)
                    {
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t\t" << obj->opcode << endl;
                    }
                    else if (obj->operation.length() < 4 && obj->operand.length() == 4)
                    {
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t\t" << obj->opcode << endl;
                    }
                    else if (obj->operation.length() < 4 && obj->operand.length() > 4 && obj->operand.length() < 8)
                    {
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t\t" << obj->opcode << endl;
                    }
                    else if (obj->operation.length() <= 4 && obj->operand.length() > 4 && obj->operand.length() < 8)
                    {
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t\t" << obj->opcode << endl;
                    }
                    else if (obj->operation.length() <= 4 && obj->operand.length() > 4 && obj->operand.length() == 8)
                    {
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << obj->opcode << endl;
                    }
                    else if (obj->operand.length() == 0)
                    {
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t\t" << obj->opcode << endl;
                    }
                    else
                    {
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << obj->opcode << endl;
                    }

                    //base relative mode
                    if (operate.find("LDB") != std::string::npos)
                    {
                        if (LC.length() == 1)
                        {
                            outputFile << "000" << LC << "\t";
                        }
                        else if (LC.length() == 2)
                        {
                            outputFile << "00" << LC << "\t";
                        }
                        else if (LC.length() == 3)
                        {
                            outputFile << "0" << LC << "\t";
                        }
                        else
                        {
                            outputFile << LC << "\t";
                        }
                        obj->operation = "BASE";
                        obj->operand = symbolInfo.first;
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << endl;
                        base = TA;
                    }

                    if (isLit == true)
                    {
                        result_line.str(std::string());
                        if (LC.length() == 1)
                        {
                            outputFile << "000" << LC << "\t";
                        }
                        else if (LC.length() == 2)
                        {
                            outputFile << "00" << LC << "\t";
                        }
                        else if (LC.length() == 3)
                        {
                            outputFile << "0" << LC << "\t";
                        }
                        else
                        {
                            outputFile << LC << "\t";
                        }
                        obj->operation = "LTORG";
                        obj->operand = "";
                        outputFile << obj->label << "\t" << obj->operation << "\t" << obj->operand << "\t" << endl;
                        index = index + (2 * symbolInfo.second);
                        PC = PC + symbolInfo.second;
                    }
                    targetAddr.clear();
                }
            }
            continue;
        }
        //modification records
        else if (currentLine.at(i) == 'M')
        {
            continue;
        }

        //end of .obj file, print memory allocations before END
        else if (currentLine.at(i) == 'E')
        {

            lisAllocationPrinter(sym, objectLen, PC, LC);

            obj->operation = "END ";
            obj->operand = initialInstruction;
            outputFile << "\t" << obj->label << "\t" << obj->operation << "\t" << obj->operand << endl;

            break;
        }
    }
    //close files before ending
    objectFile.close();
    symtab.close();
    outputFile.close();
}
