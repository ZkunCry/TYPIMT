
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
    EndComment
};
void FileOpenCheck(ifstream SourceFile)
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
    ofstream OutFile("Out.c");
    char c = SourceFile.get();
    while (!SourceFile.eof())
    {
        
        switch (states)
        {
        case State::Normal:
            if (c == '/')
                states = State::Slash;
            else if (c == '\n')
                states = State::CarriageReturn;
            else if (c == '\t')
                states = State::Tabulation;
            else
                OutFile << c;
            break;
        case State::Slash:
            if (c == '*')
                states = State::Comment;
            else
                states = State::Normal;
            break;
        case State::Comment:
            if (c == '*')
                states = State::EndComment;
            else
                states = State::Normal;
            break;
        case State::EndComment:
            if (c == '/' && states == State::Comment)
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
        }
        c = SourceFile.get();
    }
    SourceFile.close();
    OutFile.close();
    return 0;
}
