#include"global.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

#define SPACE 0;
#define DIGIT 1;

map<string, Token> symbol;
int line = 1;
int col = 1;

int getType(char c) {
    if ( c == '\t' ) {
        col += 3;
        return SPACE;
    } else if ( c == '\r' ) {
        col = 1;
        return SPACE;
    } else if ( c == '\n' ) {
        ++line;
        return SPACE;
    } else if ( c == ' ' ) {
        ++col;
        return SPACE;
    } else if ( c >= '0' && c <= '9' ) {
        ++col;
        return DIGIT;
    } else if ( )


     || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
    // other letters , exp: , . ( )
    else return 2;
}

void reserve() {
    Token create(CREATE);
    symbol.insert(pair<string, Token>("create", create));
    Token view(VIEW);
    symbol.insert(pair<string, Token>("view", view));
    Token as(AS);
    symbol.insert(pair<string, Token>("as", as));
    Token output(OUTPUT);
    symbol.insert(pair<string, Token>("output", output));
    Token select(SELECT);
    symbol.insert(pair<string, Token>("select", select));
    Token from(FROM);
    symbol.insert(pair<string, Token>("from", from));
    Token extract(EXTRACT);
    symbol.insert(pair<string, Token>("extract", extract));
    Token regex(REGEX);
    symbol.insert(pair<string, Token>("regex", regex));
    Token on(ON);
    symbol.insert(pair<string, Token>("on", on));
    Token _return(RETURN);
    symbol.insert(pair<string, Token>("return", _return));
    Token group(GROUP);
    symbol.insert(pair<string, Token>("group", group));
    Token _and(AND);
    symbol.insert(pair<string, Token>("and", _and));
    Token token(TOKEN);
    symbol.insert(pair<string, Token>("token", token));
    Token pattern(PATTERN);
    symbol.insert(pair<string, Token>("pattern", pattern));
}

vector<Token> lex(ifstream& ifs) {
    // Reserve keywords
    reserve();

    char current;
    while ( (current = ifs.get()) != EOF ) {
        cout << current;
    }

    // cout << symbol.find("pattern")->second.tag << endl;

    vector<Token> v;
    return v;
}

