#include"global.h"
#include"auxiliary.cpp"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

void aql_stmt();
void create_stmt();
void output_stmt();
void optAlias();
void optSelectAlias(string& selectColName);
void view_stmt();
void select_stmt();
void select_list();
void select_item();
void optSubSelect_item();
void from_list();
void from_item();
void optSubFrom_item();

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
            matchReturnId(ID, viewId);
            match(AS);
            view_stmt();
            break;
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
            optAlias();
            break;
        default:
            error();
            break;
    }
}

void optAlias() {
    switch ( currentType ) {
        case AS:
            match(AS);
            matchInsertAlias(ID);
            break;
        default:
            break;
    }
}

void view_stmt() {
    switch ( currentType ) {
        case SELECT:
            select_stmt();
            break;
        /*
        case EXTRACT:
            extract_stmt();
            break;
        */
        default:
            error();
            break;
    }
}

void select_stmt() {
    switch ( currentType ) {
        case SELECT: {
            match(SELECT);
            select_list();
            
            // Test selectMap
            map<string, struct selectInfo>::iterator it = selectMap.begin();
            for ( ; it != selectMap.end(); ++it ) {
                cout << it->first << " " << it->second.selectView << " " << it->second.selectCol << endl;
            }
            

            match(FROM);
            from_list();
            /*
            // Test aliasMap
            map<string, string>::iterator it = aliasMap.begin();
            for ( ; it != aliasMap.end(); ++it ) {
                cout << it->first << " " << it->second << endl;
            }
            */

            // Key function
            // Select
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

            break;
        }
        default:
            error();
            break;
    }
}

void select_list() {
    switch ( currentType ) {
        case ID:
            select_item();
            optSubSelect_item();
            break;
        default:
            error();
            break;
    }
}

void select_item() {
    switch ( currentType ) {
        // error: switch case is in protected scope
        case ID: {
            matchReturnId(ID, selectView);
            match('.');
            matchReturnId(ID, selectCol);

            struct selectInfo si;
            si.selectView = selectView;
            si.selectCol = selectCol;

            optSelectAlias(selectColName);

            selectMap.insert( pair<string, struct selectInfo>(selectColName, si) );
            break;
        }
        default:
            error();
            break;
    }
}

void optSelectAlias(string& selectColName) {
    switch ( currentType ) {
        case AS:
            match(AS);
            matchReturnId(ID, selectColName);
            break;
        default:
            selectColName = selectCol;
            break;
    }
}

void optSubSelect_item() {
    switch ( currentType ) {
        case ',':
            match(',');
            select_list();
            break;
        default:
            break;
    }
}

void from_list() {
    switch ( currentType ) {
        case ID:
            from_item();
            optSubFrom_item();
            break;
        default:
            error();
            break;
    }
}

void from_item() {
    switch ( currentType ) {
        case ID:
            matchInsertAlias(ID);
            matchInsertAlias(ID);
            break;
        default:
            error();
            break;
    }
}

void optSubFrom_item() {
    switch ( currentType ) {
        case ',':
            match(',');
            from_list();
            break;
        default:
            break;
    }
}
