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
void optSelectAlias(string& colName);
void view_stmt();
void select_stmt();
void select_list();
void select_item();
void optSubSelect_item();
void from_list();
void from_item();
void optSubFrom_item();
void extract_stmt();
void extract_spec();
void regex_spec();
void pattern_spec();
void column();
void name_spec(vector<vector<Word> >& pg);
void group_spec(vector<vector<Word> >& pg);
void singleGroup(vector<vector<Word> >& pg);
vector<Word> pattern_expr(vector<vector<Word> >& pg);
void atom(PatternMatch& currentPc);
void optRange(PatternMatch& currentPc);


void aql_stmt() {
    switch ( currentType ) {
        case CREATE:
            create_stmt();
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
        case OUTPUT: {
            match(OUTPUT);
            match(VIEW);
            matchReturnId(ID, viewId);
            
            if ( currentType == AS ) {
                match(AS);
                matchReturnId(ID, aliasId);
            }
            break;
        }
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
            match(';');
            break;
        case EXTRACT:
            extract_stmt();
            break;
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
            match(FROM);
            from_list();
            select();
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

            si.selectView = selectView;
            si.selectCol = selectCol;

            optSelectAlias(colName);

            selectMap.insert( pair<string, struct selectInfo>(colName, si) );
            break;
        }
        default:
            error();
            break;
    }
}

void optSelectAlias(string& colName) {
    switch ( currentType ) {
        case AS:
            match(AS);
            matchReturnId(ID, colName);
            break;
        default:
            colName = selectCol;
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
        case ID: {
            from_item();
            optSubFrom_item();
            break;
        }
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

void extract_stmt() {
    switch ( currentType ) {
        case EXTRACT: {
            from();
            /*
            map<string, string>::iterator it = aliasMap.begin();
            for ( ; it != aliasMap.end(); ++it ) {
                cout << it->first << " " << it->second << endl;
            }
            */
            
            match(EXTRACT);
            extract_spec();
            break;
        }
        default:
            error();
            break;
    }
}

void extract_spec() {
    switch ( currentType ) {
        case REGEX:
            regex_spec();
            break;
        case PATTERN:
            pattern_spec();
            break;
        default:
            error();
            break;
    }
}

void regex_spec() {
    switch ( currentType ) {
        case REGEX: {
            match(REGEX);
            string reg;
            matchReturnId(REG, reg);
            vector<Word> v;
            v = tokenizer(reg.c_str());

            match(ON);
            // match ID after ON;
            if (aliasMap.find(current->idReg) != aliasMap.end()) {
                current++;
                currentType = current->tag;
            } else {
                cerr << "Syntax Error: (regex_spec) Match ID after On -- don't exist this ID, " << current->idReg << endl;
                exit(1);
            }

            //match .
            match('.');

            // match text
            if (current->idReg == "text") {
                current++;
                currentType = current->tag;
            } else {
                cerr << "Syntax Error: (regex_spec) column " << current->idReg << " doesn't exist" << endl;
                exit(1);
            }


            switch ( currentType ) {
            	// if D.text has alias, then match it
                case AS:
                    match(AS);
                    matchReturnId(ID, colName);
                    view[viewId][colName] = v;
                    break;

                // D.text doesn't has alias
                case RETURN:
                    match(RETURN);
                    match(GROUP);
                    int num;
                    // get the group number
                    matchReturnNum(NUM, num);
                    if (num != 0) {
                        cout << num << endl;
                        cerr << "Syntax Error: (regex_spec) the number of group should be 0" << endl;
                        exit(1);
                    }
                    match(AS);
                    matchReturnId(ID, colName);
                    view[viewId][colName] = v;
                    break;
            }
            break;
        }
        default:
            error();
            break;
    }
}

void column() {
    switch ( currentType ) {
        case ID: {
            matchReturnId(ID, selectView);
            match('.');
            matchReturnId(ID, selectCol);

            string realView;
            if ( aliasMap.count(selectView) == 0 ) {
                cerr << "Syntax Error: Alias does not exist" << endl;
                exit(1);
            } else {
                realView = aliasMap[selectView];
                if ( realView == "Document" && selectCol == "text" ) {
                    // Do nothing
                } else if ( !hasView(realView) ) {
                    cerr << "Syntax Error: View does not exist" << endl;
                    exit(1);
                } else if ( !hasCol(realView, selectCol) ) {
                    cerr << "Syntax Error: Column does not exist" << endl;
                    exit(1);
                }
            }

            si.selectView = realView;
            si.selectCol = selectCol;
            break;
        }
        default:
            error();
            break;
    }
}

void name_spec(vector<vector<Word> >& pg) {
    switch ( currentType ) {
        case AS:
            match(AS);
            matchReturnId(ID, colName);
            
            view[viewId][colName] = pg[pg.size() - 1];
            break;
        case RETURN:
            match(RETURN);
            group_spec(pg);
            break;
        default:
            error();
            break;
    }
}

void group_spec(vector<vector<Word> >& pg) {
    singleGroup(pg);
    while ( 1 ) {
        if ( currentType == AND ) {
            match(AND);
            singleGroup(pg);
        } else if ( currentType == FROM ) {
            break;
        } else {
            error();
            break;
        }
    }
}

void singleGroup(vector<vector<Word> >& pg) {
    switch ( currentType ) {
        case GROUP: {
            match(GROUP);
            int num;
            matchReturnNum(NUM, num);
            match(AS);
            matchReturnId(ID, colName);
            if ( num == 0 ) {
                view[viewId][colName] = pg[pg.size() - 1];
            } else {
                view[viewId][colName] = pg[num - 1];
            }
            break;
        }
        default:
            error();
            break;
    }
}

void pattern_spec() {
    switch ( currentType ) {
        case PATTERN: {
            match(PATTERN);
            
            pattern_expr(patternGroup);
            name_spec(patternGroup);
            break;
        }
        default:
            error();
            break;
    }
}


vector<Word> pattern_expr(vector<vector<Word> >& pg) {
    vector<PatternMatch> pm;
    vector<Word> result;
    while ( 1 ) {
        if ( currentType == '(' ) {
            match('(');
            vector<Word> temp = pattern_expr(pg);
            PatternMatch pmTemp(1, temp);
            pm.push_back(pmTemp);
        } else if ( currentType == '<' || currentType == REG ) {
            PatternMatch currentPc;
            atom(currentPc);
            optRange(currentPc);
            pm.push_back(currentPc);
        } else if ( currentType == ')' ) {
            match(')');
            break;
        } else if ( currentType == RETURN ) {
            break;
        } else {
            error();
            break;
        }
    }
    result = match_pattern(pm);

    return result;
}

void optRange(PatternMatch& currentPc) {
    switch ( currentType ) {
        case '{':
            match('{');
            matchReturnNum(NUM, currentPc.token_min);
            match(',');
            matchReturnNum(NUM, currentPc.token_max);
            match('}');
            break;
        default:
            break;
    }
}

void atom(PatternMatch& currentPc) {
    switch ( currentType ) {
        case '<': {
            match('<');
            switch ( currentType ) {
                case ID: {
                    column();
                    currentPc.column = view[si.selectView][selectCol];
                    currentPc.type = 1;
                    match('>');
                    break;
                }
                case TOKEN:
                    match(TOKEN);
                    currentPc.type = 2;
                    match('>');
                    break;
                default:
                    error();
                    break;
            }
            break;
        }
        case REG: {
            string reg;
            matchReturnId(REG, reg);
            currentPc.type = 3;
            currentPc.reg = reg;
            break;
        }
        default:
            error();
            break;
    }
}