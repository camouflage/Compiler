#include"lexer.cpp"
#include"parser.cpp"
#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<vector>
using namespace std;

map<string, Token> symbol;
vector<vector<Token> > tokenStream;

void AQL(ifstream& aqlIfs, ifstream& documentIfs) {
    // lexer
    lex(aqlIfs);
    // parser
    parse(documentIfs);
}
