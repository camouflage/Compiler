#ifndef GLOBAL
#define GLOBAL

#include"tag.h"
#include"word.h"
#include"pre_token.h"
#include"token.cpp"
#include<map>
#include<vector>
using namespace std;

extern vector<vector<Token> > tokenStream;
extern ifstream documentIfs;

#endif