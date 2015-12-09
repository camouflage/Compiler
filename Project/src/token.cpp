#include<string>
#include<cstring>
using namespace std;

class Token {
    public:
        int tag;
        int num;
        int line;
        int col;
        string idReg;
        Token(int tag, int num = 0, string idReg = "", int line = 0, int col = 0)
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