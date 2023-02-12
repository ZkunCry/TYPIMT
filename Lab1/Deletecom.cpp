
#include <iostream>
#include <fstream>
#include <ostream>
using namespace std;

enum class State
{
    Normal,
    Slash,
    Comment,
    CarriageReturn,
    Tabulation,
    EndComment,
    Double_Quotes,
    Odinari_Quotes
};
enum class Literals
{
    TYPE_Quote = '"',
    TYPE_OdinaryQuote = '\''
};

static void FileOpenCheck(ifstream &SourceFile)
{
    if (SourceFile.is_open())
        cout << "File is open\n";
    else
        cout << "File is not open. Incorrect name or does not exist\n";
}
int main()
{
    State states = State::Normal;
    ifstream SourceFile("Source.c");
    FileOpenCheck(SourceFile);
    ofstream OutFile("Out.c");
    char c;
    
    while ((int)(c = SourceFile.get())!=EOF)
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
            break;

        case State::CarriageReturn:
           
            OutFile << c;
            states = State::Normal;
            break;
        case State::Tabulation:
            OutFile << c;
            states = State::Normal;
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
    SourceFile.close();
    OutFile.close();
    return 0;
}
