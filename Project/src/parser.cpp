#include"statement.cpp"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

void parse() {
    test();
    for ( int stmtNum = 0; stmtNum < tokenStream.size(); ++stmtNum ) {
        oneStream = tokenStream[stmtNum];
        init();

        aql_stmt();
    }
}
