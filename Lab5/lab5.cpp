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

    std::vector<Var> varlist; //List of the identifiers
    std::vector<Triad> TriadList;
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
    void Optimization();
        
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
         if (varlist[i].GetOperand() == str)
         {
             Triad triad('V', new Var{ str }, new None{ });
             TriadList.push_back(triad);
             IdTriad++;
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
         if (varlist[i].GetOperand() == name)
         {
             Triad triad('V', new Var{ name }, new None{});
             TriadList.push_back(triad);
             IdTriad++;
             return IdTriad;
         }
     varlist.push_back(Var{name});
     Triad triad('V', new Var{ name }, new None{});
     TriadList.push_back(triad);
     IdTriad++;
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
     
     Triad triad('C', new Constant{ x }, new None{});
     TriadList.push_back(triad);
     IdTriad++;
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
     Triad triad('=', new Reference{ p }, new Reference{ p2 });
     TriadList.push_back(triad);
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
         
         Triad triad('|', new Reference{ x }, new Reference{ x2 });
         TriadList.push_back(triad);
         IdTriad++;
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
         Triad triad('&', new Reference{ x }, new Reference{ x2 });
         TriadList.push_back(triad);
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
             Triad triad('~', new Reference{ result }, new None());
             TriadList.push_back(triad);
             IdTriad++;
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
     i = 0;
     for (int i = 0; i < TriadList.size(); i++)
     {
         out << i+1 << " : ";
         TriadList[i].OutTriad();
     }
     Optimization();
     int j = 0;
     for (int i = 0; i < TriadList.size(); i++)
     {
         if (!TriadList[i].isDelete)
         {
             outoptimization << i+1 << " : ";
             outoptimization << TriadList[i].operation << '(' << TriadList[i].op1->GetOperand() << ", " << TriadList[i].op2->GetOperand() << ')' << std::endl;
         }
       
     }


 }
 void Analizer::Optimization()
 {
     int index = 0,index2=0;
     for (int i = 0; i < TriadList.size(); i++)
     {
         index = std::atoi(TriadList[i].op1->GetOperand().c_str() + 1);
         index2 = std::atoi(TriadList[i].op2->GetOperand().c_str() + 1);
         index--;
         index2--;
         if (TriadList[i].operation == '|' || TriadList[i].operation == '&' )
         {
                 
                 if (TriadList[index2].operation == 'C')
                 {
                     TriadList[index2].isDelete = true;
                     delete TriadList[i].op2;
                     TriadList[i].op2 = new Constant(std::atoi(TriadList[index2].op1->GetOperand().c_str()));
                     if (TriadList[index].operation == 'C')
                     {
                         TriadList[index].isDelete = true;

                         TriadList[i].op1 = new Constant(std::atoi(TriadList[index].op1->GetOperand().c_str()));
                         
                         if (TriadList[i].operation == '|')
                         {
                             TriadList[i].operation = 'C';
                            

                             TriadList[i].op1 = new Constant(std::atoi(TriadList[i].op1->GetOperand().c_str()) |
                                 (std::atoi(TriadList[i].op2->GetOperand().c_str())));
                             TriadList[i].op2 = new None();
                         }
                         else
                         {
                             TriadList[i].operation = 'C';
                             TriadList[i].op1 = new Constant(std::atoi(TriadList[i].op1->GetOperand().c_str()) &
                                 (std::atoi(TriadList[i].op2->GetOperand().c_str())));
                             TriadList[i].op2 = new None();
                         }
                     }
                 }
                 else if (TriadList[index].operation == 'C')
                 {
                     TriadList[index].isDelete = true;
                     TriadList[i].op1 = new Constant(std::atoi(TriadList[index].op1->GetOperand().c_str()));
                 }
         }
         else if (TriadList[i].operation == '~')
         {
             if (TriadList[index].operation == 'C')
             {
                 TriadList[index].isDelete = true;
                 TriadList[i].op1 = new Constant(~(std::atoi(TriadList[index2].op1->GetOperand().c_str())));
                 TriadList[i].operation = 'C';
             }
         }
         else if (TriadList[i].operation == '=')
         {
             if (TriadList[index].operation == 'V')
             {
                 TriadList[index].isDelete = true;
                 TriadList[i].op1 =new Var(TriadList[index].op1->GetOperand());
             }
         }
     }
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
