#include<string>
#include<cstring>
using namespace std;

class Token {
    public:
        int tag;
        int line;
        int col;
        int num;
        string idReg;
        Token(int tag, int line = -1, int col = -1, int num = -1, string idReg = "")
            :tag(tag), num(num), idReg(idReg), line(line), col(col)   {
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