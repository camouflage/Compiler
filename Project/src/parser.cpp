#include"statement.cpp"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

void parse() {
    for ( int stmtNum = 0; stmtNum < tokenStream.size(); ++stmtNum ) {
        oneStream = tokenStream[stmtNum];
        current = oneStream.begin();
        currentType = current->tag;
        // Clear the alias map
        aliasMap.clear();
        lastId = "!";

        aql_stmt();

    }
}
