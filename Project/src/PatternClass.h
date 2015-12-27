#ifndef PATTERNMATCH
#define PATTERNMATCH

#include"word.h"
#include<string>

class PatternMatch {
	public:
		// production rule:
		// atom -> < column > | < Token > | REG

		// <column> --> type = 1, < Token > --> type = 2, REG --> type = 3
		int type;

		// if REG, save the expression using reg;
		string reg;

		//if <Token>{xx, xx}
		int token_min;
		int token_max;

		// if <column>, save to column
		vector<Word> column;
		PatternMatch() {
			token_min = token_max = type = 0;
			reg = "";
		}
		PatternMatch(int t, string r) {
			type = t;
			reg = r;
		}
		PatternMatch(int t, int ti, int ta) {
			type = t;
			token_min = ti;
			token_max = ta;
		}
		PatternMatch(int t, vector<Word> cc) {
			type = t;
			column = cc;
		}
		PatternMatch(int t, string r, int ti, int ta, vector<Word> cc) {
			type = t;
			reg = r;
			token_min = ti;
			token_max = ta;
			column = cc;
		}
		// Only for TEST
		void output() {
			cout << type << " " << reg << " " << token_min << " " << token_max << endl;
			vector<Word>::iterator it = column.begin();
			for ( ; it != column.end(); ++it ) {
				cout << it->content << endl;
			}
		}
};

#endif