/*

	@function

	This function is used to merge the pattern that extracted from Extract.

	exp: extract pattern (<C.Cap>) /,/ (<S.Stt>)

	We will return the values of (<C.Cap>), (<S.Stt>) and the whole expression.

	And save the values with the format of Group xx.

*/

#include"tokenizer.cpp"
#include"PatternClass.h"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

struct resultStrt {

	// save the word
    Word word;
	// record the element that the word uses from each group
	// exp :   (<A>)<B>(<C>)
	// A has three element: element 0 : a, element 1: b, element 2 : c
	// B has three element: element 0 : d, element 1: e, element 2 : f
	// C has three element: element 0 : g, element 1: h, element 2 : i
	// after merge the table
	// the result is "aei", then validElement is {0, 2};  
	// the result is "adg", then validElement is {0, 0}; 
	// the result is "adh", then validElement is {0, 1};  
	// which only records the subscripts that belong to groups
    vector<int> validElement;
    resultStrt(Word w, vector<int> va) {
        word = w;
        validElement = va;
    } 
};


Word merge_word(Word first, Word second) {

    string newCont = first.content;

	// if there are Tokens between the two word, add them to the new word's content
    Pre_token startToken = all_token[first.include.back()];
    Pre_token endToken = all_token[second.include.front()];

    // <column> + <Token> + <column>
    // solve Token and other invlalid characters such as space;
    if (startToken.end > first.end) {
        for (int i = first.end; i <= startToken.end; i++) {
            newCont += startToken.content[i - startToken.start];
        }
    }

    startToken = all_token[startToken.position + 1];
    for (int i = first.end; i < second.start; i++) {
        if (i == startToken.end) {
            startToken = all_token[startToken.position + 1];
        }
        if (i >= startToken.start && i < startToken.end) {
            newCont += startToken.content[i - startToken.start];
        } else {
            newCont += " ";
        }
    }

    newCont += second.content;

	// merge two word to get a new word
    int newStart = first.start;
    int newEnd = second.end;
    vector<int> newInclude;
    newInclude.push_back(first.include.front());
    newInclude.push_back(second.include.back());

    return Word(newCont, newStart, newEnd, newInclude);
}

vector<resultStrt> getNewResult(vector<resultStrt> result, vector<Word> pmv, int minToken, int maxToken) {
    
    //test function

    /*cout << endl << endl;
    cout << "getNewResult:" << endl;
    cout << "result:" << endl;
    vector<Word>::iterator it = result.begin();
    for (; it != result.end(); it++) {
        cout << it->content << "(" << it->start << "," << it->end << ")" << endl;
    }
    cout << "pmv:" << endl;
    it = pmv.begin();
    for (; it != pmv.end(); it++) {
        cout << it->content << "(" << it->start << "," << it->end << ")" << endl;
    }*/
    

    // save the result we get after matching.
    vector<resultStrt> new_result;
    vector<resultStrt>::iterator r_it = result.begin();
    vector<Word>::iterator pm_it;
    //match: one-to-one corresponding
    for (; r_it != result.end(); r_it++) {
        int position_of_pm = 0;
        for (pm_it = pmv.begin(); pm_it != pmv.end(); pm_it++, position_of_pm++) {
            // match: succeed

            //cout << pm_it->include.front() << " " << r_it->include.back() << endl;

            if (pm_it->include.front() - (r_it->word).include.back() <= (maxToken + 1) && 
                pm_it->include.front() - (r_it->word).include.back() >= (minToken + 1)) {

                Word new_word = merge_word((*r_it).word, *pm_it);
                vector<int> ele = (*r_it).validElement;
                ele.push_back(position_of_pm);
                resultStrt res(new_word, ele);
                new_result.push_back(res);
            }
        }
    }

    /*
    //test function
    cout << "new_result:" << endl;
    it = new_result.begin();
    for (; it != new_result.end(); it++) {
        cout << it->content << "(" << it->start << "," << it->end << ")" << endl;
    }
    */
    return new_result;
}

vector<resultStrt> Initialize_Result(vector<resultStrt> result, vector<Word> column) {
    for (int i = 0; i < column.size(); i++) {
        vector<int> ele;
        ele.push_back(i);
        resultStrt res(column[i], ele);
        result.push_back(res);
    }
    return result;
}

vector<resultStrt> match_pattern(vector<PatternMatch> pm) {
    
    //cout << "in match_pattern ," << isLast << " " << patternGroup.size() << endl;
    vector<resultStrt> result;
    vector<PatternMatch>::iterator pat = pm.begin();
    // get the first expression
    bool first = true;
    for (; pat != pm.end(); pat++) {
        // the first expression cannot be Token<>{};
        if (first) {
            if (pat->type == 1) {
                result = Initialize_Result(result, pat->column);
            } 
            else if (pat->type == 2){
                cerr << "Invalid Pattern" << endl;
                exit(1);
            }
            first = false;
        } else {
            // <column> + <column>
            if (pat->type == 1) {
                // cover the old result using the new result
                result = getNewResult(result, pat->column, 0, 0);
            }

            else if (pat->type == 2) {
                // Assuming that there must be a <column> or REG behind <Token>
                // <column> + <Token> + <column>
                result = getNewResult(result, (pat+1)->column, pat->token_min, pat->token_max);
                pat++;
            }
        }

    }
    
    return result;
}
