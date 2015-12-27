#include"tokenizer.cpp"
#include"PatternClass.h"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

struct resultStrt {
    Word word;
    vector<int> validElement;
    resultStrt(Word w, vector<int> va) {
        word = w;
        va = validElement;
    } 
};

// Note: group 0 saved as the last element.
vector<vector<Word> > patternGroup;

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



    int newStart = first.start;
    int newEnd = second.end;
    vector<int> newInclude;
    newInclude.push_back(first.include.front());
    newInclude.push_back(second.include.back());

    return Word(newCont, newStart, newEnd, newInclude);
}


vector<resultStrt> getNewResult(vector<resultStrt> result, vector<Word> pmv, int minToken, int maxToken, bool isGroup) {
    
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
                if (isGroup) {
                    ele.push_back(position_of_pm);
                }
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
    cout << "in match_pattern ," << isLast << endl;
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
                //vector<Word> regex = tokenizer((pat->reg).c_str());
                vector<Word> regex = tokenizer((pat->reg).c_str());

                /*
                //test function
                vector<Word>::iterator it = regex.begin();
                for (; it != regex.end(); it++) {
                    cout << it->content << "(" << it->start << "," << it->end << ")" << endl;
                }
                */

                result = getNewResult(result, regex, 0, 0, pat->isGroup);
            }
        }

        /*
        //test function
        if (result.size() != 0) {
            vector<Word>::iterator it = result.begin();
            for (; it != result.end(); it++) {
                cout << it->content << "(" << it->start << "," << it->end << ")" << endl;
            }
        } else {
            cout << "empty" << endl;
        }
        */

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
    }

    vector<Word> new_result;
    vector<resultStrt>::iterator res = result.begin();
    for (; res != result.end(); res++) {
        new_result.push_back(res->word);
    }
    
    return new_result;
}



/*
int main() {
    vector<Word> v, vv;
    ifstream file("Revenue.input");
    ifstream file1("Revenue.input");
    ifstream file11("Revenue.input");
    ifstream f("Revenue.input");
    ofstream file2("pattern_unit.txt");
    ofstream file3("pattern_number.txt");
    ofstream file4("revenue_alltoken.txt");
    ofstream file5("merge_revenue.txt");
    char regex[] = "[0-9]+(\\.[0-9])?";
    char regex2[] = "percent|billion|million|trillion";
    pre_tokenizer(file);
    v = tokenizer(file1, regex);
    vv = tokenizer(file11, regex2);


    vector<Word>::iterator it = vv.begin();
    for (; it != vv.end(); it++) {
        file2 << it->content << "(" << it->start << "," << it->end << ")" << " ";
        
        vector<int>::iterator itt = (it->include).begin();
        for(; itt != (it->include).end(); itt++) {
            file2 << *itt << " ";
        }
        file2 << endl;
    }

    it = v.begin();
    for (; it != v.end(); it++) {
        file3 << it->content << "(" << it->start << "," << it->end << ")" << " ";
        
        vector<int>::iterator itt = (it->include).begin();
        for(; itt != (it->include).end(); itt++) {
            file3 << *itt << " ";
        }
        file3 << endl;
    }
    

    vector<Pre_token>::iterator it2;
    for (it2 = all_token.begin(); it2 != all_token.end(); it2++) {
        file4 << it2->content << "(" << it2->start << "," << it2->end << ")"<< " " << it2->position << endl;
    }

    vector<PatternMatch> pm;
    PatternMatch a(3, "$?");
    PatternMatch aa(1, v);
    PatternMatch aaa(1, vv);
    pm.push_back(a);
    pm.push_back(aa);
    pm.push_back(aaa);

    vector<Word> p = match_pattern(pm, f);
    it = p.begin();
    for (; it != p.end(); it++) {
        file5 << it->content << "(" << it->start << "," << it->end << ")" << endl;
    }


    return 0;
}
*/