#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::ifstream in;
std::ofstream out;
int IdTriad = 0;

enum class TypeErrors //Error codes
{
    SYNTAX_ERROR,
    UNDEF_ID,
    ID_MISS,
    MISSING_SYMBOL,
};

struct Var // Struct of identifier
{
private:
    std::string IdName; //Identifier name
    long value;//Identifier value
public:
   Var(const std::string& src, long value);
   std::string Get_IdName();
};
struct Constant
{

};
struct Reference
{
public:
    long TriadRef;
    Reference(const long TriadRef);
};





class Analizer
{
private:
    std::vector<Var> varlist; //List of the identifiers
    int c = EOF;
public:
    inline void GetSymbol(); //Get symbol from input file
    long GetVarValue(std::string &str); //Return identifier value
    long GetVarAdress(std::string name); //Return  identifier address or add in array

    long MethodC();
    void MethodS();
    long MethodL();
    long MethodE();
    long MethodT();
    long MethodM();
    long MethodI();
    void Run(); //Run program
        
};


Var::Var(const std::string& src, long value) :IdName{ src }, value{ value }{}

std::string Var::Get_IdName()
{
    return this->IdName;
}

Reference::Reference(const long TriadRef)
{
    this->TriadRef = TriadRef;
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
    }
    in.close();
    exit(1);
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
         if (varlist[i].Get_IdName() == str)
         {
             IdTriad++;
             out << IdTriad << " : " << "V(" << str << ", @)\n";
             return IdTriad;
         }

     PrintError(TypeErrors::UNDEF_ID ,str);
     return 0;
 }

 long Analizer::GetVarAdress(std::string name)
 {
     if (name.empty() || name.size() == 0)
         PrintError(TypeErrors::SYNTAX_ERROR);

     for (int i = 0; i < varlist.size(); i++)
         if (varlist[i].Get_IdName() == name)
         {
             IdTriad++;
             out << IdTriad << " : " << "V(" << name << ", @)\n";
             return IdTriad;
         }
     varlist.push_back(Var{name,0});
     IdTriad++;
     out << IdTriad << " : " << "V(" << name << ", @)\n";
     return IdTriad;
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
     IdTriad++;
     out << IdTriad << " : " << "C(" << x << ", @)\n";
     return IdTriad;
 }

void Analizer::MethodS()
{
     long p = MethodL();
     if (c != '=')
         PrintError(TypeErrors::MISSING_SYMBOL,"\'=\'");
     GetSymbol();
     long p2 = MethodE();
     if (c != ';')
         PrintError(TypeErrors::MISSING_SYMBOL, "\';\'");
     IdTriad++;
     out << IdTriad << " : " << "=(^" << p << ", ^"<<p2<<")\n";
     GetSymbol();
}

 long Analizer::MethodL()
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
     long x2;
     while (c == '|')
     {
         GetSymbol();
         x2 = MethodT();
         IdTriad++;
         out << IdTriad << " : " << "|(^" << x << ", " << "^" << x2 << ")\n";
         x = IdTriad;
     }
    
     return x;
 }

 long Analizer::MethodT()
 {
     long x = MethodM();
     long x2;
     while (c == '&')
     {
         GetSymbol();
         x2 = MethodM();
         IdTriad++;
         out << IdTriad << " : " << "&(^" << x << ", ^" << x2 << ")\n";
         x = IdTriad;
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
             result = MethodM();
             IdTriad++;
             out << IdTriad << " : " << "~(^" << result << ", @)\n";
             result = IdTriad;
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
         MethodS();
     }

 }
 int main()
 {
     in.open("text.txt", std::ios::binary);
     out.open("translation.txt");
     if (!in.is_open() || !out.is_open())
     {
         std::cout << "Error! Can't open file " << std::endl;
         return 2;
     }
     Analizer start;
     start.Run();
     in.close();
     out.close();
     return 0;
 }
