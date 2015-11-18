bison -d -v regex.y &&
lex regex.l && 
gcc -g lex.yy.c regex.tab.c -ly -ll -o regex &&
./regex < test.input > output &&
cat output &&
diff output regex.ans