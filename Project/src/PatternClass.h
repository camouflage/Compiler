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
		// atom -> < column > | < Token >

		// <column> --> type = 1, < Token > --> type = 2
		int type;

		//if <Token>{xx, xx}
		int token_min;
		int token_max;

		// if <column>, save to column
		vector<Word> column;

		PatternMatch() {
			token_min = token_max = type = 0;
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
		PatternMatch(int t, int ti, int ta, vector<Word> cc, bool isg) {
			type = t;
			token_min = ti;
			token_max = ta;
			column = cc;
		}

		void output() {
			// cout << type << " " << " " << token_min << " " << token_max << endl;
			vector<Word>::iterator it = column.begin();
			for ( ; it != column.end(); ++it ) {
				cout << it->content << endl;
			}
		}
};

#endif