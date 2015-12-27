#include"lexer.cpp"
#include"parser.cpp"
#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<vector>
using namespace std;

vector<vector<Token> > tokenStream;
ifstream documentIfs;
ofstream outputIfs;

void AQL(ifstream& aqlIfs) {
    // lexer
    lex(aqlIfs);
    // parser
    parse();
}
