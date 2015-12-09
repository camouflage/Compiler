#include"aql.cpp"
#include<iostream>
#include<fstream>
#include<cstring>
#include<dirent.h>

#define FILELEN 100
#define NAMELEN 6
using namespace std;

// Return true if the extesion is .input
bool isInput(char* fileName, int nameLen) {
    char* extension = &fileName[nameLen];
    return strcmp(extension, ".input") == 0;
}

int main(int argc, char* argv[]) {
    char aqlPath[FILELEN];
    snprintf(aqlPath, FILELEN, "../dataset/%s", argv[1]);
    ifstream aqlIfs(aqlPath);
    // AQL input
    if ( !aqlIfs ) {
        cerr << "Could not open aql file!" << endl;
        return 2;
    }

    char documentPath[FILELEN];
    int nameLen = strlen(argv[2]) - NAMELEN;
    
    // Single input file
    if (  isInput(argv[2], nameLen) ) {
        // File name
        char lowerNamePath[FILELEN];
        strncpy(lowerNamePath, argv[2], nameLen);
        
        // To lower case: directory name
        for ( int i = 0; i < strlen(lowerNamePath); ++i ) {
            lowerNamePath[i] = tolower(lowerNamePath[i]);
        }

        snprintf(documentPath, FILELEN, "../dataset/%s/%s", lowerNamePath, argv[2]);
        ifstream documentIfs(documentPath);
        if ( !documentIfs ) {
            cerr << "Could not open document file!" << endl;
            return 1;
        } else {
            // MAIN FUNCTION
            AQL(aqlIfs, documentIfs);
        }
    } else { // Given directory
        snprintf(documentPath, FILELEN, "../dataset/%s", argv[2]);
        // Ref: http://baike.baidu.com/link?url=sIJz87W5ckf74dcpsKIE1paczSx201EPFMND9J7aM2j-lzGs5l_6EdT9fd6mD-jVgrhJfAoHPY9CT9Je1xugF_
        DIR* dp = opendir(documentPath);

        if ( dp == NULL ) {
            cerr << "Could not find the directory!" << endl;
            return 3;
        } else {
            struct dirent* dirp = readdir(dp);
            while ( dirp != NULL ) {
                // Get input file name
                if ( isInput(dirp->d_name, strlen(dirp->d_name) - NAMELEN) ) {
                    snprintf(documentPath, FILELEN, "../dataset/%s/%s", argv[2], dirp->d_name);
                    ifstream documentIfs(documentPath);
                    if ( !documentIfs ) {
                        cerr << "Could not open document file!" << endl;
                        return 1;
                    } else {
                        // MAIN FUNCTION
                        AQL(aqlIfs, documentIfs);
                    }
                }
                dirp = readdir(dp);
            }
        }
    }
}
