#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <format>

enum class TypeErrors
{
    SYNTAX_ERROR,
    MANY_VAR,
    UNDEF_ID,
    LONG_ID,
    ID_MISS,
    MISSING_SYMBOL,
};

typedef struct Var
{
    std::string IdName;
    long value;
}Var;
std::ifstream out;

typedef struct Analizer
{
    std::vector<Var> varlist;
    int c = EOF;
    int last = -1;

    inline void GetSymbol();
    long GetVarValue(std::string &str);
    long& GetVarAdress(std::string name);
    
    long MethodC();
    void MethodS();
    long& MethodL();
    long MethodE();
    long MethodT();
    long MethodM();
    long MethodI();

    void Print();
    void Run();
    
}Analizer;

static void PrintError(TypeErrors typeer, std::string param = nullptr)
{
    switch (typeer)
    {
    case TypeErrors::SYNTAX_ERROR:
        std::cout << "Error: Syntax error. "<< param << std::endl;
    case TypeErrors::LONG_ID:
        std::cout << "Error: "  << param << std::endl;
    case TypeErrors::ID_MISS:
        std::cout << "Error: "  << param << std::endl;
    case TypeErrors::MANY_VAR:
        std::cout << "Error: "  << param << std::endl;
    case TypeErrors::MISSING_SYMBOL:
        std::cout << std::format("Error: {} missing.", param)<<std::endl;
    }
    out.close();
    exit(1);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
       std::cout << "Command line parameter(file name) missing.\n" << std::endl;
       return 1;
    }
    out.open(argv[1], std::ios::binary);
    if (!out.is_open())
    {
        std::cout << "Error! Can't open file " << argv[1] << std::endl;
        return 2;
    }
    Analizer start;
    start.Run();
    out.close();
    return 0;
}

 void Analizer::GetSymbol()
{
    c = out.get();
}

 long Analizer::GetVarValue(std::string& str)
 {
     if (str.size() == 0 || str.empty())
         PrintError(TypeErrors::SYNTAX_ERROR);
     for (int i = 0; i < varlist.size(); i++)
         if (varlist[i].IdName == str)
             return varlist[i].value;
     PrintError(TypeErrors::UNDEF_ID ,str);
     
 }


 long& Analizer::GetVarAdress(std::string name)
 {
     if (name.empty() || name.size() == 0)
     {
         PrintError(TypeErrors::SYNTAX_ERROR);
     }
     for (int i = 0; i < varlist.size(); i++)
         if (varlist[i].IdName == name)
             return varlist[last = i].value;
     last = varlist.size();
     varlist[varlist.size()].IdName = name;
     return varlist[varlist.size()].value;
 }

 long Analizer::MethodC()
 {
     long x = 0;
     while (c >= '0' && c <= '1')
     {
         x *= 2;
         x += c - '0';
         this->GetSymbol();
     }
     return x;
 }

 void Analizer::MethodS()
 {
     long& p = MethodL();
     if (c != '=')
         PrintError(TypeErrors::MISSING_SYMBOL,"\'=\'");
     GetSymbol();
     p = MethodE();
     if (c != ';')
         PrintError(TypeErrors::MISSING_SYMBOL, "\';\'");
     GetSymbol();
 }

 long& Analizer::MethodL()
 {
     std::string str;
     int i = 0;
     while ((c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9'))
     {
         str.push_back(c);
         GetSymbol();
     }
     if (!i)
         PrintError(TypeErrors::ID_MISS);
     return GetVarAdress(str);
 }

 long Analizer::MethodE()
 {
     long x = MethodT();
     while (c == '|')
     {
         char p = c;
         GetSymbol();
         if (p == '|')
             x |= MethodT();
     }
     return x;
 }

 long Analizer::MethodT()
 {
     long x = MethodM();
     while (c == '&')
     {
         char p = c;
         GetSymbol();
         if (p == '&')
             x &= MethodM();
     }
     return x;
 }

 long Analizer::MethodM()
 {
     long result;
     if (c == '(')
     {
         GetSymbol();
         result = MethodE();
         if (c != ')')
             PrintError(TypeErrors::MISSING_SYMBOL,"\')\'");
         GetSymbol();
     }
     else
     {
         if (c == '~')
         {
             GetSymbol();
             result = ~MethodM();
         }
         else
             if (c >= '0' && c <= '1')
                 result = MethodC();
             else
                 if ((c >= 'a' && c <= 'z') || c == '_')
                     result = MethodI();
                 else
                     PrintError(TypeErrors::SYNTAX_ERROR);
     }
     return result;
 }

 long Analizer::MethodI()
 {
     std::string str;
     int i = 0;
     while ((c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '1'))
     {
         str.push_back(c);
         GetSymbol();
     }
     if (!i)
         PrintError(TypeErrors::ID_MISS);
     return GetVarValue(str);
 }

 void Analizer::Print()
 {
     if (last < 0)
         std::cout << "No varibales defined yet.\n";
     else
         std::cout << varlist[last].IdName << " = " << varlist[last].value;
 }

 void Analizer::Run()
 {
     int i = 0;
     GetSymbol();
     for (i;; i++)
     {
         while(c >=0 && c <=' ')
             GetSymbol();
         if (c == EOF)
             break;
         std::cout << "Operator: " << i + 1;
         MethodS();
         Print();
     }
     std::cout << std::format("Result ({} variables define in {} operators):\n", varlist.size(), i);
 }
