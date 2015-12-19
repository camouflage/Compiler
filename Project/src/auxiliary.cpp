#include"global.h"
#include<iostream>
#include<vector>
#include<string>
#include<map>
using namespace std;

// For global variable among auxiliary.cpp
string lastId = "!";

// Map Id to its alias
map<string, string> aliasMap;
// Map view to its content
map<string, map<string, vector<Word> > > view;
// viewId for create_stmt
string viewId;

// selectView & selectCol & selectColName for select_item
string selectView;
string selectCol;
struct selectInfo {
    string selectView;
    string selectCol;
};
string selectColName;

// selectMap
map<string, struct selectInfo> selectMap;


vector<Token> oneStream;
vector<Token>::iterator current;
int currentType;

void error() {
    cerr << "Syntax error!" << endl;
}

// Simple match and advances input
void match(int first) {
    if ( currentType == first ) {
        // Move forward
        ++current;
        currentType = current->tag;
    } else {
        error();
    }
}

// Match and insert alias
void matchInsertAlias(int first) {
    if ( currentType == first ) {
        if ( lastId == "!" ) {
            lastId = current->idReg;
        } else {
            aliasMap.insert(pair<string, string>(lastId, current->idReg));
        }
        // Move forward
        ++current;
        currentType = current->tag;
    } else {
        error();
    }
}

// Match and return id
void matchReturnId(int first, string& id) {
    if ( currentType == first ) {
        id = current->idReg;
        // Move forward
        ++current;
        currentType = current->tag;
    } else {
        error();
    }
}

void output() {
    map<string, string>::iterator it = aliasMap.find(lastId);
    if ( it != aliasMap.end() ) {
        cout << it->first << " " << it->second << endl;
    } else {
        cout << lastId << endl;
    }
}
