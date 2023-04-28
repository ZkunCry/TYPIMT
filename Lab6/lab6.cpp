#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>


/*
* Исходная грамматика
    * S→ I=E;
      E→ E “|” T | T
      T→ T & M | M
      M→ ~M | (E) | I | C
      I→ AK | A
      K→ DK | D
      C → DC | D
      A→[a-z] | _
      D→0 | 1

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
    {'=', ' ', '<=', '<', '<', ' ', ' ', ' ', ' ', ' ', '<', '<', '<', '<',' '},
    {';', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ','>'},
    {'|', '<', '=', '<=', '<', '<', ' ', '<', '<', '<', ' ', ' ', '<', '<',' '},
    {'&', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ',' '},
    {'~', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ',' '},
    {'(', ' ', '<=', ' ', '<', '<', ' ', '<', '<', '<', ' ', ' ', ' ', ' ',' '},
    {')', ' ', 'c', 'c', '<', '<', ' ', '<', '<', '<', ' ', ' ', ' ', ' ',' '},
    {'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>', ' ', '>', '>',' '},
    {'C', ' ', ' ', ' ', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' '},
    {'#', '=', ' ', ' ', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', ' ',' '},
};



struct Rule
{
    char left;
    std::string right;
    Rule(char currentS, const std::string src);
};
class Base
{
public:
    virtual std::string GetOperand() { return ""; };
    virtual ~Base();

};
enum class TypeErrors //Error codes
{
    SYNTAX_ERROR,
    UNDEF_ID,
    ID_MISS,
    MISSING_SYMBOL,
    UNKOWN_SYMBOL
};

struct Var:public Base // Struct of identifier
{
private:
    std::string IdName;
public:
   Var(const std::string& src);
   std::string GetOperand()override;
};


struct Constant:public Base
{
public:
    long ConstValue;
    Constant(const long value);
    std::string GetOperand()override;
};


struct Reference :public Base
{
public:
    long TriadRef;
    Reference(const long TriadRef);
    std::string GetOperand()override;
   
};
struct None :public Base
{
public:
    None();
    std::string GetOperand()override;
};

class Triad
{
public:
    char operation;
    Base* op1;
    Base* op2;
    bool isDelete = false;
public:
    Triad(char operation, Base *op1, Base *op2);
    void OutTriad();

};

class Analizer
{
private:
    std::vector<Triad> TriadList;
    int _Lex = EOF;
    int _currentSymbol;
    const std::string _Native = "=;~|&()";
    std::stack<std::string> Stack;
    
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
    const std::string GetStrFromStack();
    void ReplaceContent(const std::string src);
    inline void GetLex();
    void Run();
    char FindMatrixElement(char Y);
};


Var::Var(const std::string& src) :IdName{ src }{}

std::string Var::GetOperand()
{
    return this->IdName;
}

Reference::Reference(const long TriadRef)
{
    this->TriadRef = TriadRef;
}

std::string Reference::GetOperand()
{
    return '^' + std::to_string(this->TriadRef);
}

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

 void Analizer::GetSymbol()
{
     _currentSymbol = in.get();
}

 inline void Analizer::GetLex()
 {
     while (isspace(_currentSymbol)) GetSymbol();

     if (isalpha(_currentSymbol) || _currentSymbol == '_')
     {
         GetSymbol();
         while (isalnum(_currentSymbol) || _currentSymbol == '_')
         {
        
             GetSymbol();
         }
         _currentSymbol = 'I'; 
     }
     else if (isdigit(_currentSymbol))
     {
         GetSymbol();
         while (isdigit(_currentSymbol))
         {
             
             GetSymbol();
         }
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

     if (_Lex == 'I' && Stack.top().back() == ';')
     {
         _Lex = '#';

         if (GetStrFromStack() == "#S#")
         {
             std::cout << "Ready!";
             while (!Stack.empty()) {
                 std::cout << Stack.top();
                 Stack.pop();
             }
             Stack.push("#");
         }
         _Lex = 'I';
     }
 }
 const std::string  Analizer::GetStrFromStack()
 {
     std::stack<std::string> stack_copy(Stack);
     std::string str_buf;
     while (!stack_copy.empty()) {
         str_buf += stack_copy.top();
         stack_copy.pop();
     }
     return str_buf;
 }
 void Analizer::ReplaceContent(const std::string src)
 {
     while (!Stack.empty())
         Stack.pop();
     Stack.push(src);
 }
 void Analizer::Run()
 {
     GetSymbol();
     Stack.push("Q");
     do
     {
         GetLex();    /* получаем лексему   */
         std::cout << _Lex<<std::endl;    /* и печатаем лексему */
         if (GetStrFromStack() == "#S#")
         {
             std::cout << "Ready! " << GetStrFromStack() << std::endl;
             ReplaceContent("Q");
         }
     } while (_Lex != EOF);



 }
 char Analizer::FindMatrixElement(char Y)
 {
     char X = (char)Stack.top().c_str();
     int i = 0, j = 0;
     while (Matrix[i][0] != X)
         i++;

     while (Matrix[0][j] != Y)
         j++;

     if (j > MATRIX_SIZE)
         PrintError(TypeErrors::UNKOWN_SYMBOL, "No matrix element found");

     return Matrix[i][j];
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
     Analizer start;
     start.Run();
     in.close();
     out.close();
     outoptimization.close();
     return 0;
 }

 Constant::Constant(const long value)
 {
     this->ConstValue = value;
 }

 std::string Constant::GetOperand()
 {
     return std::to_string(this->ConstValue);
 }

 Triad::Triad(char operation, Base *op1, Base *op2):op1(op1),op2(op2),operation{operation}
 {}

 void Triad::OutTriad()
 {
     out << operation << '(' << op1->GetOperand() << ", " << op2->GetOperand() << ')' << std::endl;
 }

 None::None(){  }

 std::string None::GetOperand()
 {
     return std::string(1,'@');
 }

 Base::~Base()
 {
 }

 Rule::Rule(char currentS,const std::string src):left(currentS),right(src){}
