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

void init() {
    current = oneStream.begin();
    currentType = current->tag;

    // Clear the alias map
    aliasMap.clear();
    lastId = "!";

    // Clear view
    view.clear();

    // Clear the select map
    selectMap.clear();

    // View Loc for test
    vector<Word> vw;
    Word w1;
    w1.content = "G";
    vw.push_back(w1);
    w1.content = "W";
    vw.push_back(w1);
    w1.content = "V";
    vw.push_back(w1);
    map<string, vector<Word> > msv;
    msv["Cap"] = vw;

    vw.clear();
    w1.content = "P,G";
    vw.push_back(w1);
    w1.content = "G,W";
    vw.push_back(w1);
    w1.content = "W,V";
    vw.push_back(w1);
    msv["Loc"] = vw;

    view["Loc"] = msv;
}

void error() {
    cerr << "Syntax error!" << endl;
}

void notFoundError(string symbol) {
    cerr << "Symbol " << symbol << " not found!" << endl;
    exit(1);
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
            aliasMap.insert(pair<string, string>(current->idReg, lastId));
            lastId = "!";
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
