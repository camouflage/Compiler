#include"global.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;


void error() {
    cerr << "Syntax error!" << endl;
}

bool match(int lookahead, int first) {
    if ( lookahead == first ) {
        return true;
    } else {
        error();
        return false;
    }
}

void parse() {
    for ( int stmtNum = 0; stmtNum < tokenStream.size(); ++stmtNum ) {
        vector<Token> oneStream = tokenStream[stmtNum];
        Token current = oneStream[0];
    }
}
