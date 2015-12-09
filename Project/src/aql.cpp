#include"lexer.cpp"
#include<iostream>
#include<fstream>
#include<map>
#include<string>
using namespace std;

void AQL(ifstream& aqlIfs, ifstream& documentIfs) {
    // lexer
    lex(aqlIfs);
}


