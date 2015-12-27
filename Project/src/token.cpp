#include<string>
#include<cstring>
using namespace std;

class Token {
    public:
        // tag used to identify different type of tokens
        int tag;
        // At which line and col
        int line;
        int col;
        // Store number, for tag(NUM)
        int num;
        // Store id or regex, for tag(ID, REG)
        string idReg;
        Token(int tag, int line = -1, int col = -1, int num = -1, string idReg = "")
            :tag(tag), num(num), idReg(idReg), line(line), col(col)   {
        }
};
