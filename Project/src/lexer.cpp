#include"global.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
using namespace std;

// #define does not work here.
const int SPACE = 0;
const int DIGIT = 1;
const int DELIMIT = 2;
const int LETTER = 3;
const int REGEXSTART = 4;
const int STMTEND = 5;
const int INVALID = 6;

map<string, Token> symbol;
int line = 1;
int col = 0;

// Return the type of the char then advance line and col.
int getType(char c) {
    if ( c == '\t' ) {
        col += 3;
        return SPACE;
    } else if ( c == '\n' ) {
        ++line;
        col = 0;
        return SPACE;
    } else if ( c == ' ' ) {
        ++col;
        return SPACE;
    } else if ( c >= '0' && c <= '9' ) {
        ++col;
        return DIGIT;
    } else if ( c == '(' || c == ')' || c == '<' || c == '>' || c == '{' || c == '}' ||
        c == ',' || c == '.' ) {
        ++col;
        return DELIMIT;
    } else if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ) {
        ++col;
        return LETTER;
    } else if ( c == '/' ) {
        ++col;
        return REGEXSTART;
    } else if ( c == ';' ) {
        ++col;
        return STMTEND;
    } else {
        return INVALID;
    }
}

/* If the lookahead does not belong to the previous token,
 * revert the col and row.
 */
void revert(char c) {
    if ( c == '\t' ) {
        col -= 3;
    } else if ( c == '\n' ) {
        --line;
    } else {
        --col;
    }
}

void reserve() {
    // Reserve keywords
    Token create(CREATE);
    symbol.insert(pair<string, Token>("create", create));
    Token view(VIEW);
    symbol.insert(pair<string, Token>("view", view));
    Token as(AS);
    symbol.insert(pair<string, Token>("as", as));
    Token output(OUTPUT);
    symbol.insert(pair<string, Token>("output", output));
    Token select(SELECT);
    symbol.insert(pair<string, Token>("select", select));
    Token from(FROM);
    symbol.insert(pair<string, Token>("from", from));
    Token extract(EXTRACT);
    symbol.insert(pair<string, Token>("extract", extract));
    Token regex(REGEX);
    symbol.insert(pair<string, Token>("regex", regex));
    Token on(ON);
    symbol.insert(pair<string, Token>("on", on));
    Token _return(RETURN);
    symbol.insert(pair<string, Token>("return", _return));
    Token group(GROUP);
    symbol.insert(pair<string, Token>("group", group));
    Token _and(AND);
    symbol.insert(pair<string, Token>("and", _and));
    Token token(TOKEN);
    symbol.insert(pair<string, Token>("token", token));
    Token pattern(PATTERN);
    symbol.insert(pair<string, Token>("pattern", pattern));
}

vector< vector<Token> > lex(ifstream& ifs) {
    // Reserve
    reserve();

    vector< vector<Token> > tokenStream;
    vector<Token> oneStmtToken;
    char current;
    while ( (current = ifs.get()) != EOF ) {
        int type = getType(current);
        if ( type == SPACE ) {
            // Ignore
        } else if ( type == DIGIT ) {
            // Get the actual number
            int v = 0;
            int startCol = col;
            do {
                v = 10 * v + current - '0';
                current = ifs.get();
            } while ( getType(current) == DIGIT );
            ifs.putback(current);
            revert(current);

            Token num(NUM, line, startCol, v);
            oneStmtToken.push_back(num);
        } else if ( type == LETTER ) {
            string buffer;
            int subType;
            int startCol = col;
            do {
                buffer += current;
                current = ifs.get();
                subType = getType(current);
            } while ( subType == LETTER || subType == DIGIT );
            ifs.putback(current);
            revert(current);

            // If it is keyword
            int tag = symbol.find(buffer)->second.tag;
            if ( tag <= 269 ) {
                Token keyword(tag, line, startCol, -1, buffer);
                oneStmtToken.push_back(keyword);
            } else {
                Token keyword(ID, line, startCol, -1, buffer);
                oneStmtToken.push_back(keyword);
                // Insert into symbol table
                symbol.insert(pair<string, Token>(buffer, keyword));
            }
        } else if ( type == DELIMIT ) {
            // Convert char to string
            stringstream ss;
            ss << current;
            string cur;
            ss >> cur;

            Token delimit(current, line, col, -1, cur);
            oneStmtToken.push_back(delimit);
        } else if ( type == REGEXSTART ) {
            string buffer;
            // Ignore '/'
            int startCol = col + 1;
            int subType;
            // Assume that there is no '/' in regex.
            current = ifs.get();
            subType = getType(current);
            while ( subType != REGEXSTART ) {
                // Enforce escape
                if ( subType == SPACE ) {
                    cerr << "Error: invalid regex in AQL"
                         << " at line " << line << " col " << startCol
                         << ": \\t, \\n and space are not allowed, please escape them." << endl;
                    exit(1);
                }

                buffer += current;
                current = ifs.get();
                subType = getType(current);
            }

            Token regex(REG, line, startCol, -1, buffer);
            oneStmtToken.push_back(regex);
        } else if ( type == STMTEND ) {
            Token delimit(';', line, col, -1, ";");
            oneStmtToken.push_back(delimit);

            // Push oneStmtToken to the tokenStream and clear oneStmtToken
            tokenStream.push_back(oneStmtToken);
            oneStmtToken.clear();
        } else {
            // Deal with invalid char
            cerr << "Error: invalid AQL input symbol " << current 
                 << " at line " << line << " col " << col << endl;
            exit(1);
        }
    }

    // Output only for TEST
    for ( int i = 0; i < tokenStream.size(); ++i ) {
        vector<Token>::iterator it = tokenStream[i].begin();
        for ( ; it != tokenStream[i].end(); ++it ) {
            cout << it->tag << " " << it->num << " " << it->idReg << " "
                 << it->line << " " << it->col << endl;
        }
        cout << endl;
    }
    
    return tokenStream;
}
