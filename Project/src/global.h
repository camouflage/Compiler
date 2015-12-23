#ifndef GLOBAL
#define GLOBAL

#include"tag.h"
#include"word.h"
#include"pre_token.h"
#include"token.cpp"
#include<map>
#include<vector>
using namespace std;

extern map<string, Token> symbol;
extern vector<vector<Token> > tokenStream;

#endif