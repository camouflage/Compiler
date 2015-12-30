// Global includes some headers and global variables.
#ifndef GLOBAL
#define GLOBAL

#include"tag.h"
#include"word.h"
#include"pre_token.h"
#include"token.h"
#include"PatternClass.h"
using namespace std;

// Global variables
// tokenStream stores the tokens returned by lexer 
extern vector<vector<Token> > tokenStream;
// Stores document and output 
extern ifstream documentIfs;
extern ofstream outputOfs;

#endif