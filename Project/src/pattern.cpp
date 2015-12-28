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


// Note: group 0 saved as the last element.
vector<vector<Word> > patternGroup;

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

    Pre_token startToken = all_token[first.include.back()];
    Pre_token endToken = all_token[second.include.front()];

    // <column> + <Token> + <column>
    // solve Token and other invlalid characters such as space;
    if (startToken.end > first.end) {
        for (int i = first.end; i <= startToken.end; i++) {
            newCont += startToken.content[i - startToken.start];
        }
    }


    // if there are Tokens between the two word, add them to the new word's content
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


vector<resultStrt> getNewResult(vector<resultStrt> result, vector<Word> pmv, int minToken, int maxToken, bool isGroup) {

    // save the result we get after matching.
    vector<resultStrt> new_result;
    vector<resultStrt>::iterator r_it = result.begin();
    vector<Word>::iterator pm_it;
    //match: one-to-one corresponding
    for (; r_it != result.end(); r_it++) {
        int position_of_pm = 0;
        for (pm_it = pmv.begin(); pm_it != pmv.end(); pm_it++, position_of_pm++) {

            // match: succeed
            if (pm_it->include.front() - (r_it->word).include.back() <= (maxToken + 1) && 
                pm_it->include.front() - (r_it->word).include.back() >= (minToken + 1)) {

                Word new_word = merge_word((*r_it).word, *pm_it);
                vector<int> ele = (*r_it).validElement;

                // if it's a Group, the push_back the subscript into the vector
                if (isGroup) {
                    ele.push_back(position_of_pm);
                }
                resultStrt res(new_word, ele);

                new_result.push_back(res);
            }
        }
    }

    return new_result;
}

// Initialize when we read the first element
vector<resultStrt> Initialize_Result(vector<resultStrt> result, vector<Word> column, bool isGroup) {
    for (int i = 0; i < column.size(); i++) {
        vector<int> ele;
        if (isGroup) {
            ele.push_back(i);
        }
        resultStrt res(column[i], ele);
        result.push_back(res);
    }
    return result;
}

vector<Word> match_pattern(vector<PatternMatch> pm, bool isLast) {
    
    vector<resultStrt> result;
    vector<PatternMatch>::iterator pat = pm.begin();
    // get the first expression
    bool first = true;
    for (; pat != pm.end(); pat++) {
        // the first expression cannot be Token<>{};
        if (first) {
            if (pat->type == 1) {
                if (pat->isGroup) {
                    
                    patternGroup.push_back(pat->column);
                }
                result = Initialize_Result(result, pat->column, pat->isGroup);
            } 
            else if (pat->type == 3) {
                vector<Word> v = tokenizer((pat->reg).c_str());
                if (pat->isGroup) {
                    patternGroup.push_back(v);
                }
                result = Initialize_Result(result, v, pat->isGroup);
                //result = tokenizer((pat->reg).c_str());
            } else {
                cerr << "Invalid Pattern" << endl;
                exit(1);
            }
            first = false;
        } else {
            // <column> + <column>
            if (pat->type == 1) {
                // cover the old result using the new result
                if (pat->isGroup) {

                    patternGroup.push_back(pat->column);
                }
                result = getNewResult(result, pat->column, 0, 0, pat->isGroup);
            }

            else if (pat->type == 2) {
                // Assuming that there must be a <column> or REG behind <Token>
                // <column> + <Token> + <column>
                if ((pat+1)->type == 1) {
                    if ((pat+1)->isGroup) {

                        patternGroup.push_back((pat+1)->column);
                    }
                    result = getNewResult(result, (pat+1)->column, pat->token_min, pat->token_max, (pat+1)->isGroup);
                }
                // <column> + <Token> + REG
                else if ((pat+1)->type == 3){
                    //vector<Word> regex = tokenizer(((pat+1)->reg).c_str());
                    vector<Word> regex = tokenizer(((pat+1)->reg).c_str());
                    if ((pat+1)->isGroup) {

                        patternGroup.push_back(regex);
                    }
                    result = getNewResult(result, regex, pat->token_min, pat->token_max, (pat+1)->isGroup);
                }
                pat++;
            }
            else if (pat->type == 3) {
                // <column> + REG
                // vector<Word> regex = tokenizer((pat->reg).c_str());
                vector<Word> regex = tokenizer((pat->reg).c_str());

                result = getNewResult(result, regex, 0, 0, pat->isGroup);
            }
        }

    }

    // get the reuslt whose type is vector<Word> from [vector<resultStrt> result];
    vector<Word> new_result;
    vector<resultStrt>::iterator res = result.begin();
    for (; res != result.end(); res++) {
        new_result.push_back(res->word);
    }


    if (isLast) {
        vector<vector<Word> >::iterator out_it = patternGroup.begin();
        vector<resultStrt>::iterator it = result.begin();

        for (int i = 0; out_it != patternGroup.end(); out_it++, i++) {
            vector<Word> newvector;

            for (it = result.begin(); it != result.end(); it++) {
                int pos = it->validElement[i];
                newvector.push_back((*out_it)[pos]);
            }
            *out_it = newvector;
        }
        patternGroup.push_back(new_result);

    }
    
    return new_result;
}

