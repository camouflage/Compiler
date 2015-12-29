#ifndef PATTERNMATCH
#define PATTERNMATCH

/*
	@function
	
	This class is used when we transport the pattern we analyzed to the pattern.cpp

*/

#include"word.h"
#include<string>

class PatternMatch {
	public:
		// production rule:
		// atom -> < column > | < Token > | REG

		// <column> --> type = 1, < Token > --> type = 2
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
		PatternMatch(int t, string r, bool isg) {
			type = t;
			reg = r;
		}
		PatternMatch(int t, int ti, int ta, bool isg) {
			type = t;
			token_min = ti;
			token_max = ta;
		}
		PatternMatch(int t, vector<Word> cc, bool isg) {
			type = t;
			column = cc;
		}
		PatternMatch(int t, string r, int ti, int ta, vector<Word> cc, bool isg) {
			type = t;
			reg = r;
			token_min = ti;
			token_max = ta;
			column = cc;
		}

		void output() {
			cout << type << " " << reg << " " << token_min << " " << token_max << endl;
			vector<Word>::iterator it = column.begin();
			for ( ; it != column.end(); ++it ) {
				cout << it->content << endl;
			}
		}
};

#endif