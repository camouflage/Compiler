#include"tokenizer.cpp"
#include"PatternClass.h"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

Word merge_word(Word first, Word second) {

    string newCont = first.content;

    int numOfspace = 0;

    if (first.end < second.start) {
        numOfspace = second.start - first.end;
    }

    for (int i = 0; i < numOfspace; i++) {
        newCont += " ";
    }

    newCont += second.content;
    int newStart = first.start;
    int newEnd = second.end;
    vector<int> newInclude;
    newInclude.push_back(first.include.front());
    newInclude.push_back(second.include.back());

    return Word(newCont, newStart, newEnd, newInclude);
}


vector<Word> getNewResult(vector<Word> result, vector<Word> pmv, int minToken, int maxToken) {
    
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
    vector<Word> new_result;
    vector<Word>::iterator r_it = result.begin(), pm_it;
    //match: one-to-one corresponding
    for (; r_it != result.end(); r_it++) {
        for (pm_it = pmv.begin(); pm_it != pmv.end(); pm_it++) {
            // match: succeed

            //cout << pm_it->include.front() << " " << r_it->include.back() << endl;

            if (pm_it->include.front() - r_it->include.back() <= (maxToken + 1) && 
                pm_it->include.front() - r_it->include.back() >= (minToken + 1)) {

                Word new_word = merge_word(*r_it, *pm_it);
                new_result.push_back(new_word);

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


vector<Word> match_pattern(vector<PatternMatch> pm) {
    vector<Word> result;
    vector<PatternMatch>::iterator pat = pm.begin();
    // get the first expression
    bool first = true;
    for (; pat != pm.end(); pat++) {
        // the first expression cannot be Token<>{};
        if (first) {
            if (pat->type == 1) {
                result = pat->column;
            } 
            else if (pat->type == 3) {
                result = tokenizer((pat->reg).c_str());
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
                result = getNewResult(result, pat->column, 0, 0);
            }

            else if (pat->type == 2) {
                // Assuming that there must be a <column> or REG behind <Token>
                // <column> + <Token> + <column>
                if ((pat+1)->type == 1) {
                    result = getNewResult(result, (pat+1)->column, pat->token_min, pat->token_max);
                }
                // <column> + <Token> + REG
                else if ((pat+1)->type == 3){
                    //vector<Word> regex = tokenizer(((pat+1)->reg).c_str());
                    vector<Word> regex = tokenizer(((pat+1)->reg).c_str());
                    result = getNewResult(result, regex, pat->token_min, pat->token_max);
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

                result = getNewResult(result, regex, 0, 0);
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

    return result;
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