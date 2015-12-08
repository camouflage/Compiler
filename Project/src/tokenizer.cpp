#include<iostream>
#include<fstream>
#include<vector>
#include"Word.h"
using namespace std;

int getType(char c) {
	// invalid char, return 0
	if (c == '\t' || c == '\r' || c == '\n' || c == ' ') return 0;
	// int or char, return 1
	else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
	// other letters , exp: , . ( )
	else return 2;
}

vector<Word> tokenizer(ifstream& ip) {
	vector<Word> all_word;
	int lookahead = 0;
	string cont = "";
	int start = 0, end = 0;
	char cur;
	while ((cur = ip.get()) != EOF) {
		end++;
		int cur_type = getType(cur);
		if (cur_type == 0) {
			if (lookahead == 1) {
				Word nword(cont, start, end - 1);
				all_word.push_back(nword);
			}
			lookahead = 0;
			start = end;
			// clear the string
			cont = "";
		}
		else if (cur_type == 1) {
			cont += cur;
			lookahead = 1;
		}
		else {
			if (lookahead == 1) {
				// save word
				Word nword(cont, start, end - 1);
				all_word.push_back(nword);
				// save single letter
				cont = "";
				cont += cur;
				Word nword2(cont, end - 1, end);
				all_word.push_back(nword2);
			}
			lookahead = 2;
			start = end;
			cont = "";
		}
	}
	return all_word;
}


int main() {
	vector<Word> v;
	ifstream file("complier.txt");
	ofstream file2("complier_out.txt");
	v = tokenizer(file);
	vector<Word>::iterator it = v.begin();
	for (; it != v.end(); it++) {
		file2 << it->getContent() << "(" << it->getStart() << "," << it->getEnd() << ")" << endl;
	}
	return 0;
}