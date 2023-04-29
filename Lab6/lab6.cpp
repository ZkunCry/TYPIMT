﻿#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>


/*
* Исходная грамматика
      S → I=E;
      E → E “|” T | T
      T → T & M | M
      M → ~M | (E) | I | C
      I → AK | A
      K → DK | D
      C → DC | D
      A → [a-z] | _
      D → 0 | 1

  Приводим грамматику в виду слабого предшествования:
  
      S'→ ꞱSꞱ
      S → I = E;
      E → E "|" T | T
      T → T & M | M
      M → ~M | (E) | I | C
*/



constexpr short int  MATRIX_SIZE = 15;

std::ifstream in;
std::ofstream out;
std::ofstream outoptimization;
int IdTriad = 0;

char Matrix[MATRIX_SIZE][MATRIX_SIZE] =
{
    {' ', 'S', 'E', 'T', 'M', '=', ';', '|', '&', '~', '(', ')', 'I', 'C','#'},
    {'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ','='},
    {'E', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', '=', ' ', ' ',' '},
    {'T', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'M', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'=', ' ', '$', '<', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {';', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ','>'},
    {'|', ' ', ' ', '$', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<',' '},
    {'&', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'~', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'(', ' ', '$', '<', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {')', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'I', ' ', ' ', ' ', ' ', '=', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'C', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'#', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', ' ',' '},
};



struct Rule
{
    char left;
    std::string right;
    Rule(char currentS, const std::string src);
};
enum class TypeErrors //Error codes
{
    SYNTAX_ERROR,
    UNDEF_ID,
    ID_MISS,
    MISSING_SYMBOL,
    UNKOWN_SYMBOL
};


struct Attitude
{
private:
    char SymbolAttitude;
    char Symbol;
public:
    Attitude(const char currentAttitude,const char symbol):SymbolAttitude(currentAttitude),
        Symbol(symbol){}
};

class Translation
{
private:

    int _Lex = EOF;
    int _currentSymbol;

    const std::string _Native = "=;~|&()";
    /*  Текущий стэк
        Левый операнд - хранит отношения
        Правый операнд текущий символ Y
    */
    std::vector<Attitude> stack;    
    const Rule Rules[10] =
    {
        {'_',"#S#"},
        {'I',"=E;"},
        {'E',"E|T"},
        {'E',"T"},
        {'T',"T&M"},
        {'T',"M"},
        {'M',"~M"},
        {'M',"(E)"},
        {'M',"I"},
        {'M',"C"}
    };
    const int rulesCount = sizeof(Rules) / sizeof(*Rules) - 1;

public:

    inline void GetSymbol(); 

    inline void GetLex();
    void Run();
    char FindMatrixElement(char Y);
    void Analize();
    char RuleConvolution();
};


static void PrintError(TypeErrors typeer, std::string param = "")
{
    switch (typeer)
    {
    case TypeErrors::SYNTAX_ERROR:
        std::cout << "Error: Syntax error. " << std::endl;
        break;
    case TypeErrors::ID_MISS:
        std::cout << "Error: Invalid character in identifier. "  << param << std::endl;
        break;
    case TypeErrors::UNDEF_ID:
        std::cout << "Error: Undefined identifier: " << param << std::endl;
        break;
    case TypeErrors::MISSING_SYMBOL:
        std::cout << "Error: " << param << "missing." << std::endl;
        break;
    case TypeErrors::UNKOWN_SYMBOL:
        std::cout << "Error: Unkown symbol " << param << std::endl;
        break;
    }
    in.close();
    exit(1);
}

 void Translation::GetSymbol()
{
     _currentSymbol = in.get();
}

 inline void Translation::GetLex()
 {
     while (isspace(_currentSymbol)) GetSymbol();

     if (isalpha(_currentSymbol) || _currentSymbol == '_')
     {
         GetSymbol();
         while (isdigit(_currentSymbol))
             GetSymbol();
         _currentSymbol = 'I'; 
     }
     else if (isdigit(_currentSymbol))
     {
         GetSymbol();
         while (isdigit(_currentSymbol))
             GetSymbol();
         _currentSymbol = 'C'; 
     }
     else if (_Native.find(_currentSymbol) > -1)
     {
         _Lex = _currentSymbol;
         GetSymbol();
     }
     else if (_currentSymbol == EOF)
         _Lex = '#';
     else
         PrintError(TypeErrors::UNKOWN_SYMBOL, std::string((char)_currentSymbol,1));

 }

 void Translation::Run()
 {
     GetSymbol();
     stack.push_back(Attitude('#', ' '));
     do
     {
         GetLex();    /* получаем лексему   */
         std::cout << _Lex<<std::endl;    /* и печатаем лексему */
     } while (_Lex != EOF);

 }
 char Translation::FindMatrixElement(char Y)
 {
     /*char X = (char)Stack.top().c_str();*/

     char X = ' ';
     int i = 0, j = 0;
     while (Matrix[i][0] != X)
         i++;

     while (Matrix[0][j] != Y)
         j++;

     if (j > MATRIX_SIZE)
         PrintError(TypeErrors::UNKOWN_SYMBOL, "No matrix element found");
     return Matrix[i][j];
 }
 void Translation::Analize()
 {
     char symbol = FindMatrixElement(_Lex);
     if (symbol == ' ')
         PrintError(TypeErrors::UNKOWN_SYMBOL, "No relation for lexeme " + symbol);
     /*else if (symbol == '>')
     {
         Stack.push(std::string(symbol,1));
         Stack.push(std::string(_Lex, 1));

     }*/

 }
 char Translation::RuleConvolution()
 {
     return 0;
 }
 int main()
 {
     in.open("text.txt", std::ios::binary);
     out.open("translation.txt");
     outoptimization.open("optimization.txt");
     if (!in.is_open() || !out.is_open())
     {
         std::cout << "Error! Can't open file " << std::endl;
         return 2;
     }
     Translation start;
     start.Run();
     in.close();
     out.close();
     outoptimization.close();
     return 0;
 }

 Rule::Rule(char currentS,const std::string src):left(currentS),right(src){}
