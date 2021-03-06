#include<iostream>
#include<fstream>
#include"regex.cpp"
using namespace std;

vector<Pre_token> all_token;

int getTypeTokenizer(char c) {
	// invalid char, return 0
	if (c == '\t' || c == '\r' || c == '\n' || c == ' ') return 0;
	// int or char, return 1
	else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
	// other letters , exp: , . ( )
	else return 2;
}

vector<int> getAllTokenPosition(int start, int end) {
	vector<int> po;
	vector<Pre_token>::iterator it;
	int flag = 0;
	for (it = all_token.begin(); it != all_token.end(); it++) {
		if (flag == 1) {
			if (it->start >= end) break;
			po.push_back(it->position);
			if (end <= it->end) break;
		}
		else {
			if (start <= it->start && it->start < end) {
				po.push_back(it->position);
				flag = 1;
				if (end <= it->end) break;
			}
			else if (it->start <= start && it->end > start) {
				po.push_back(it->position);
				flag = 1;
				if (end <= it->end) break;
			}
		}
	}
	return po;
}


vector< vector<Word> >tokenizer(const char* regex) {
	vector< vector<Word> > all_catch;
	// read all the contents of a text one time
	int length;

	documentIfs.seekg(0, ios::end);
	length = documentIfs.tellg();
	documentIfs.seekg(0, ios::beg);

	char* cont = new char[length];

	documentIfs.read(cont, length);

	// get the cursor back to the begin.
	documentIfs.seekg(0, ios::beg);

	vector<vector<int> > coordinates = findall(regex, cont);
	
	for ( int i = 0; i < coordinates[0].size(); i += 2 ) {
		vector<vector<int> >::iterator it = coordinates.begin();
		vector<Word> all_word;
		for (; it != coordinates.end(); it++) {
			vector<int> v;
			int begin = (*it)[i];
			int end = (*it)[i + 1];
			// cout << begin << " " << end << endl;
			v = getAllTokenPosition(begin, end);
			string content = "";
			for (int i = begin; i < end; i++) {
				content += cont[i];
			}
			Word new_word(content, begin, end, v);
			// cout << new_word.content << "--\n";
			all_word.push_back(new_word);
		}
		all_catch.push_back(all_word);
	}

	documentIfs.clear();
	documentIfs.seekg(0, documentIfs.beg);

	return all_catch;
}


void pre_tokenizer() {
	all_token.clear();
	int position = 0;
	int lookahead = 0;
	string cont = "";
	char cur;
	int start = 0, end = 0;
	while ((cur = documentIfs.get()) != EOF) {
		end++;
		int cur_type = getTypeTokenizer(cur);
		if (cur_type == 0) {
			if (lookahead == 1) {
				Pre_token ntoken(cont, start, end - 1, position);
				position++;
				all_token.push_back(ntoken);
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
				Pre_token ntoken(cont, start, end - 1, position);
				position++;
				all_token.push_back(ntoken);
				// save single letter
			}

			cont = "";
			cont += cur;
			Pre_token ntoken2(cont, end - 1, end, position);
			position++;
			all_token.push_back(ntoken2);
			
			lookahead = 2;
			start = end;
			cont = "";
		}
	}
	if (cont != "") {
		Pre_token ntoken2(cont, start, end - 1, position);
		position++;
		all_token.push_back(ntoken2);
	}

	documentIfs.clear();
	documentIfs.seekg(0, documentIfs.beg);
}
