#include<string>
#include<cstring>
using namespace std;

class Token {
    public:
        int tag;
        int num;
        string idReg;
        Token(int tag, int num = 0, string idReg = "") :tag(tag), num(num), idReg(idReg)  {
        }
};

/*
// Test
#include<iostream>

int main() {
    Token t(NUM, 3);
    cout << t.tag << " " << t.num << " " << t.idReg << endl;
    
}
*/