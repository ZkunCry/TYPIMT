#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

std::ifstream in;
std::ofstream out;
std::ofstream outoptimization;
int IdTriad = 0;


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

class Scanner
{
private:
    std::vector<Triad> TriadList;
    int _Lex = EOF;
    int _currentSymbol;
    const std::string _Native = "=;~|&()";
public:

    inline void GetSymbol(); //Get symbol from input file
    inline void GetLex();
    void Run(); //Run program
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

 void Scanner::GetSymbol()
{
     _currentSymbol = in.get();
}

 inline void Scanner::GetLex()
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
 }

 
 void Scanner::Run()
 {
     GetSymbol();
     do
     {
         GetLex();    /* получаем лексему   */
         std::cout << _Lex<<std::endl;    /* и печатаем лексему */
     } while (_Lex != '#');



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
     Scanner start;
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
