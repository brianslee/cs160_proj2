#pragma once

#include "helpers.hpp"

#include <climits>
#include <vector>

using namespace std;
// SCANNER DEFINITION

// You can freely add member fields and functions to this class.
class Scanner {
    int line;
    int value;
    bool scannedAhead;

public:
    // You really need to implement these four methods for the scanner to work.
    Token nextToken();
    void eatToken(Token);
    int lineNumber();
    int getNumberValue();
    
    Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
    Scanner scanner;
    
    bool evaluate; //Evaluate or don't evaluate
    int upperBound = INT_MAX;
    vector<long> outputs;
    
    void start();
    // You will need to define the recursive descent functions you're going to use here.
    // WRITEME
    void ExpressionList();
    void ExpressionList2();
    
    long Exp();
    long ExpPrime(long begin);
    long ExpTwo();
    long ExpTwoPrime(long begin);
    long Term();
    
    void print();
    
public:
    void parse();
    
    Parser(bool);
};
