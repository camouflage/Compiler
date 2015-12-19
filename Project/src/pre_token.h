#include<iostream>
#include<string>

using namespace std;

class Pre_token {

	public:
		string content;
		int start;
		int end;
		int position;
		Pre_token() {
			content = "";
			start = end = position = 0;
		}
		Pre_token(string c, int s, int e, int p) {
			content = c;
			start = s;
			end = e;
			position = p;
		}
};
