#include"global.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;


void aql_stmt();
void create_stmt();
void output_stmt();
void optalias();

vector<Token> oneStream;
vector<Token>::iterator current;
int currentType;

void error() {
    cerr << "Syntax error!" << endl;
}

void match(int first) {
    if ( currentType == first ) {
        // move forward
        ++current;
        currentType = current->tag;
    } else {
        error();
    }
}

void aql_stmt() {
    switch ( currentType ) {
        case CREATE:
            create_stmt();
            match(';');
            break;
        case OUTPUT:
            output_stmt();
            match(';');
            break;
        default:
            error();
            break;
    }
}

void create_stmt() {

}

void output_stmt() {
    switch ( currentType ) {
        case OUTPUT:
            match(OUTPUT);
            match(VIEW);
            match(ID);
            optalias();
            break;
        default:
            error();
            break;
    }
}

void optalias() {
    switch ( currentType ) {
        case AS:
            match(AS);
            match(ID);
            break;
        default:
            break;
    }
}
