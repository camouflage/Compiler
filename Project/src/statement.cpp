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
void regex_name_spec(vector< vector<Word> >& v);
void regex_group_spec(vector< vector<Word> >& v);
void regex_singleGroup(vector< vector<Word> >& v);
void pattern_spec();
void column();
void pattern_name_spec(vector<resultStrt>& result, map<int, vector<int> >& index, vector<PatternMatch>& vpm);
void pattern_group_spec(vector<resultStrt>& result, map<int, vector<int> >& index, vector<PatternMatch>& vpm);
void pattern_singleGroup(vector<resultStrt>& result, map<int, vector<int> >& index, vector<PatternMatch>& vpm);
vector<int> pattern_expr(vector<PatternMatch>& vpm, map<int, vector<int> >& index, int& position, int& paren);
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
            cerr << "Syntax Error: (aql_stmt) Expects CREATE or OUTPUT, but not found" << endl;
            exit(1);
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
            cerr << "Syntax Error: (create_stmt) Expects CREATE, but not found" << endl;
            exit(1);
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
            cerr << "Syntax Error: (output_stmt) Expects OUTPUT, but not found" << endl;
            exit(1);
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
            cerr << "Syntax Error: (view_stmt) Expects SELECT OR EXTRACT, but not found" << endl;
            exit(1);
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
            cerr << "Syntax Error: (select_stmt) Expects SELECT, but not found" << endl;
            exit(1);
    }
}

void select_list() {
    switch ( currentType ) {
        case ID:
            select_item();
            optSubSelect_item();
            break;
        default:
            cerr << "Syntax Error: (select_list) Expects ID, but not found" << endl;
            exit(1);
    }
}

void select_item() {
    switch ( currentType ) {
        // Error: switch case is in protected scope
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
            cerr << "Syntax Error: (select_item) Expects ID, but not found" << endl;
            exit(1);
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
            cerr << "Syntax Error: (from_list) Expects ID, but not found" << endl;
            exit(1);
    }
}

void from_item() {
    switch ( currentType ) {
        case ID:
            matchInsertAlias(ID);
            matchInsertAlias(ID);
            break;
        default:
            cerr << "Syntax Error: (select_item) Expects ID, but not found" << endl;
            exit(1);
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
            cerr << "Syntax Error: (extract_spec) Expects REGEX OR PATTERN, but not found" << endl;
            exit(1);
    }
}

