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
    // Path of AQL
    char aqlPath[FILELEN];
    snprintf(aqlPath, FILELEN, "../dataset/%s", argv[1]);
    ifstream aqlIfs(aqlPath);
    // Cannot open AQL file
    if ( !aqlIfs ) {
        cerr << "Could not open aql file(" <<  aqlPath << ")!" << endl;
        return 2;
    }

    // Path of document
    char documentPath[FILELEN];
    // Length of document's name, not considering extension
    int nameLen = strlen(argv[2]) - NAMELEN;
    char outputPath[FILELEN];
    char dotOutput[8] = ".output";
    
    // Single input file case(with .input as the ending of the argument)
    if (  isInput(argv[2], nameLen) ) {
        // File name with lower case character
        char lowerNamePath[FILELEN];
        snprintf(lowerNamePath, nameLen + 1, "%s", argv[2]);
        // Get path of output 
        snprintf(outputPath, FILELEN, "../dataset/%s/%s%s", lowerNamePath, lowerNamePath, dotOutput);
        
        // Convert to lower case, get directory name
        for ( int i = 0; i < strlen(lowerNamePath); ++i ) {
            lowerNamePath[i] = tolower(lowerNamePath[i]);
        }

        snprintf(documentPath, FILELEN, "../dataset/%s/%s", lowerNamePath, argv[2]);
        documentIfs.open(documentPath);
        outputOfs.open(outputPath);
        // Cannot open document
        if ( !documentIfs ) {
            cerr << "Could not open document file(" << documentPath << ")!" << endl;
            return 1;
        } else {
            if ( !outputOfs ) {
                cerr << "Could not open output file(" << outputPath << ")!" << endl;
                return 1;
            } else {
                outputOfs << "Processing " << argv[2] << endl;
                // MAIN FUNCTION AQL
                AQL(aqlIfs);
            }
        }
    } else { // Given directory
        snprintf(documentPath, FILELEN, "../dataset/%s", argv[2]);
        // Ref: http://baike.baidu.com/link?url=sIJz87W5ckf74dcpsKIE1paczSx201EPFMND9J7aM2j-lzGs5l_6EdT9fd6mD-jVgrhJfAoHPY9CT9Je1xugF_
        DIR* dp = opendir(documentPath);

        if ( dp == NULL ) {
            cerr << "Could not find the directory(" << documentPath << ")!" << endl;
            return 3;
        } else {
            struct dirent* dirp = readdir(dp);
            // Iterate through the directory
            while ( dirp != NULL ) {
                // Choose those that are .input file
                int fileNameLen = strlen(dirp->d_name) - NAMELEN;
                if ( isInput(dirp->d_name, fileNameLen) ) {
                    snprintf(documentPath, FILELEN, "../dataset/%s/%s", argv[2], dirp->d_name);

                    // Get fileName
                    char fileName[FILELEN];
                    snprintf(fileName, fileNameLen + 1, "%s", dirp->d_name);
                    // Append .output
                    snprintf(outputPath, FILELEN, "../dataset/%s/%s%s", argv[2], fileName, dotOutput);
                    documentIfs.open(documentPath);
                    outputOfs.open(outputPath);
                    // Cannot open document
                    if ( !documentIfs ) {
                        cerr << "Could not open document file(" << documentPath << ")!" << endl;
                        return 1;
                    } else {
                        if ( !outputOfs ) {
                            cerr << "Could not open output file(" << outputPath << ")!" << endl;
                            return 1;
                        } else {
                            outputOfs << "Processing " << dirp->d_name << endl;
                            // MAIN FUNCTION AQL
                            AQL(aqlIfs);
                            // Move back to beginning
                            aqlIfs.clear();
                            aqlIfs.seekg(0, aqlIfs.beg);
                            documentIfs.clear();
                            documentIfs.seekg(0, documentIfs.beg);
                        }
                    }
                }
                dirp = readdir(dp);
            }
        }
    }
}
