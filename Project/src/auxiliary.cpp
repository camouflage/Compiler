#include"global.h"
#include"pattern.cpp"
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<cstdlib>
using namespace std;

// Map view to its content
map<string, map<string, vector<Word> > > view;

// For global variable among auxiliary.cpp
string lastId = "!";
// Map Id to its alias
map<string, string> aliasMap;

// viewId for create_stmt
string viewId;

// selectView & selectCol & selectColName for select_item
string selectView;
string selectCol;
struct selectInfo {
    string selectView;
    string selectCol;
} si;

// colName
string colName;

// selectMap
map<string, struct selectInfo> selectMap;

// documentAlias for regex_spec
string documentAlias;

int parenNum;


vector<Token> oneStream;
vector<Token>::iterator current;
int currentType;

void starter(ifstream& documentIfs) {
    pre_tokenizer();

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

void init() {
    current = oneStream.begin();
    currentType = current->tag;

    // Clear the alias map
    aliasMap.clear();
    lastId = "!";

    // Clear the select map
    selectMap.clear();

    parenNum = 1;
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

// Match and return number
void matchReturnNum(int first, int& num) {
    if ( currentType == first ) {
        num = current->num;
        // Move forward
        ++current;
        currentType = current->tag;
    } else {
        error();
    }
}

void output() {
    /*
    map<string, string>::iterator it = aliasMap.find(lastId);
    if ( it != aliasMap.end() ) {
        cout << it->first << " " << it->second << endl;
    } else {
        cout << lastId << endl;
    }
    */

            // Test output
            map<string, map<string, vector<Word> > >::iterator viewIt = view.begin();
            for ( ; viewIt != view.end(); ++viewIt ) {
                cout << "View: " << viewIt->first << endl;
                map<string, vector<Word> > col = viewIt->second;
                map<string, vector<Word> >::iterator colIt = col.begin();
                for ( ; colIt != col.end(); ++colIt ) {
                    cout << "   Col: " << colIt->first << endl;
                    vector<Word> w = colIt->second;
                    vector<Word>::iterator wIt = w.begin();
                    for ( ; wIt != w.end(); ++wIt ) {
                        cout << "       " << wIt->content << endl; 
                    }
                }
                cout << endl;
            }
            
}

void select() {
    /* Example
        select A.B as C
            from D A;

        selectMap: C -> (A, B)
        aliasMap: A -> D
     */

    /* Still has a problem in error report.
     * e.g. When the view is not correct,
     * we can only report that the col specified is in that view.
     */
    map<string, struct selectInfo>::iterator selectMapIt = selectMap.begin();
    for ( ; selectMapIt != selectMap.end(); ++selectMapIt ) {
        // A
        string alias = selectMapIt->second.selectView;
        // B
        string col = selectMapIt->second.selectCol;
        // C
        string newCol = selectMapIt->first;

        map<string, string>::iterator aliasFoundIt = aliasMap.find(alias);
        string real;
        if ( aliasFoundIt != aliasMap.end() ) {
            // D
            real = aliasFoundIt->second;
        } else {
            notFoundError(alias);
        }

        map<string, vector<Word> > viewReal = view[real];
        map<string, vector<Word> >::iterator realColFoundIt = viewReal.find(col);
        vector<Word> selectCol;
        if ( realColFoundIt != viewReal.end() ) {
            selectCol = realColFoundIt->second;
        } else {
            notFoundError(col);
        }
        
        view[viewId][newCol] = selectCol;
    }
}
