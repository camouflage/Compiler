#include"global.h"
#include"auxiliary.cpp"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

void aql_stmt();
void create_stmt();
void output_stmt();
void optalias();

void aql_stmt() {
    switch ( currentType ) {
        case CREATE:
            create_stmt();
            match(';');
            break;
        case OUTPUT:
            output_stmt();
            match(';');
            // Print
            output();
            break;
        default:
            error();
            break;
    }
}

void create_stmt() {
    switch ( currentType ) {
        case CREATE:
            match(CREATE);
            match(VIEW);
        default:
            error();
    }
}

void output_stmt() {
    switch ( currentType ) {
        case OUTPUT:
            match(OUTPUT);
            match(VIEW);
            matchInsertAlias(ID);
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
            matchInsertAlias(ID);
            break;
        default:
            break;
    }
}
