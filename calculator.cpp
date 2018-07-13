#include "calculator.hpp"
#include <string>
#include <cmath>
#include <iostream>
#include <climits>
#include <stack>
#include <string>
#include <sstream>

using namespace std;

// Scanner implementation

// You may have to modify this constructor, although it might not be neccessary.
Scanner::Scanner() : line(1), 
                     value(0),
                     scannedAhead(false){
    // WRITEME
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
Token Scanner::nextToken() {
    // I am a placeholder. Change me into something that can actually decypher the next token without consuming it.
    // WRITEME
    char token;
    token = cin.peek();

    if(token == '(')
    {
        scannedAhead = true;
        return T_OPENPAREN;
    }
    else if(token == ')')
    {
        scannedAhead = true;
        return T_CLOSEPAREN;
    }
    else if(token == '+')
    {
        scannedAhead = true;
        return T_PLUS;
    }
    else if(token =='-')
    {
        scannedAhead = true;
        return T_MINUS;
    }
    else if(token == '1' || token == '2' || token == '3' || token == '4' ||
    token == '5' || token == '6' || token == '7' || token == '8' || token == '9'
    || token == '0')
    {
        int val;
        
        cin >> val;
        this->value = val;
        
        char next = cin.peek();
        if(next == ' ')
        {
            while(next == ' ')
            {
                cin.get(next);
            }
            
            if(next =='1' || next == '2' || next == '3' || next == '4' || 
            next == '5' || next == '6' || next == '7' || next == '8' || 
            next == '9' || next == '0')
            {
                scanError(this->line, ' ');
            }
            cin.putback(next);
        }
        
        string back = to_string(val);
        for(int i = back.length() - 1; i >= 0; --i)
        {
            cin.putback(back.at(i));
        }
        
        scannedAhead = true;
        return T_NUMBER;
    }
    else if(token == '*')
    {
        scannedAhead = true;
        return T_MULTIPLY;
    }
    else if(token == '/')
    {
        scannedAhead = true;
        return T_DIVIDE;
    }
    else if(token == '\n')
    {
        if(!scannedAhead)
        {
            this->line++;
        }
        scannedAhead = true;
        return T_NEWLN;
    }
    else if(token == ';')
    {
        scannedAhead = true;
        return T_SEMICOLON;
    }
    else if(token == 'm')
    {
        //modulo operator
        char first, second, third;
        cin.get(first);
        cin.get(second);
        cin.get(third);
        
        cin.putback(third);
        cin.putback(second);
        cin.putback(first);
        scannedAhead = true;
        if(first == 'm' && second == 'o' && third == 'd')
        {
            return T_MODULO;
        }
        else
        {
            if(second != 'o')
            {
                scanError(lineNumber(), second);
            }
            else
            {
                scanError(line, third);
            }
        }
    }
    else if(token == ' ')
    {
        //Get rid of regular whitespace
        while(token == ' ')
        {
            cin.get(token);
        }
        cin.putback(token);
        return nextToken();
    }
    else if(token < 0)
    {
        //End if no more tokens
        return T_EOF;
    }
    else
    {
        //Error if incorrect token
        scanError(line, token); //Problem code, seems to think that ' ' is an incorrect character
    }
    scannedAhead = true;
    return T_EOF;
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
void Scanner::eatToken(Token toConsume) {
    // I am a placeholder. I'm not even fun. Change me into something that can actually consume tokens!
    // WRITEME
    
    scannedAhead = false;
    char food;
    cin.get(food);
    if(food == '1' || food == '2' || food == '3' || food == '4' ||
    food == '5' || food == '6' || food == '7' || food == '8' || food == '9'
    || food == '0')
    {
        while(food == '1' || food == '2' || food == '3' || food == '4' ||
        food == '5' || food == '6' || food == '7' || food == '8' || food ==
        '9' || food == '0')
        {
            cin.get(food);
        }
        cin.putback(food);
    }
    else if(food == 'm')
    {
        cin.get(food);
        cin.get(food);
    }
    
}

int Scanner::lineNumber() {
    return this->line;
}

int Scanner::getNumberValue() {
    return this->value;
}

// Parser implementation

// You may need to modify this constructor and make it do stuff, although it might not be neccessary.
Parser::Parser(bool eval) : evaluate(eval) {
    // WRITEME
}

void Parser::parse() {
    start();
}

void Parser::start() {
    // I am a placeholder. Implement a recursive descent parser starting from me. Add other methods for different recursive steps.
    // Depending on how you designed your grammar, how many levels of operator precedence you need, etc., you might end up with more
    // or less steps in the process.
    //
    // WRITEME
    ExpressionList();
    print();
}

// You will need to add more methods for this to work. Don't forget to also define them in calculator.hpp!
// WRITEME

void Parser::ExpressionList()
{
    long output = Exp();
    if(evaluate)
    {
        if(output > upperBound)
        {
            outOfBoundsError(scanner.lineNumber(), output);
        }
        outputs.push_back(output);
    }
    ExpressionList2();
}

void Parser::ExpressionList2()
{
    if(scanner.nextToken() == T_SEMICOLON)
    {
        //Evaluate Semicolon Token
        scanner.eatToken(T_SEMICOLON);
        if(scanner.nextToken() == T_NEWLN)
        {
            scanner.eatToken(T_NEWLN);
        }
        
        long output = Exp();
        if(evaluate)
        {
            outputs.push_back(output);
        }
        ExpressionList2();
    }
    else
    {
        return;
    }
}

long Parser::Exp()
{
    long output;
    output = ExpPrime(ExpTwo());
    
    return output;
}
long Parser::ExpPrime(long begin)
{
    long output, second, third;
    if(scanner.nextToken() == T_PLUS)
    {
        //Evaluate Plus Token
        scanner.eatToken(T_PLUS);
        second = ExpTwo();
        third = begin + second;
        output = ExpPrime(third);
    }
    else if(scanner.nextToken() == T_MINUS)
    {
        //Evaluate Minus Token
        scanner.eatToken(T_MINUS);
        second = ExpTwo();
        third = begin - second;
        output = ExpPrime(third);
    }
    else
    {
        output = begin;
    }
    return output;
}

long Parser::ExpTwo()
{
    long output;
    output = ExpTwoPrime(Term());
    
    return output;
}
long Parser::ExpTwoPrime(long begin)
{
    long output, second, third;
    if(scanner.nextToken() == T_MULTIPLY)
    {
        //Evaluate Multiply Token
        scanner.eatToken(T_MULTIPLY);
        second = Term();
        third = begin * second;
        output = ExpTwoPrime(third);
    }
    else if(scanner.nextToken() == T_DIVIDE)
    {
        //Evaluate Divide Token
        scanner.eatToken(T_DIVIDE);
        second = Term();
        if(second == 0)
        {
            divideByZeroError(scanner.lineNumber(), begin);
        }
        third = begin / second;
        output = ExpTwoPrime(third);
    }
    else if(scanner.nextToken() == T_MODULO)
    {
        //Evaluate Modulus Token
        scanner.eatToken(T_MODULO);
        second = Term();
        third = begin % second;
        output = ExpTwoPrime(third);
    }
    else
    {
        output = begin;
    }
    return output;
}

long Parser::Term()
{
    long output;
    if(scanner.nextToken() == T_OPENPAREN)
    {
        Token here = scanner.nextToken();
        //Evaluate Open Parenthesis
        scanner.eatToken(T_OPENPAREN);
        output = Exp();
        //Evaluate Closed Parenthesis
        here = scanner.nextToken();
        if(here != T_CLOSEPAREN)
        {
            mismatchError(scanner.lineNumber(), T_CLOSEPAREN, here);
        }
        scanner.eatToken(T_CLOSEPAREN);
    }
    else
    {
        Token here = scanner.nextToken();
        if(here != T_NUMBER && here != T_EOF)
        {
            parseError(scanner.lineNumber(), here);
        }
        if(evaluate)
        {
            output = scanner.getNumberValue();
        }
        scanner.eatToken(T_NUMBER);
    }
    return output;
}

void Parser::print()
{
    for(unsigned int i = 0; i < outputs.size(); ++i)
    {
        cout << outputs.at(i) << endl;
    }
}
