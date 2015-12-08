#include<iostream>
#include<string>

using namespace std;

class Word {

	public:
		Word() {
			content = "";
			start = end = 0;
		}
		Word(string c, int s, int e) {
			content = c;
			start = s;
			end = e;
		}
		string getContent() {
			return content;
		}
		int getStart() {
			return start;
		}
		int getEnd() {
			return end;
		}
	private:
		string content;
		int start;
		int end;
};
