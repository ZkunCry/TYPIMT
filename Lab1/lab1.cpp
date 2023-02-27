
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
    Odinari_Quotes
};
enum class Literals
{
    TYPE_Quote = '"',
    TYPE_OdinaryQuote = '\''
};

static bool FileOpenCheck(ifstream &SourceFile)
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
int main()
{
    State states = State::Normal;
    ifstream SourceFile("Source.c");
    ofstream OutFile("Out.c");
    char c;
    if (FileOpenCheck(SourceFile))
    {
        while ((int)(c = SourceFile.get()) != EOF)
        {
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
                default:
                    OutFile << c;
                    break;
                }
                break;
            case State::Slash:
                if (c == '*')
                    states = State::Comment;
                else if (c == '/')
                {
                    states = State::Slash;
                    OutFile << '/';
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
                    states = State::Normal;
                else if (c == '*')
                    states = State::EndComment;
                else
                    states = State::Comment;
                
                break;
            case State::Double_Quotes:
                switch (c)
                {
                case (char)Literals::TYPE_Quote:
                    states = State::Normal;
                    OutFile << c;
                    break;
                default:
                    OutFile << c;
                    break;
                }
                break;

            case State::Odinari_Quotes:
                switch (c)
                {
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
