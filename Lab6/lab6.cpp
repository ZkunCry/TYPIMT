#include <iostream>
#include <vector>
#include <string>
#include <fstream>

constexpr short int  MATRIX_SIZE = 16;

std::ifstream in;
std::ofstream out;
int IdTriad = 0;
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

class Base
{
public:
    virtual std::string GetOperand() { return ""; };
    virtual ~Base();

};

struct Var :public Base // Struct of identifier
{
private:
    std::string IdName;
public:
    Var(const std::string& src);
    std::string GetOperand()override;
};

struct Constant :public Base
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

class Triad
{
public:
    char operation;
    Base* op1;
    Base* op2;
    bool isDelete = false;
public:
    Triad(char operation, Base* op1, Base* op2);
    void OutTriad();

};
Var::Var(const std::string& src) :IdName{ src } {}

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

struct None :public Base
{
public:
    None();
    std::string GetOperand()override;
};

Constant::Constant(const long value)
{
    this->ConstValue = value;
}

std::string Constant::GetOperand()
{
    return std::to_string(this->ConstValue);
}

Triad::Triad(char operation, Base* op1, Base* op2) :op1(op1), op2(op2), operation{ operation }
{}

void Triad::OutTriad()
{
    out << operation << '(' << op1->GetOperand() << ", " << op2->GetOperand() << ')' << std::endl;
}

None::None() {  }

std::string None::GetOperand()
{
    return std::string(1, '@');
}

Base::~Base()
{
}


char Matrix[MATRIX_SIZE][MATRIX_SIZE] =
{
    {' ','L', 'S', 'E', 'T', 'M', '=', ';', '|', '&', '~', '(', ')', 'I', 'C','#'},
    {'L',' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<',' ','=' },
    {'S',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',' ', '>'},
    {'E',' ', ' ', ' ', ' ', ' ', ' ', '=', '=', ' ', ' ', ' ', '=', ' ',' ', ' '},
    {'T',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '=', ' ', ' ', '>', ' ',' ', ' '},
    {'M',' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', ' ', ' ', '>', ' ',' ', ' '},
    {'=',' ', ' ', '$', '<', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<','<', ' '},
    {';',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',' ', '>'},
    {'|',' ', ' ', ' ', '$', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'&',' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'~',' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
    {'(',' ', ' ', '$', '<', '<', ' ', ' ', ' ', ' ', '<', '<', ' ', '<', '<',' '},
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
    int ValueConstant;
    std::string nameIdentifier;
    int _idTriad;
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
    
    std::string name;
    int value;

    const std::string _Native = "=;~|&()";
    std::vector<Attitude> stack;
    std::vector<Var> varlist;
    std::vector<Triad> TriadList; 

    const Rule Rules[12] =
    {
        {'W',"#L#"},
        {'L',"LS"},
        {'L',"S"},
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
    int FindAttitude(const char symbol);
    bool GetVarValue(std::string& str); //Return identifier value
    bool GetVarAdress(std::string name); //Return  identifier address or add in array
    
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

inline void Translation::GetSymbol()
{
     _currentSymbol = in.get();
}

 inline void Translation::GetLex()
 {
     while (isspace(_currentSymbol)) GetSymbol();

     if (isalpha(_currentSymbol) || _currentSymbol == '_')
     {
         std::string temp;
         GetSymbol();
         while (isdigit(_currentSymbol))
         {
             temp += _currentSymbol;
             GetSymbol();

         }
         name = temp;
        _Lex = 'I';
     
     }
     else if (isdigit(_currentSymbol))
     {
         long x = 0;
         GetSymbol();
         while (_currentSymbol >='0' && _currentSymbol <='1')
         {
             x *= 2;
             x += _currentSymbol - '0';
             GetSymbol();
         }
         value = x;
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
         if (_Lex == ';')
         {
             for (auto it = stack.begin(); it != stack.end(); ++it)
                 std::cout << it->Symbol;
             std::cout << std::endl;
         }
         
     } while (_Lex != '#');
     std::cout << "Parsing was successful."  << std::endl;

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
     {
         Attitude attitude(symbol, _Lex);
         if (_Lex == 'C')
         {
             attitude.ValueConstant = value;
             Triad triad('C', new Var(name), new None());
             TriadList.push_back(triad);
             IdTriad++;

             attitude._idTriad = IdTriad;
         }
         else if (_Lex == 'I')
         {
             attitude.nameIdentifier = name;
             
             Triad triad('V', new Var(name), new None());
             TriadList.push_back(triad);
             IdTriad++;
             attitude._idTriad = IdTriad;
         }
         stack.push_back(attitude);
     }
         
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
inline char Translation::FindRules(std::string src)
 {

     for (int i = 0; i <= rulesCount; i++)
         if (src == Rules[i].right)
             return Rules[i].left;
     return NULL;
 }
 char Translation::RuleConvolution()
 {
     std::string base;
     char result;
     int posL = 0,tempposL=0;
     size_t _sizestack = stack.size();

     posL = FindAttitude('<');      
     if (posL == 0)
         posL = FindAttitude('$');

     while (true)
     {
         tempposL = posL;

         for(;posL< _sizestack;posL++)
            base.push_back(stack[posL].Symbol);
         GetVarValue(name);
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
                 if (stack[posL].SymbolAttitude == '$')
                     break;

             if (posL == stack.size())
                 PrintError(TypeErrors::SYNTAX_ERROR, "Couldn't find symbol < and <= .");
         }

     }
 }
 inline int Translation::FindAttitude(const char symbol)
 {
     for (int j = stack.size() - 1; j > 1; j--)
         if (stack[j].SymbolAttitude == symbol)
             return j;
 }

 bool Translation::GetVarValue(std::string& str)
 {
     if (str.size() == 0 || str.empty())
         PrintError(TypeErrors::SYNTAX_ERROR);

     for (int i = 0; i < varlist.size(); i++)
         if (varlist[i].GetOperand() == str)
             return true;

     PrintError(TypeErrors::UNDEF_ID, str);
   
 }

 bool Translation::GetVarAdress(std::string name)
 {
     if (name.empty() || name.size() == 0)
         PrintError(TypeErrors::SYNTAX_ERROR);

     for (int i = 0; i < varlist.size(); i++)
         if (varlist[i].GetOperand() == name)
             return true;
     varlist.push_back(Var{ name });
 
 }

 int main()
 {
     in.open("text.txt", std::ios::binary);
     out.open("translation.txt", std::ios::binary);
     if (!in.is_open() || !out.is_open())
     {
         std::cout << "Error! Can't open file " << std::endl;
         return 2;
     }

     Translation start;
     start.Run();
     in.close();
     out.close();

     return 0;
 }

 Rule::Rule(char currentS,const std::string src):left(currentS),right(src){}