void regex_spec() {
    switch ( currentType ) {
        case REGEX: {
            match(REGEX);
            string reg;
            matchReturnId(REG, reg);
            vector< vector<Word> > v = tokenizer(reg.c_str());
            
            match(ON);
            // match ID after ON;
            if (aliasMap.find(current->idReg) != aliasMap.end()) {
                current++;
                currentType = current->tag;
            } else {
                cerr << "Syntax Error: (regex_spec) Match ID after On -- " << current->idReg << "doesn't exist" << endl;
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

            regex_name_spec(v);
            break;
        }
        default:
            cerr << "Syntax Error: (regex_spec) Expects REGEX, but not found" << endl;
            exit(1);
    }
}

void regex_name_spec(vector< vector<Word> >& v) {
    switch ( currentType ) {
        // if D.text has alias, then match it
        case AS:
            match(AS);
            matchReturnId(ID, colName);
            view[viewId][colName] = v[0];
            break;

        // D.text doesn't has alias
        case RETURN:
            match(RETURN);
            regex_group_spec(v);
            break;
        default:
            cerr << "Syntax Error: (regex_name_spec) Expects AS OR RETURN, but not found" << endl;
            exit(1);
    }
}

void regex_group_spec(vector< vector<Word> >& v) {
    regex_singleGroup(v);
    while ( 1 ) {
        if ( currentType == AND ) {
            match(AND);
            regex_singleGroup(v);
        } else if ( currentType == FROM ) {
            break;
        } else {
            cerr << "Syntax Error: (regex_group_spec) Expects AND OR FROM, but not found" << endl;
            exit(1);
        }
    }
}

void regex_singleGroup(vector< vector<Word> >& v) {
    switch ( currentType ) {
        case GROUP: {
            match(GROUP);
            int num;
            matchReturnNum(NUM, num);
            match(AS);
            matchReturnId(ID, colName);

            view[viewId][colName] = v[num];
            break;
        } 
        default:
            cerr << "Syntax Error: (regex_singleGroup) Expects GROUP, but not found" << endl;
            exit(1);
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
            cerr << "Syntax Error: (column) Expects ID, but not found" << endl;
            exit(1);
    }
}

void pattern_name_spec(vector<resultStrt>& result, map<int, vector<int> >& index, vector<PatternMatch>& vpm) {
    switch ( currentType ) {
        case AS: {
            match(AS);
            matchReturnId(ID, colName);

            vector<Word> g0;
            vector<resultStrt>::iterator it = result.begin();
            for ( ; it != result.end(); ++it ) {
                g0.push_back(it->word);
            }
            view[viewId][colName] = g0;
            break;
        }
        case RETURN:
            match(RETURN);
            pattern_group_spec(result, index, vpm);
            break;
        default:
            cerr << "Syntax Error: (pattern_name_spec) Expects AS OR RETURN, but not found" << endl;
            exit(1);
    }
}

void pattern_group_spec(vector<resultStrt>& result, map<int, vector<int> >& index, vector<PatternMatch>& vpm) {
    pattern_singleGroup(result, index, vpm);
    while ( 1 ) {
        if ( currentType == AND ) {
            match(AND);
            pattern_singleGroup(result, index, vpm);
        } else if ( currentType == FROM ) {
            break;
        } else {
            cerr << "Syntax Error: (pattern_group_spec) Expects AND OR FROM, but not found" << endl;
            exit(1);
        }
    }
}

void pattern_singleGroup(vector<resultStrt>& result, map<int, vector<int> >& index, vector<PatternMatch>& vpm) {
    switch ( currentType ) {
        case GROUP: {
            match(GROUP);
            int num;
            matchReturnNum(NUM, num);
            match(AS);
            matchReturnId(ID, colName);
            vector<Word> match;
            if ( num == 0 ) {
                vector<resultStrt>::iterator it = result.begin();
                for ( ; it != result.end(); ++it ) {
                    match.push_back(it->word);
                }
                view[viewId][colName] = match;
            } else {
                // Save which column is used
                vector<int> col = index[num];
                vector< vector<Word> > all;

                // Iterate through each column and get all words
                for ( int i = 0; i < col.size(); ++i ) {
                    vector<Word> oneCol = vpm[col[i]].column;    
                    vector<Word> valid;

                    vector<resultStrt>::iterator it = result.begin();
                    for ( ; it != result.end(); ++it ) {
                        valid.push_back(oneCol[it->validElement[col[i]]]);
                    }
                    all.push_back(valid);
                    /*
                    for ( int j = 0; j < valid.size(); ++j ) {
                        cout << valid[j].content << endl;
                    }
                    cout << "--\n";
                    */
                }

                vector<Word> match = merge_vector(all);
                view[viewId][colName] = match;
            }
            break;
        }
        default:
            cerr << "Syntax Error: (pattern_singleGroup) Expects GROUP, but not found" << endl;
            exit(1);
    }
}

void pattern_spec() {
    switch ( currentType ) {
        case PATTERN: {
            match(PATTERN);
            vector<PatternMatch> vpm;
            map<int, vector<int> > index;
            int position = 0;
            int paren = 0;
            pattern_expr(vpm, index, position, paren);

            vector<resultStrt> result = match_pattern(vpm);
            pattern_name_spec(result, index, vpm);
            break;
        }
        default:
            cerr << "Syntax Error: (pattern_spec) Expects PATTERN, but not found" << endl;
            exit(1);
    }
}

vector<int> pattern_expr(vector<PatternMatch>& vpm, map<int, vector<int> >& index, int& position, int& paren) {
    vector<int> subIndex;
    int curParen = paren;
    while ( 1 ) {
        if ( currentType == '(' ) {
            match('(');
            vector<int> temp = pattern_expr(vpm, index, position, ++paren);
            for ( int i = 0; i < temp.size(); ++i ) {
                subIndex.push_back(temp[i]);
            }
        } else if ( currentType == '<' || currentType == REG ) {
            PatternMatch currentPc;
            atom(currentPc);
            optRange(currentPc);
            vpm.push_back(currentPc);
            subIndex.push_back(position++);
        } else if ( currentType == ')' ) {
            match(')');
            break;
        } else if ( currentType == RETURN ) {
            break;
        } else {
            cerr << "Syntax Error: (pattern_expr) Expects ( OR < OR REG OR ) OR RETURN, but not found" << endl;
            exit(1);
        }
    }

    index[curParen] = subIndex;
    return subIndex;
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
                    cerr << "Syntax Error: (atom) Expects ID OR TOKEN, but not found" << endl;
                    exit(1);
            }
            break;
        }
        case REG: {
            string reg;
            matchReturnId(REG, reg);
            currentPc.type = 1;
            currentPc.column = tokenizer(reg.c_str())[0];
            break;
        }
        default:
            cerr << "Syntax Error: (atom) Expects < OR REG, but not found" << endl;
            exit(1);
    }
}
