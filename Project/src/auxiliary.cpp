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
// Map alias to its id
map<string, string> aliasMap;

// viewId & aliasId
string viewId;
string aliasId;

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

vector<Token> oneStream;
vector<Token>::iterator current;
int currentType;

void starter(ifstream& documentIfs) {
    pre_tokenizer();
}

void error() {
    cerr << "Syntax error!" << endl;
}

void notFoundError(string symbol) {
    cerr << "Symbol " << symbol << " not found!" << endl;
    exit(1);
}

void from() {
    bool hasFrom = 0;
    for ( int i = oneStream.size() - 1; i >= 0; --i ) {
        if ( oneStream[i].tag == FROM ) {
            hasFrom = 1;
            while ( 1 ) {
                Token real = oneStream[++i];
                if ( real.tag == ';' ) {
                    cerr << "Syntax error: Expects ID." << endl;
                    exit(1);
                }

                Token alias = oneStream[++i];
                if ( real.tag == ID && alias.tag == ID ) {

                    if ( aliasMap.count(alias.idReg) > 0 ) {
                        cerr << "Syntax error: Duplicate alias " << alias.idReg << endl;
                        exit(1);
                    } else {
                        aliasMap[alias.idReg] = real.idReg;
                        Token comma = oneStream[++i];
                        if ( comma.tag == ';' ) {
                            return;
                        } else if ( comma.tag != ',' ) {
                            cerr << "Syntax error: Expects Comma." << endl;
                            exit(1);
                        }
                    }
                } else {
                    cerr << "Syntax error: Expects ID." << endl;
                    exit(1);
                }
            }
        }
    }

    if ( hasFrom == 0 ) {
        notFoundError("From");
    }
}

void init() {
    current = oneStream.begin();
    currentType = current->tag;

    // Clear the alias map
    aliasMap.clear();
    lastId = "!";
    aliasId = "!";
    patternGroup.clear();
    // Clear the select map
    selectMap.clear();
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

bool hasView(string viewName) {
    if ( view.count(viewName) > 0 ) {
        return true;
    } else {
        return false;
    }
}

bool hasCol(string viewName, string colName) {
    if ( view[viewName].count(colName) > 0 ) {
        return true;
    } else {
        return false;
    }
}

void output() {
    int maxrow;
    vector<int> colsize;
    vector<string> colname;
    vector< vector<Word>::iterator > wIts;
    stringstream ss;
    string startstr, endstr;

    map<string, map<string, vector<Word> > >::iterator viewIt;
    viewIt = view.find(viewId);
    // map<string, map<string, vector<Word> > >::iterator viewIt = view.begin();
    // for ( ; viewIt != view.end(); ++viewIt ) {
        colsize.clear();
        colname.clear();
        wIts.clear();
        maxrow = 0;

        if ( aliasId == "!" ) {
            cout << "View: " << viewIt->first << endl;
        } else {
            cout << "View: " << aliasId << endl;
        }
        
        map<string, vector<Word> > cols = viewIt->second;
        map<string, vector<Word> >::iterator colIt = cols.begin();
        for ( ; colIt != cols.end(); ++colIt ) {
            colsize.push_back(0);
            colname.push_back(colIt->first);
            vector<Word>::iterator wIt = colIt->second.begin();
            maxrow = colIt->second.size();
            for ( ; wIt != colIt->second.end(); ++wIt ) {
                ss.clear();
                ss << wIt->start;
                ss >> startstr;
                ss.clear();
                ss << wIt->end;
                ss >> endstr;
                wIt->content += ":(" + startstr + "," + endstr + ")";
                if (wIt->content.length() > colsize[colsize.size() - 1]) colsize[colsize.size() - 1] = wIt->content.length();
            }
            wIt = colIt->second.begin();
            wIts.push_back(wIt);
        }
        if (colsize.size() > 0) {
            //seperate row
            cout << "+";
            for (int i = 0; i < colsize.size(); i++) {
                cout << "-";
                for (int j = 0; j < colsize[i]; j++) cout << "-";
                cout << "-+";
            }
            cout << endl;
            //name row
            cout << "|";
            for (int i = 0; i < colsize.size(); i++) {
                cout << ' ';
                for (int j = 0; j < colsize[i]; j++) {
                    if (j < colname[i].length()) cout << colname[i][j];
                    else cout << ' ';
                }
                cout << " |";
            }
            cout << endl;
            //seperate row
            cout << "+";
            for (int i = 0; i < colsize.size(); i++) {
                cout << "-";
                for (int j = 0; j < colsize[i]; j++) cout << "-";
                cout << "-+";
            }
            cout << endl;
            //data rows
            for (int k = 0; k < maxrow; k++) {
                cout << "|";
                for (int i = 0; i < colsize.size(); i++) {
                    cout << ' ';
                    for (int j = 0; j < colsize[i]; j++) {
                        if (j < wIts[i]->content.length()) cout << wIts[i]->content[j];
                        else cout << ' ';
                    }
                    cout << " |";
                    (wIts[i])++;
                }
                cout << endl;
            }
            //seperate row
            cout << "+";
            for (int i = 0; i < colsize.size(); i++) {
                cout << "-";
                for (int j = 0; j < colsize[i]; j++) cout << "-";
                cout << "-+";
            }
            cout << endl;
        }
        cout << maxrow << " rows in set\n\n";
    // }    
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
