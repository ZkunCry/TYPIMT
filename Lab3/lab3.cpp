#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <format>

enum class TypeErrors //Error codes
{
    SYNTAX_ERROR,
    UNDEF_ID,
    ID_MISS,
    MISSING_SYMBOL,
};

struct Var // Struct of identifier
{
public:
    std::string IdName; //Identifier name
    long value;//Identifier value
public:
   Var(const std::string& src, long value);
};

std::ifstream in;

class Analizer
{
private:
    std::vector<Var> varlist; //List of the identifiers
    int c = EOF;
    int last = -1; //Index of last identifier
public:
    inline void GetSymbol(); //Get symbol from input file
    long GetVarValue(std::string &str); //Return identifier value
    long& GetVarAdress(std::string name); //Return  identifier address or add in array

    long MethodC();
    void MethodS();
    long& MethodL();
    long MethodE();
    long MethodT();
    long MethodM();
    long MethodI();

    void Print(); //Output of one identifier
    void PrintAll(); //Output of all identifiers
    void Run(); //Run program
        
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
    }
    in.close();
    exit(1);
}

int main()
{
    in.open("text.txt", std::ios::binary);
    if (!in.is_open())
    {
        std::cout << "Error! Can't open file "<< std::endl;
        return 2;
    }

    Analizer start;
    start.Run();
    in.close();
    return 0;
}

 void Analizer::GetSymbol()
{
    c = in.get();
}

 long Analizer::GetVarValue(std::string& str)
 {
     if (str.size() == 0 || str.empty())
         PrintError(TypeErrors::SYNTAX_ERROR);

     for (int i = 0; i < varlist.size(); i++)
         if (varlist[i].IdName == str)
             return varlist[i].value;

     PrintError(TypeErrors::UNDEF_ID ,str);
     return 0;
     
 }

 long& Analizer::GetVarAdress(std::string name)
 {
     if (name.empty() || name.size() == 0)
         PrintError(TypeErrors::SYNTAX_ERROR);

     for (int i = 0; i < varlist.size(); i++)
         if (varlist[i].IdName == name)
             return varlist[last=i].value;
     
     varlist.push_back(Var{name,0});
     last = varlist.size()-1;
     return varlist[varlist.size() - 1].value;
 }

 long Analizer::MethodC()
 {
     long x = 0;
     while (c >= '0' && c <= '1') // Translation from binary system to decimal
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
     bool flag = false;
     if ((c >= 'a' && c <= 'z') || c == '_')
     {
         str.push_back(c);
         GetSymbol();
         if ((c >= 'a' && c <= 'z') || c == '_')
             flag = false;
         else
             flag = true;
         while ((c >= '0' && c <= '1'))
         {
             str.push_back(c);
             GetSymbol();
         }
     }
     if (!flag)
         PrintError(TypeErrors::ID_MISS, std::string(1, (char)c));
     return GetVarAdress(str);
 }

 long Analizer::MethodE()
 {
     long x = MethodT();
     while (c == '|')
     {
         GetSymbol();
         x |= MethodT();
     }
     return x;
 }

 long Analizer::MethodT()
 {
     long x = MethodM();
     while (c == '&')
     {
         GetSymbol();
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
     bool flag = false;
     if ((c >= 'a' && c <= 'z') || c == '_' )
     {

         str.push_back(c);
         GetSymbol();
         if ((c >= 'a' && c <= 'z') || c == '_')
             flag = false;
         else
             flag = true;
         while ((c >= '0' && c <= '1'))
         {
             str.push_back(c);
             GetSymbol();
         }
     }
     if (!flag)
         PrintError(TypeErrors::ID_MISS,std::string(1,(char)c));
     return GetVarValue(str);
 }

 void Analizer::Print()
 {
     if (varlist.size() == 0)
         std::cout << "No varibales defined yet.\n";
     else
         std::cout << varlist[last].IdName << " = " << varlist[last].value << std::endl;
 }

 void Analizer::PrintAll()
 {
     if (!varlist.size())
         printf("No variables defined yet.\n");
     else
         for (int i = 0; i < varlist.size(); i++)
             std::cout<<varlist[i].IdName<< " = " << varlist[i].value<<std::endl;
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
         std::cout << "Operator: " << i + 1<<std::endl;
         MethodS();
         Print();
     }
     std::cout << "Result (" << varlist.size() << " variables define in " << i << " operators)\n";
     PrintAll();
 }

 Var::Var(const std::string& src, long value):IdName{src},value{value}{}
