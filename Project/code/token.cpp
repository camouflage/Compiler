#include"tag.h"

class Token {
    public:
        int tag;
        Token(int tag) :tag(tag) {
        }
};

class Number: public Token {
    public:
        int value;
        Number(int tag, int value) :Token(tag), value(value) {
        }
};

/*
// Test
#include<iostream>
using namespace std;

int main() {
    Token t(NUM);
    cout << t.tag << endl;
    Number n(NUM, 10);
    cout << n.tag << " " << n.value << endl;
}
*/
