#include <iostream>
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



constexpr short int  MATRIX_SIZE = 16;

std::ifstream in;
std::ofstream out;
std::ofstream outoptimization;
int IdTriad = 0;

char Matrix[MATRIX_SIZE][MATRIX_SIZE] =
{
    {' ','L', 'S', 'E', 'T', 'M', '=', ';', '|', '&', '~', '(', ')', 'I', 'C','#'},
    {'L',' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<',' ','=' },
    {'S',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ','>'},
    {'E',' ', ' ', ' ', ' ', ' ', ' ', '=', '=', ' ', ' ', ' ', '=', ' ',' ',' '},
    {'T',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ',' ',' '},
    {'M',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ',' ',' '},
    {'=',' ', ' ', '$', '<', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<','<', ' '},
    {';',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',' ', '>'},
    {'|',' ', ' ', ' ', '$', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', ' '},
    {'&',' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'~',' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'(',' ', '$', '<', '<', ' ', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {')',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'I',' ', ' ', ' ', ' ', ' ', '=', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'C',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ', ' ',' '},
    {'#','$', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', ' ',' '},
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
public:
    char SymbolAttitude;
    char Symbol;
public:
    Attitude() { }
    Attitude(const char currentAttitude,const char symbol):SymbolAttitude(currentAttitude),
        Symbol(symbol){}
};

class Translation
{
private:

    int _Lex = EOF;
    int _currentSymbol;
    const std::string _Native = "=;~|&()";
    std::vector<Attitude> stack;    
    const Rule Rules[11] =
    {
        {'_',"#L#"},
        {'L',"LS|S"},
        {'S',"I=E;"},
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
    char FindRules(std::string src);
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
        _Lex = 'I';
     }
     else if (isdigit(_currentSymbol))
     {
     GetSymbol();
     while (isdigit(_currentSymbol))
         GetSymbol();
     _Lex = 'C';
     }
     else if (strchr(_Native.c_str(), _currentSymbol))
     {
        _Lex = _currentSymbol;
        GetSymbol();
     }
     else if (_currentSymbol == EOF)
        _Lex = '#';
     else
     PrintError(TypeErrors::UNKOWN_SYMBOL, std::string((char)_currentSymbol, 1));

 }

 void Translation::Run()
 {
     GetSymbol();
     stack.push_back(Attitude(' ', '#'));
     do
     {
         GetLex();
         Analize();
         std::cout << (char)_Lex;
         if (_Lex == ';')
         {
             for (auto it = stack.begin(); it != stack.end(); ++it)
                 std::cout << it->Symbol;
         }
     } while (_Lex != '#');


 }
 char Translation::FindMatrixElement(char Y)
 {
     char X = stack.back().Symbol;
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
     else if (symbol != '>')
         stack.push_back(Attitude(symbol, _Lex));
     else
     {
         while (symbol == '>')
         {
             char result = RuleConvolution();
             stack.push_back(Attitude(FindMatrixElement(result), result));
             symbol = FindMatrixElement(_Lex);
         }
         stack.push_back(Attitude(symbol, _Lex));
     }
 }
 char Translation::FindRules(std::string src)
 {

     for (int i = 0; i <= rulesCount; i++)
     {
         if (src == Rules[i].right)
             return Rules[i].left;
     }
     return NULL;
 }
 char Translation::RuleConvolution()
 {
     std::string base;
     char result;
     int posL = 0,tempposL=0;

     for (int j = stack.size() - 1; j > 0; j--)
         if (stack[j].SymbolAttitude == '<')
         {
             posL = j;
             break;
         }
            
     if (posL == 0)
     {
         for (int j = stack.size() - 1; j > 0; j--)
             if (stack[j].SymbolAttitude == '$')
             {
                 posL = j;
                 break;
             }
     }

     size_t _sizestack = stack.size();
     while (true)
     {

         tempposL = posL;
         for(;posL< _sizestack;posL++)
            base.push_back(stack[posL].Symbol);
         result = FindRules(base);
         posL = tempposL;
         if (result != NULL)
         {
             for (posL; posL < _sizestack; posL++)
                 stack.pop_back();
             return result;
         }

         else
         {
             base.clear();
             for (; posL < _sizestack; posL++)
             {
                 if (stack[posL].SymbolAttitude == '$')
                 {
                     break;
                 }
             }
             if (posL == stack.size())
                 PrintError(TypeErrors::SYNTAX_ERROR, "Error!");
         }

     }

         /*for (int j = stack.size()-1; j > 0; j--)
         {
             if (stack[j].SymbolAttitude != '<')
                 base = stack[j].Symbol + base;
             else
             {
                 base = stack[j].Symbol + base;
                 result = FindRules(base);
                 if (result != NULL)
                 {
                     for (j; j < stack.size(); j++)
                         stack.pop_back();
                     return result;
                 }
                     
                 else
                 {
                     base.clear();
                     for (int q = j; q < stack.size(); q++)
                     {
                         if (stack[q].SymbolAttitude == '$')
                         {
                             for (; q < stack.size(); q++)
                                 base += stack[q].Symbol;
                             result =  FindRules(base);
                             if (result != NULL)
                             {
                                 for (q = j; q < stack.size(); q++)
                                     stack.pop_back();
                                 return result;
                             }
                         }
                     }
                 }
             }   
         }*/
 }
 int main()
 {
     in.open("text.txt", std::ios::binary);

     if (!in.is_open())
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
