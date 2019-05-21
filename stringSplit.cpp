#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

string checkSyntax(vector<int> classifiedLine) {
    string returnStatement;
    
    int sumParenthesis = 0;
    for (int i = 0; i < classifiedLine.size(); i++) {

        if (classifiedLine[i] == 1 && i != 1) {
            returnStatement.append("\n\tError: assignment operator (=) at incorrect index.");
        }

        if (classifiedLine[i] == 5 && i != classifiedLine.size()-1){
            returnStatement.append("\n\tError: incorrect symbol/operation used.");
        }

        if (classifiedLine[i] == 4 && i != classifiedLine.size()-2) {
            returnStatement.append("\n\tError: line misuses semicolon.");  
        }

        if (classifiedLine[i] != 4 && i == classifiedLine.size()-2) {
            returnStatement.append("\n\tError: line missing semicolon. (if there, needs space after)");   
        }

        if (classifiedLine[i] == 3) {
            sumParenthesis += 1;
        }

        if ((classifiedLine[i] == classifiedLine[i+1]) && (classifiedLine[i] != 5)){
            returnStatement.append("\n\tError: consecutive symbols/operations used."); 
        }
        
    }

    if (sumParenthesis%2 != 0) {
        returnStatement.append("\n\tError: line missing parenthesis."); 
    }
    
    return returnStatement;
}

bool checkOp(string token){
    if (token.length() > 1){
        return false;
    }
    return true;
}

bool checkId(string token){
    for (int i = 1; i < token.length(); i++){
        if (token[i] <= 39 || (token[i] >= 42 && token[i] <= 47) || (token[i] >= 58 && token[i] <= 64) || (token[i] >= 91 && token[i] <= 96) || (token[i] >= 123) ){
            return false;
        }
    }
    return true;
}

vector<int> checkToken(string token) {

    vector<int> stagingVector;
    char firstChar = token[0];
    char lastChar = token[token.size() - 1];
    
    char operations[] = {'+','-','*','/','%'};
    
    if ((firstChar >= 65 && firstChar <= 90 && lastChar == ')') || (firstChar >= 97 && firstChar <=122 && lastChar == ')')){
        stagingVector.push_back(0);
        stagingVector.push_back(3);
        return stagingVector;
    }

    if ((firstChar >= 65 && firstChar <= 90) || (firstChar >= 97 && firstChar <=122)) {
        stagingVector.push_back(0);
        return stagingVector;
    }

    if (firstChar == '=') {
        stagingVector.push_back(1);
        return stagingVector;
    }

    for (int i = 0; i < 5; i++) {
        if(firstChar == operations[i]){
            stagingVector.push_back(2);
            return stagingVector;
        }
    }

    if (firstChar == '(' || firstChar == ')') {
        stagingVector.push_back(3);
        return stagingVector;
    }

    if (firstChar == ';') {
        stagingVector.push_back(4);
        return stagingVector;
    }
    
    stagingVector.push_back(5);
    return stagingVector;

}


vector<int> classifier(vector<string> tokens) {
    vector<int> types;
    //symbol == 0
    //equal sign == 1
    //operation == 2
    //parenthesis == 3
    //semicolon == 4
    vector<int> result;

    for (int i = 0; i < tokens.size(); i++) {
        result = checkToken(tokens[i]);
        types.insert(types.end(), result.begin(), result.end());
    }
    return types;
}

vector<string> inputSplitter(string currentLine) {
    int head = 0;
    int tail;
    vector<string> tokens;

    while (true) {
        tail = currentLine.find(" ",head);
        string token;

        if (tail != string::npos) {
            token = currentLine.substr(head, tail - head);
            head = tail + 1;
        }
        else {
            token = currentLine.substr(head);
        }
        tokens.push_back(token);

        if (tail == string::npos) {
            break;
        }
    }
    return tokens;
}

int main(int argc,char *argv[]){
    string file = argv[1];
    string currentLine;
    string message = "";
    string symbolParseError = "";
    string opParseError = "";
    
    int counter = 0;
    vector<string> inputVector;
    vector<int> type;
    ifstream userFile(file.c_str());
    while(getline(userFile,currentLine)){
        try {
            counter += 1;
            cout << "\nFile line #" << counter << ": " << currentLine;
            inputVector = inputSplitter(currentLine);
            bool isValid = true;
            type = classifier(inputVector);
            message = checkSyntax(type);
            for (int i = 0; i < type.size(); i++) {
                if (type[i] == 0) {
                    if (checkId(inputVector[i]) == false){
                        symbolParseError = "\n\tError: symbol contains incorrect characters.";
                        message.append(symbolParseError);
                    }
                }
                if (type[i] == 2) {
                    if (checkOp(inputVector[i]) == false){
                        opParseError = "\n\tError: consecutive symbols/operations used.";
                        message.append(opParseError);
                    }
                }
            }
            
            if (message.compare("") != 0){
                isValid = false;
            }

            if (isValid == false){
                cout << message << endl;
            }
            if (isValid == true) {
                cout << "\n\tCorrect Statement." << endl;
            }

        }
        catch(string error){
            cout << "File Error: " << error << endl;
            exit(0);
        }
    }
    cout << endl;
    userFile.close();
}
