
#include <iostream>
#include <fstream>
#include <ostream>
using namespace std;

enum class State
{
    Normal,
    Slash,
    Comment,
    EndComment,
    Double_Quotes,
    Odinari_Quotes,
    EndAloneComment,
    PredDoubleQuote,
    PredOdQuote,
    Number,
    Hex,
    Octal,
    Skip,
    Identifier,
    UnsignedInt,
    UnsignedLongInt,
    UnsignedLongLongInt,
    LongInt,
    LongLongint,
    Decimal,
    PreX
};
enum class Literals
{
    TYPE_Quote = '"',
    TYPE_OdinaryQuote = '\''
};
 static bool FileOpenCheck(ifstream& SourceFile)
{
    if (SourceFile.is_open())
        cout << "File is open\n";
    else
    {
        cout << "File is not open. Incorrect name or does not exist\n";
        return 1;
    }
    return 1;
}
static const bool isOctal(char pc)
{
    if (isdigit(pc))
    {
        int num = atoi(&pc);
        return num >= 0 && num <= 7;
    }
    else return false;
}
int main()
{
    State states = State::Normal;
    ifstream SourceFile("Source.cpp");
    ofstream OutFile("Out.cpp");
    char c;

    if (FileOpenCheck(SourceFile))
    {

        while ((int)(c = SourceFile.get()) != EOF)
        {
            if ((isdigit(c) && c !='0') && states == State::Normal )
            {
                states = State::Decimal;
                OutFile << c;
                continue;
            }
            if ((isalpha(c) || c == '_') && states == State::Normal)
            {
                states = State::Identifier;
                OutFile << c;
             
            }
            switch (states)
            {
            case State::Normal:
                switch (c)
                {
                case '/':
                    states = State::Slash;
                    break;
                case (char)Literals::TYPE_Quote:
                    states = State::Double_Quotes;
                    OutFile << c;
                    break;
                case (char)Literals::TYPE_OdinaryQuote:
                    states = State::Odinari_Quotes;
                    OutFile << c;
                    break;
                case '0':
                    states = State::Number;
                    OutFile << c;
                    break;
                default:
                    OutFile << c;
                    break;
                }
                break;
            case State::Decimal:
                if (toupper(c) == 'U')
                {
                    states = State::UnsignedInt;
                    OutFile << c;
                }
                else if (toupper(c) == 'L')
                {
                    states = State::LongInt;
                    OutFile << c;
                }
                else  if (isdigit(c))
                {
                    states = State::Decimal;
                    OutFile << c;
                 }
                else if (isalpha(c) || c == '.' || c == ',' || c == '?' || c == '$')
                {
                    states = State::Skip;
                    OutFile << c << '\t' << "  ERROR!";
                }
                else if (!isalpha(c) && !isdigit(c))
                {
                     states = State::Normal;
                     OutFile << '\t' << "int" << c;
                     break;
                 }
                 break;
            case State::Number:
                if (tolower(c) == 'x')
                {
                    states = State::PreX;
                    OutFile << c;
                }
                else if (isOctal(c))
                {
                    states = State::Octal;
                    OutFile << c;
                  
                }
                else if(toupper(c) == 'U')
                {
                    states = State::UnsignedInt;
                    OutFile << c;
                }
                else if (toupper(c) == 'L')
                {
                    states = State::LongInt;
                    OutFile << c;
                }
                else if (c == '.' || c == ',')
                {
                    states = State::Skip;
                    OutFile << c << "\tERROR!!";
                }
                else if (!isalpha(c) && !isdigit(c))
                {
                    states = State::Normal;
                    OutFile  << "\tint"<<c;
                }
              
                break;
            case State::PreX:
                if (isxdigit(c))
                {
                    states = State::Hex;
                    OutFile << c;
                }
                else
                {
                    states = State::Skip;
                    OutFile << c << '\t' << "  ERROR!";
                }
                break;

            case State::Hex:
                 if (toupper(c) == 'U')
                {
                    states = State::UnsignedInt;
                    OutFile << c;
                }
                else if (toupper(c) == 'L')
                {
                    states = State::LongInt;
                    OutFile << c;
                }
                
                else if (!isalpha(c) && !isdigit(c) && c !='.' && c !=',' && c !='?' && c !='$')
                {
                    states = State::Normal;
                    OutFile << '\t' << "int" << c;
                }
                else if (!isxdigit(c) || c == '.')
                 {
                     states = State::Skip;
                     OutFile << c << '\t' << "  ERROR!";
                 }
                else
                {
                     states = State::Hex;
                     OutFile << c;
                }
                break;
            case State::Octal:
               if (toupper(c) == 'U')
                {
                    states = State::UnsignedInt;
                    OutFile << c;
                }
                else if (toupper(c) == 'L')
                {
                    states = State::LongInt;
                    OutFile << c;
                }
                
                else  if (!isalpha(c) && !isdigit(c) && c != '.' && c != ',' && c != '?' && c != '$')
                {
                    states = State::Normal;
                    OutFile << '\t' << "int" << c;

                }
                else if (!isOctal(c) || c == '.' || isalpha(c))
               {
                   states = State::Skip;
                   OutFile << c << '\t' << "  ERROR";
               }
                else
                {
                    OutFile << c;
                    states = State::Octal;
                }
                break;
            case State::UnsignedInt:

                if (toupper(c) == 'L')
                {
                    states = State::UnsignedLongInt;
                    OutFile << c;
                }
                else if (isalpha(c) || isdigit(c))
                {
                    states = State::Skip;
                    OutFile << c << '\t' << c << "  ERROR!";
                }
                else
                {
                    states = State::Normal;
                    OutFile << '\t' << "unsigned int" << c;
                }
                break;
            case State::UnsignedLongInt:
                if (toupper(c) == 'L')
                {
                    states = State::UnsignedLongLongInt;
                    OutFile << c;
                }
                else if (isalpha(c) || isdigit(c))
                {
                    states = State::Skip;
                    OutFile << c << '\t' << c << "  ERROR!";
                }
                else
                {
                    states = State::Normal;
                    OutFile << '\t'  << "unsigned long int"<<c;
                }
                break;
            case State::UnsignedLongLongInt:
                if (isalpha(c) || isdigit(c))
                {
                    states = State::Skip;
                    OutFile << c << '\t' << c << "  ERROR!";

                }
                else
                {
                    states = State::Normal;
                    OutFile << '\t' << "unsigned long long int" << c;
                }
                break;
            case State::LongInt:
                if (toupper(c) == 'L')
                {
                    states = State::LongLongint;
                    OutFile << c;
                }
                else if (isalpha(c) || isdigit(c))
                {
                    states = State::Skip;
                    OutFile << c << '\t' << c << "  ERROR!";
                }
                else
                {
                    states = State::Normal;
                    OutFile << '\t' << "long int" << c;
                }
                break;
            case State::LongLongint:
                if (isalpha(c) || isdigit(c))
                {
                    states = State::Skip;
                    OutFile << c << '\t' << c << "  ERROR!";
                }
                else
                {
                    states = State::Normal;
                    OutFile << '\t' << "long long int" << c;
                }
                break;
            case State::Skip:
                if (!isalpha(c) && !isdigit(c))
                {
                    states = State::Normal;
                    OutFile << c;
                    break;
                }
                else
                {
                    states = State::Skip;
                    break;
                }
            case State::Identifier:
              
                if (isalpha(c) || isdigit(c) || c == '_' )
                {
                    OutFile << c;
                    states = State::Identifier;
                }
                else  if (!isalpha(c) && !isdigit(c))
                {
                    states = State::Normal;
                    OutFile << c;
                }
                else
                {
                    states = State::Skip;
                    OutFile << c << '\t' << "  ERROR";
                }
                break;
            case State::Slash:
                if (c == '*')
                    states = State::Comment;
                else if (c == '/')
                {
                    states = State::EndAloneComment;
                }
                else
                {
                    states = State::Normal;
                    OutFile << '/';
                    OutFile << c;
                }
                break;

            case State::Comment:
                if (c == '*')
                    states = State::EndComment;
                break;

            case State::EndComment:
                if (c == '/')
                {
                    states = State::Normal;
                    OutFile << ' ';
                }
                else if (c == '*')
                    states = State::EndComment;
                else
                    states = State::Comment;

                break;
            case State::EndAloneComment:
                if (c == '\n')
                {
                    states = State::Normal;
                    OutFile << '\n';
                }
                else if (c == '\r')
                {
                    states = State::Normal;
                    OutFile << '\r';
                }
                break;
            case State::Double_Quotes:
                switch (c)
                {
                 case '\\':
                    OutFile << c;
                    states = State::PredDoubleQuote;
                    break;
                case (char)Literals::TYPE_Quote:
                    states = State::Normal;
                    OutFile << c;
                    break;
                
                default:
                    OutFile << c;
                    break;
                }
                break;
            case State::PredDoubleQuote:
                OutFile << c;
                states = State::Double_Quotes;
                break;
            case State::PredOdQuote:
                OutFile << c;
                states = State::Odinari_Quotes;
                break;
            case State::Odinari_Quotes:
                switch (c)
                {
                case '\\':
                    OutFile << c;
                    states = State::PredOdQuote;
                    break;
                case (char)Literals::TYPE_OdinaryQuote:
                    states = State::Normal;
                    OutFile << c;
                    break;
                default:
                    OutFile << c;
                    break;
                }
                break;
            }
        }
    }
    SourceFile.close();
    OutFile.close();
    return 0;
}
