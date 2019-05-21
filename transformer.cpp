/************************************************************
 Names: Brian Rafferty -- cssc0475
        Youssef Iraqi -- cssc0428
        Adam Hancock -- cssc0435
 Project: Program Assignment 2 -- SIC/XE Disassembler
 File: transformer.cpp
 Notes: This program acts as the middle-man for the calculations
        relating to converting from one type to another. It is
        referenced throughout the other files whenever it is
        needed.
 
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
 FUNCTION: hexConversion(string,bool)
 DESCRIPTION: This method takes in hex and converts it to
                its integer representation. Also checks for
                signed vs unsigned values.
*************************************************************/
int hexConversion(string address, bool k = false)
{
    char *p;
    long n = strtoul(address.c_str(), &p, 16);
    if (*p != 0)
    {
        cout << "HEX ERROR" << endl;
    }

    if (n > 2047 && k == true)
    {
        for (int i = address.length(); i < 8; i++)
        {
            address = 'F' + address;
        }
        unsigned int x;
        stringstream swap;
        swap << hex << address;
        swap >> x;
        n = static_cast<int>(x);
    }
    return n;
}

/************************************************************
 FUNCTION: characterConversion(char)
 DESCRIPTION: This method takes in a character and converts
                it to the correct integer by subtracting
                its ascii value.
*************************************************************/
int characterConversion(char a)
{
    int b = (int)a;

    if (b > 47 && b < 58)
    {
        b = b - 48;
    }
    else
    {
        b = b - 55;
    }
    return b;
}

/************************************************************
 FUNCTION: numberConversion(int)
 DESCRIPTION: This method takes in an integer and converts
                it to hex in the form of a char.
*************************************************************/
char numberConversion(int a)
{
    if (a > 9)
    {
        char letters[] = {'A', 'B', 'C', 'D', 'E', 'F'};
        int i = 1;
        for (i = 1; i < 7; i++)
        {
            if ((a - 9) == i)
            {
                return letters[i - 1];
            }
        }
    }
    else
    {
        int b = a + '0';
        return b;
    }
    return 'Z';
}

/************************************************************
 FUNCTION: getReg(char)
 DESCRIPTION: This method takes in a character value, and
                returns the corresponding register based
                upon the converted character's int value.
*************************************************************/
string getReg(char a)
{
    string value[] = {"A", "X", "L", "B", "S", "T", "F", "PC", "SW", "ERROR"};

    int reVal = characterConversion(a);

    return value[reVal];
}