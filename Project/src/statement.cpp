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
void name_spec();
void group_spec();
void single_group();
void optSubSingle_group();
vector<PatternMatch> pattern_expr();
void pattern_pkg(vector<PatternMatch>& pm);
void optSubPattern_pkg();
void pattern_group();
void atom(PatternMatch& currentPc);
void optRange(PatternMatch& currentPc);


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
        case EXTRACT:
            match(EXTRACT);
            extract_spec();
            match(FROM);
            from_list();
            break;
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

            /*
            vector<Word>::iterator it = v.begin();
            for (; it != v.end(); it++) {
                cout << it->content << "(" << it->start << "," << it->end << ")" << " ";
                vector<int>::iterator itt = (it->include).begin();
                for(; itt != (it->include).end(); itt++) {
                    cout << *itt << " ";
                }
                cout << endl;
            }
            */

            match(ON);
            column();
            name_spec();

            // Test selectMap
            map<string, struct selectInfo>::iterator it = selectMap.begin();
            for ( ; it != selectMap.end(); ++it ) {
                cout << it->first << " " << it->second.selectView << " " << it->second.selectCol << endl;
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

            si.selectView = selectView;
            si.selectCol = selectCol;
            break;
        }
        default:
            error();
            break;
    }
}

void name_spec() {
    switch ( currentType ) {
        case AS:
            match(AS);
            matchReturnId(ID, colName);
            selectMap.insert( pair<string, struct selectInfo>(colName, si) );
            break;
        case RETURN:
            match(RETURN);
            group_spec();
            break;
    }
}

void group_spec() {
    switch ( currentType ) {
        case GROUP:
            single_group();
            optSubSingle_group();
            break;
        default:
            error();
            break;
    }
}

void single_group() {
    switch ( currentType ) {
        case GROUP: {
            match(GROUP);
            string num;
            matchReturnId(NUM, num);
            match(AS);
            match(ID);
            break;
        }
        default:
            error();
            break;
    }
}

void optSubSingle_group() {
    switch ( currentType ) {
        case AND:
            match(AND);
            group_spec();
            break;
        default:
            break;
    }
}

void pattern_spec() {
    switch ( currentType ) {
        case PATTERN:
            match(PATTERN);
            pattern_expr();
            name_spec();
            break;
        default:
            error();
            break;
    }
}

vector<PatternMatch> pattern_expr() {
    vector<PatternMatch> pm;
    switch ( currentType ) {
        case '<':
        case REG:
        case '(': {
            pattern_pkg(pm);
            optSubPattern_pkg();

            vector<PatternMatch>::iterator it = pm.begin();
            for ( ; it != pm.end(); ++it ) {
                cout << it->type << endl;
            }
            
            break;
        }
        default:
            error();
            break;
    }
    return pm;
}

void pattern_pkg(vector<PatternMatch>& pm) {
    switch ( currentType ) {
        case '<':
        case REG: {
            PatternMatch currentPc;
            atom(currentPc);
            optRange(currentPc);
            pm.push_back(currentPc);
            break;
        }
        case '(':
            pattern_group();
            break;
        default:
            error();
            break;
    }
}

void optSubPattern_pkg() {
    switch ( currentType ) {
        case '<':
        case REG:
        case '(':
            pattern_expr();
            break;
        default:
            break;
    }
}

void optRange(PatternMatch& currentPc) {
    switch ( currentType ) {
        case '{':
            match('{');
            int min, max;
            matchReturnNum(NUM, min);
            match(',');
            matchReturnNum(NUM, max);
            currentPc.token_min = min;
            currentPc.token_max = max;
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
                    currentPc.selectView = si.selectView;
                    currentPc.selectCol = si.selectCol;
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

void pattern_group() {
    switch ( currentType ) {
        case '(': {
            match('(');
            int currentParen = parenNum++;
            pattern_expr();
            match(')');
            break;
        }
        default:
            error();
            break;
    }
}
