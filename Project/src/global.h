#ifndef GLOBAL
#define GLOBAL

#include"tag.h"
#include"word.h"
#include"token.cpp"
#include<map>
#include<vector>
using namespace std;

extern map<string, Token> symbol;
extern vector<vector<Token> > tokenStream;

#endif