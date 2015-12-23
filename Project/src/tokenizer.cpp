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
			if(end == it->end) break;
		} else {
			if (start <= it->start && it->start < end) {
				po.push_back(it->position);
				flag = 1;
				if(end == it->end) break;
			}
		}
	}
	return po;
}


vector<Word> tokenizer(ifstream& ip, const char* regex) {
	vector<Word> all_word;
	// read all the contents of a text one time
	int length;

	ip.seekg(0, ios::end);
	length = ip.tellg();
	ip.seekg(0, ios::beg);

	char* cont = new char[length];

	ip.read(cont, length);

	//get the cursor back to the begin.
	ip.seekg(0, ios::beg);

	vector<vector<int> > coordinates = findall(regex, cont);
	vector<vector<int> >::iterator it = coordinates.begin();
	
	for (; it != coordinates.end(); it++) {
		vector<int> v;
		int begin = (*it)[0];
		int end = (*it)[1];
		v = getAllTokenPosition(begin, end);
		string content = "";
		for (int i = begin; i < end; i++) {
			content += cont[i];
		}
		Word new_word(content, begin, end, v);
		all_word.push_back(new_word);
	}

	return all_word;
}


void pre_tokenizer(ifstream& ip) {
	int position = 0;
	int lookahead = 0;
	string cont = "";
	char cur;
	int start = 0, end = 0;
	while ((cur = ip.get()) != EOF) {
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
}

/*
int main() {
	vector<Word> v;
	ifstream file("Revenue.input");
	ifstream file4("Revenue.input");
	ofstream file2("compiler_out.txt");
	ofstream file3("compiler_out2.txt");
	char regex[] = ".[0-9]+(\\.[0-9])+";
	pre_tokenizer(file);
	v = tokenizer(file4, regex);
	vector<Word>::iterator it = v.begin();
	for (; it != v.end(); it++) {
		file2 << it->content << "(" << it->start << "," << it->end << ")" << " ";
		
		vector<int>::iterator itt = (it->include).begin();
		for(; itt != (it->include).end(); itt++) {
			file2 << *itt << " ";
		}
		file2 << endl;
	}
	
	vector<Pre_token>::iterator it2;
	for (it2 = all_token.begin(); it2 != all_token.end(); it2++) {
		file3 << it2->content << "(" << it2->start << "," << it2->end << ")"<< " " << it2->position << endl;
	}	
	return 0;
}
*/