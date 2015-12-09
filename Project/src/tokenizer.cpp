#include<iostream>
#include<fstream>
#include<vector>
#include"Word.h"
#include"regex.cpp"
using namespace std;

int getType(char c) {
	// invalid char, return 0
	if (c == '\t' || c == '\r' || c == '\n' || c == ' ') return 0;
	// int or char, return 1
	else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
	// other letters , exp: , . ( )
	else return 2;
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

	vector<vector<int>> coordinates = findall(regex, cont);
	vector<vector<int>>::iterator it = coordinates.begin();
	for (; it != coordinates.end(); it++) {
		vector<int>::iterator it2 = (*it).begin();
		for (; it2 != (*it).end(); it2++) {
			cout << *it2 << " ";
		}
		cout << endl;
	}

	return all_word;
}


int main() {
	vector<Word> v;
	ifstream file("complier.txt");
	ofstream file2("complier_out.txt");
	char regex[] = "[A-Z][a-z]*";
	v = tokenizer(file, regex);
	/*vector<Word>::iterator it = v.begin();
	for (; it != v.end(); it++) {
		file2 << it->getContent() << "(" << it->getStart() << "," << it->getEnd() << ")" << endl;
	}*/
	return 0;
}