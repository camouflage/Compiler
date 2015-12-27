#include"statement.cpp"
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

void parse() {
    // Before parsing
    starter(documentIfs);
    for ( int stmtNum = 0; stmtNum < tokenStream.size(); ++stmtNum ) {
        // Get one statement
        oneStream = tokenStream[stmtNum];
        // Init variables before each statement
        init();
        // Parse
        aql_stmt();
    }

    // Close file stream
    documentIfs.close();
    outputOfs.close();
}
