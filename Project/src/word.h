// Class Word to save each cell in the view
#ifndef WORD
#define WORD

#include<iostream>
#include<string>
#include<vector>
using namespace std;

class Word {
	public:
		string content;
		int start;
		int end;
		vector<int> include;
		Word() {
			content = "";
			start = end = 0;
		}
		Word(string c, int s, int e, vector<int> v) {
			content = c;
			start = s;
			end = e;
			include = v;
		}
};

#endif