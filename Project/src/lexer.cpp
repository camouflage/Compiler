#include"global.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

map<string, Token> symbol;

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
    // cout << symbol.find("pattern")->second.tag << endl;

    vector<Token> v;
    return v;
}

