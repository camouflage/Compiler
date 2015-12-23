#include"statement.cpp"
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

void parse(ifstream& documentIfs) {
    starter(documentIfs);
    for ( int stmtNum = 0; stmtNum < tokenStream.size(); ++stmtNum ) {
        oneStream = tokenStream[stmtNum];
        init();

        aql_stmt();
    }
}
