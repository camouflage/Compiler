#include"tag.h"
#include"token.cpp"
#include"lexer.cpp"
#include<iostream>
#include<fstream>
#include<cstring>

#define LEN 100
using namespace std;

int main(int argc, char* argv[]) {
    char aqlPath[LEN];
    snprintf(aqlPath, LEN, "../dataset/%s", argv[1]);
    ifstream aqlIfs(aqlPath);

    char documentPath[LEN];
    int nameLen = strlen(argv[2]) - 6;
    char* extension = &argv[2][nameLen];

    // Single input file
    if ( strcmp(extension, ".input") == 0 ) {
        char lowerPath[LEN];
        strncpy(lowerPath, argv[2], nameLen);
        
        // To lower case
        for ( int i = 0; i < strlen(lowerPath); ++i ) {
            lowerPath[i] = tolower(lowerPath[i]);
        }

        snprintf(documentPath, LEN, "../dataset/%s/%s", lowerPath, argv[2]);
        ifstream documentIfs(documentPath);
        if ( !documentIfs ) {
            cerr << "Could not open document file!" << endl;
            return 1;
        } else {
            /* tokenizer
             * @params:
                    documentIfs: ifstream
               return: Vector of Word
             */
            // Tokenize(documentIfs);

        }
    } else {
        // TO DO: Specify directory
    }

    // AQL
    if ( !aqlIfs ) {
        cerr << "Could not open aql file!" << endl;
        return 2;
    } else {
        // lexer
        lex(aqlIfs);
    }
}
